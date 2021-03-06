#include "Player.h"

#include"OBJLoader.h"
#include"SphereCollider.h"
#include"Input.h"
#include"DebugText.h"
#include"CollisionManager.h"
#include"CollisionAttribute.h"
#include "DrawMode.h"
#include "Easing.h"
#include "FBXManager.h"
#include "GameSettingParam.h"
#include "ParticleEmitter.h"

DebugCamera* Player::camera = nullptr;

Player::Player()
{
	//アニメーション用にモデルのポインタを格納
	myModel = FBXManager::GetModel("player");
	//モデルの生成
	Create(myModel);
	//当たり判定(Box)の生成
	BoxCollider* boxCollider = new BoxCollider();
	boxCollider->SetObject(this);
	boxCollider->SetScale({0.2f,0.5f,0.2f});
	boxCollider->SetOffset({ 0,0.5f,0 ,0 });
	SetCollider(boxCollider);
	collider->SetAttribute(COLLISION_ATTR_ALLIES);
	collider->SetMove(true);
	Initialize();

	//定数バッファの作成
	HRESULT result;
	for (int i = 0; i < 3; i++)
	{
		result = DirectXLib::GetInstance()->GetDevice()->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBuffData) + 0xff) & ~0xff),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constBuff[i]));
		assert(SUCCEEDED(result));

	}
	result = DirectXLib::GetInstance()->GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstLightCameraBuff) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constCameraBuff));
	assert(SUCCEEDED(result));

	//ポリゴン分解処理のステータスを初期化
	sendData._Destruction = 1;
	sendData._PositionFactor = 2;
	sendData._RotationFactor = 0.2f;
	sendData._ScaleFactor = 1;
	sendData._Tessellation = 1;
	sendData._OnEasing = 0;

	//デプスシャドウ用のカメラ情報を設定
	ConstLightCameraBuff* constMap2 = nullptr;
	result = constCameraBuff->Map(0, nullptr, (void**)&constMap2);
	assert(SUCCEEDED(result));
	constMap2->cameraPos = Object3D::GetLightCamera()->GetEye();
	constMap2->viewProjection = XMMatrixLookAtLH(Vector3(Object3D::GetLightCamera()->GetEye()).ConvertXMVECTOR(), Vector3(Object3D::GetLightCamera()->GetTarget()).ConvertXMVECTOR(), Vector3(Object3D::GetLightCamera()->GetUp()).ConvertXMVECTOR()) * XMMatrixOrthographicLH(100, 100, 1.0f, 1000.0f);
	constCameraBuff->Unmap(0, nullptr);

}

void Player::Initialize()
{
	name = typeid(*this).name();
	secondJump = true;
	onGround = true;
	scale = { 0.3f };
	position = StartPos;
	rotation = 0;
	prePos = position;
	direction = { 0,0,1 };
	cameraRotCount = 0;
	rotCamera = false;
	radY = 0;
	reset = true;
	appear = false;
	resetMove = false;
	appearCounter = 0;
	disappearCounter = 0;
	resetMoveCounter = 0;
	camera->SetTarget(position + Vector3{ 0, 1, 0 });
	camera->Update();
	//----------カメラの回転角度を算出---------------
	XMMATRIX camMatWorld = XMMatrixInverse(nullptr, camera->GetMatView());
	const Vector3 cameraDirectionZ = Vector3(camMatWorld.r[2].m128_f32[0], 0, camMatWorld.r[2].m128_f32[2]).Normalize();
	float cosA = Vector3(0, 0, 1).Dot(cameraDirectionZ);
	if (cosA > 1.0f)
		cosA = 1.0f;
	else if (cosA < -1.0f)
		cosA = -1.0f;
	float rad = acos(cosA);
	const Vector3 CrossVec = Vector3(0, 0, 1).Cross(cameraDirectionZ);
	if (CrossVec.y < 0)
		rad *= -1;
	//-----------------------------------------------
	//カメラの回転
	camera->AddPhi(rad);
}

void Player::Update()
{
#ifdef _DEBUG
	//パーティクル確認用
	if (Input::TriggerKey(DIK_RETURN))
	{
		ParticleEmitter::CreateExplosion(position);
		ParticleEmitter::CreateAir(position);
		ParticleEmitter::CreateGetEffect(position);

	}
#endif

	//やり直し時の処理
	if(reset)
	{
		ResetPerform();
		camera->SetTarget(position + Vector3{ 0, 1, 0 });
		if (!appear)
			myModel->PlayAnimation("stand", true, 1, false);
		Object::Update();
		return;//やり直し時はここまで
	}
	//移動処理
	Move();
	//カメラのリセット処理
	MoveCamera();
	//壁ジャンプ処理
	WallJump();
	//エアスライド処理
	AirSlide();
	//二段ジャンプ処理
	if (!onGround && !secondJump && !wallJump &&(Input::TriggerKey(DIK_SPACE) || Input::TriggerPadButton(SettingParam::GetJumpButton())))
	{
		secondJump = true;
		fallV = { 0,secondJumpVYFist,0,0 };
		ParticleEmitter::CreateShock(position);
	}
	//落下処理
	if (!onGround)
	{
		//エアスライド中は落下しない
		if (airSlideCounter<=0)
		{
			//加速
			fallV.m128_f32[1] = max(fallV.m128_f32[1] + fallAcc, fallVYMin);
			//移動
			position.x += fallV.m128_f32[0];
			position.y += fallV.m128_f32[1];
			position.z += fallV.m128_f32[2];
		}
	}
	//ジャンプ動作
	else if ((Input::TriggerKey(DIK_SPACE) || Input::TriggerPadButton(SettingParam::GetJumpButton())) &&!jump)
	{
		jump = true;
		onGround = false;
		fallV = { 0,jumpVYFist,0,0 };
		ParticleEmitter::CreateShock(position);
	}
	//他のオブジェクトとのヒットチェック
	CheckHit();

	if (prePos != position)
	{
		camera->SetTarget(position + Vector3{0, 1, 0});
	}
	if (airSlideCounter>0)
		myModel->PlayAnimation("airSlide", false, 1, false);
	else if(secondJump||wallJump)
		myModel->PlayAnimation("secondJump", false,1,false);
	else if(jump)
		myModel->PlayAnimation("jump", false,1,false);

}

void Player::Draw()
{
	
	if(resetMoveCounter != 0)
		return;
	if (!Object3D::GetDrawShadow())
	{
		HRESULT result;
		if (reset)
		{
			ConstBuffData* constMap = nullptr;
			result = constBuff[bbIndex]->Map(0, nullptr, (void**)&constMap);
			assert(SUCCEEDED(result));
			memcpy(constMap, &sendData, sizeof(ConstBuffData));
			constBuff[bbIndex]->Unmap(0, nullptr);
		}

		//定数バッファをセット
		DirectXLib::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(1, constCameraBuff->GetGPUVirtualAddress());
		if (reset)
		{
			DirectXLib::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(5, constBuff[bbIndex]->GetGPUVirtualAddress());
			DirectXLib::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(6, Texture::GetGpuDescHandleSRV("shadowMap" +std::to_string(bbIndex)));  //ヒープの先頭が定数バッファ
		}
		else
		{
			DirectXLib::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(5, Texture::GetGpuDescHandleSRV("shadowMap" + std::to_string(bbIndex)));  //ヒープの先頭が定数バッファ
		}
	}
	CustomDraw(true, true);
}

void Player::DrawReady()
{
#ifdef _DEBUG
	if (!Object3D::GetDrawShadow() && DrawMode::GetDrawImGui())
	{
		XMMATRIX camMatWorld = XMMatrixInverse(nullptr, camera->GetMatView());
		Vector3 cameraDirectionZ = Vector3(camMatWorld.r[2].m128_f32[0], 0, camMatWorld.r[2].m128_f32[2]);
		cameraDirectionZ.Normalize();
		ImGui::Begin("PlayerStatus");
		ImGui::Text("CameraDirection : {%f, %f, %f }\n", cameraDirectionZ.x, cameraDirectionZ.y, cameraDirectionZ.z);
		ImGui::Text("Direction : {%f, %f, %f }\n", direction.x, direction.y, direction.z);
		ImGui::Text("Position : {%f, %f, %f }\n", position.x, position.y, position.z);
		ImGui::SliderFloat("destruction", &sendData._Destruction, 0, 1.0f);
		ImGui::SliderFloat("scaleFactor", &sendData._ScaleFactor, 0, 1.0f);
		ImGui::SliderFloat("positionFactor", &sendData._PositionFactor, 0, 2.0f);
		ImGui::SliderFloat("rotationFactor", &sendData._RotationFactor, 0, 30.0f);
		ImGui::SliderInt("tessellation", &sendData._Tessellation, 1, 32);
		ImGui::SliderInt("onEasing", &sendData._OnEasing, 0, 1);
		ImGui::Checkbox("reset", &reset);

		ImGui::End();
	}
#endif

	if (Object3D::GetDrawShadow())
		pipelineName = "FBXShadowMap";
	else
	{
		if (reset)
			pipelineName = "FBXPolygonBreak";
		else
			pipelineName = "FBX";

	}
}

void Player::Reset()
{
	//出現しきっている状態ならやり直しフラグを立てる
	if (appear)
		reset = true;
}

void Player::Move()
{
	prePos = position;
	//カメラのビュー行列の逆行列を計算
	XMMATRIX camMatWorld = XMMatrixInverse(nullptr, camera->GetMatView());
	const Vector3 cameraDirectionZ = Vector3(camMatWorld.r[2].m128_f32[0], 0, camMatWorld.r[2].m128_f32[2]).Normalize();
	const Vector3 cameraDirectionX = Vector3(camMatWorld.r[0].m128_f32[0], 0, camMatWorld.r[0].m128_f32[2]).Normalize();

	//走りと歩きの切り替え処理
	if(Input::TriggerKey(DIK_X)||Input::TriggerPadButton(SettingParam::GetRunButton()))
	{
		if (run)
		{
			run = false;
			speed = walkSpeed;
		}
		else
		{
			speed = runSpeed;
			run = true;
		}
	}
	//エアスライドの残り時間が10より上なら移動操作せず終了
	if(airSlideCounter>10)
		return;
	
	//移動処理
	if (Input::DownKey(DIK_A) || Input::DownKey(DIK_D) || Input::DownKey(DIK_S) || Input::DownKey(DIK_W)||
		Input::CheckPadLStickDown()|| Input::CheckPadLStickUp() || Input::CheckPadLStickRight() || Input::CheckPadLStickLeft())
	{
		if(wallJump && wallJumpCounter<13)
			return;
		if (onGround)
		{
			walkDustCounter++;
			if (walkDustCounter > 25)
			{
				if (!run)
				{
					ParticleEmitter::CreateWalkDust(position, direction);
				}
				else
				{
					ParticleEmitter::CreateRunDust(position, direction);
				}
				walkDustCounter = 0;
			}
		}
		//移動方向
		Vector3 moveDirection = {};
		if (Input::DownKey(DIK_A))
			moveDirection += cameraDirectionX * -1;
		if (Input::DownKey(DIK_D))
			moveDirection += cameraDirectionX;
		if (Input::DownKey(DIK_S))
			moveDirection += cameraDirectionZ * -1;
		if (Input::DownKey(DIK_W))
			moveDirection += cameraDirectionZ;
		if(Input::CheckPadLStickDown() || Input::CheckPadLStickUp() || Input::CheckPadLStickRight() || Input::CheckPadLStickLeft())
		{
			auto vec = Input::GetLStickDirection();

			moveDirection = cameraDirectionX * vec.x + cameraDirectionZ * vec.y;
		}
		moveDirection.Normalize();

	//回転処理
		//現在の進行方向とカメラの正面と角度を求める
		direction.Normalize();
		float cosA = direction.Dot(moveDirection);
		if (cosA > 1.0f)
			cosA = 1.0f;
		else if (cosA < -1.0f)
			cosA = -1.0f;

		float rotY = acos(cosA) * 180 / 3.14159365f;
		const Vector3 CrossVec = direction.Cross(moveDirection);

		float rotSpeed = rotateSpeed;
		if(abs(rotY) < 55)
			position += moveDirection * speed;

		if (rotSpeed > abs(rotY))
		{
			rotSpeed = rotY;
		}
		if (CrossVec.y < 0)
			rotSpeed *= -1;
		rotation.y += rotSpeed;
		XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(rotSpeed));
		XMVECTOR dir = { direction.x,direction.y,direction.z,0 };
		dir = XMVector3TransformNormal(dir, matRot);
		direction = dir;

		if (!jump&&!secondJump&&!wallJump&&airSlideCounter<=0)
		{
			if (run)
				myModel->PlayAnimation("run", true);
			else
				myModel->PlayAnimation("walk", true);
		}
	}
	else
	{
		if (!jump && !secondJump && !wallJump && airSlideCounter <= 0)
			myModel->PlayAnimation("stand", true);
	}
}

void Player::CheckHit()
{
	//ボックスコライダーを取得
	BoxCollider* boxCollider = dynamic_cast<BoxCollider*>(collider);
	assert(boxCollider);

	//コライダー更新
	Object::Update();
	Ray overDirectionRay;
	overDirectionRay.start = prePos.ConvertXMVECTOR() + boxCollider->GetOffset() + Vector3(0, boxCollider->GetScale().y-0.01f, 0).ConvertXMVECTOR();
	overDirectionRay.dir = direction.ConvertXMVECTOR();
	RaycastHit overDirectionRayCastHit;
	if (CollisionManager::GetInstance()->Raycast(overDirectionRay, boxCollider, COLLISION_ATTR_LANDSHAPE,
		&overDirectionRayCastHit, 5))
	{
		float preToPosLength = Vector3(position - prePos).Length();
		if (preToPosLength > overDirectionRayCastHit.distance)
		{
			Vector3 rejectPos = overDirectionRayCastHit.inter - direction * boxCollider->GetScale().z;
			rejectPos.y = position.y;
			position = rejectPos;
		}
	}
	Ray underDirectionRay;
	underDirectionRay.start = prePos.ConvertXMVECTOR() + boxCollider->GetOffset() - Vector3(0,boxCollider->GetScale().y-0.01f,0).ConvertXMVECTOR();
	underDirectionRay.dir = direction.ConvertXMVECTOR();
	RaycastHit underDirectionRayCastHit;
	if (CollisionManager::GetInstance()->Raycast(underDirectionRay, boxCollider, COLLISION_ATTR_LANDSHAPE,
		&underDirectionRayCastHit, 5))
	{
		float preToPosLength = Vector3(position - prePos).Length();
		if (preToPosLength > underDirectionRayCastHit.distance)
		{
			Vector3 rejectPos = underDirectionRayCastHit.inter - direction * boxCollider->GetScale().z;
			rejectPos.y = position.y;
			position = rejectPos;
		}
	}

	//コライダー更新
	Object::Update();
	
	//ボックスの上端から球の下端までのレイキャスト用レイを準備
	Ray downRay;
	downRay.start = boxCollider->center;
	downRay.start.m128_f32[1] += boxCollider->GetScale().y;
	downRay.dir = { 0,-1,0,0 };
	RaycastHit downRayCastHit;

	//接地状態
	if (onGround)
	{
		//スムーズに坂を下る為の吸着距離
		const float absDistance = 0.2f;
		//接地を維持
		if (CollisionManager::GetInstance()->Raycast(downRay, boxCollider, COLLISION_ATTR_LANDSHAPE,
			&downRayCastHit, boxCollider->GetScale().y * 2.0f + absDistance))
		{
			onGround = true;
			jump = false;
			secondJump = false;
			airSlide = false;
			position.y -= (downRayCastHit.distance - boxCollider->GetScale().y * 2.0f);
			//行列更新など
			Object::Update();
		}
		//地面がないので落下
		else {
			onGround = false;
			fallV = {};
		}
	}
	//落下状態
	else if (fallV.m128_f32[1] <= 0.0f)
	{
		if (CollisionManager::GetInstance()->Raycast(downRay, boxCollider, COLLISION_ATTR_LANDSHAPE,
			&downRayCastHit, boxCollider->GetScale().y * 2.0f))
		{
			//着地
			onGround = true;
			secondJump = false;
			jump = false;
			airSlide = false;
			position.y -= (downRayCastHit.distance - boxCollider->GetScale().y * 2.0f);
			//行列更新など
			Object::Update();
			if (fallV.m128_f32[1] <= fallVYMin +0.3f)
				ParticleEmitter::CreateJumpDust(position);

		}
	}
	class PlayerQueryCallBack :public QueryCallback
	{
	public:
		PlayerQueryCallBack(Box* box) :box(box) {};

		//衝突時のコールバック関数
		bool OnQueryHit(const QueryHit& info)override
		{
			box->center += info.reject;
			move += info.reject;
			collider = info.collider;
			//}
			return true;
		}
		//クエリーに使用するボックス
		Box* box = nullptr;
		//衝突相手のコライダー
		BaseCollider* collider = nullptr;
		//排斥による移動量(累積値)
		DirectX::XMVECTOR move = {};
	};

	//クエリーコールバックの関数オブジェクト
	PlayerQueryCallBack callback(boxCollider);
	//callback.OnQueryHit(info);
	//地形との交差を全検索
	CollisionManager::GetInstance()->QueryBox(*boxCollider, &callback, COLLISION_ATTR_LANDSHAPE);
	Vector3 rejectVec = callback.move;
	rejectVec.Normalize();
	if (airSlide && rejectVec.Length() != 0)
	{
		BoxCollider coll = *(BoxCollider*)collider;
		BoxCollider* preCollider = &coll;
		preCollider->SetOffset(preCollider->GetOffset() - airSlideVel.ConvertXMVECTOR());
		preCollider->Update();
		if (callback.collider->GetMax().x > preCollider->GetMax().x && callback.collider->GetMax().x < boxCollider->GetMax().x ||
			callback.collider->GetMax().x < preCollider->GetMax().x && callback.collider->GetMax().x > boxCollider->GetMax().x)
		{
			callback.move.m128_f32[0] = prePos.x - position.x;

		}
		else if (callback.collider->GetMin().x > preCollider->GetMin().x && callback.collider->GetMin().x < boxCollider->GetMin().x ||
			callback.collider->GetMin().x < preCollider->GetMin().x && callback.collider->GetMin().x > boxCollider->GetMin().x)
		{
			callback.move.m128_f32[0] = prePos.x - position.x;

		}
		if (callback.collider->GetMax().z > preCollider->GetMax().z && callback.collider->GetMax().z < boxCollider->GetMax().z ||
			callback.collider->GetMax().z < preCollider->GetMax().z && callback.collider->GetMax().z > boxCollider->GetMax().z)
		{
			callback.move.m128_f32[2] = prePos.z - position.z;

		}
		else if (callback.collider->GetMin().z > preCollider->GetMin().z && callback.collider->GetMin().z < boxCollider->GetMin().z ||
			callback.collider->GetMin().z < preCollider->GetMin().z && callback.collider->GetMin().z > boxCollider->GetMin().z)
		{
			callback.move.m128_f32[2] = prePos.z - position.z;
		}

	}
	//交差による排斥文を動かす
	position.x += callback.move.m128_f32[0];
	position.y += callback.move.m128_f32[1];
	position.z += callback.move.m128_f32[2];

	wallRejectVec = callback.move.m128_f32;

	wallRejectVec.y = 0;

	if (callback.move.m128_f32[1] < 0 && fallV.m128_f32[1]>0)
		fallV.m128_f32[1] = 0;

	Object::Update();

}

void Player::WallJump()
{
	const int WallJumpFrame = 20;//何フレームで終了するか
	if (!onGround && wallRejectVec.Length() != 0 && (Input::TriggerKey(DIK_SPACE) || Input::TriggerPadButton(SettingParam::GetJumpButton())))
	{
		wallRejectVec.Normalize();
		Vector3 BaseDir = { 0,0,1 };
		if (wallRejectVec.Dot({ 1,0,0 }) < 0)
			BaseDir = { 0,0,-1 };
		const float effectRotY = acosf(wallRejectVec.Dot(BaseDir));
		Vector3 effectPos = position - wallRejectVec * ((BoxCollider*)collider)->GetScale().z;
		ParticleEmitter::CreateShock(effectPos, { 0,effectRotY,0 });

		myModel->PlayAnimation("stand", false, 1, false);
		wallJump = true;
		wallJumpCounter = 0;
		wallJumpVel = wallRejectVec / 3.0f;
		wallJumAccel = wallJumpVel / WallJumpFrame;
		fallV = { 0,wallJumpVYFist,0,0 };
		//-----------------回転角度の計算----------------------------

		float cosA = direction.Dot(wallRejectVec);
		if (cosA > 1.0f)
			cosA = 1.0f;
		else if (cosA < -1.0f)
			cosA = -1.0f;
		float rotY = acos(cosA);
		const Vector3 CrossVec = direction.Cross(wallRejectVec);
		if (CrossVec.y < 0)
			rotY *= -1;
		//-----------------------------------------------------------
			//進行方向を壁ジャンプの方向に変更
		direction = wallRejectVec;
		//オブジェクトの向きを回転
		rotation.y += rotY * 180 / 3.1415926535f;//ライアン角から度数法に直す
	}
	if (!wallJump)
		return;
	if (wallJumpCounter >= WallJumpFrame)
	{
		wallJumpCounter = 0;
		wallJump = false;
		return;
	}

	wallJumpVel -= wallJumAccel;

	position += wallJumpVel;

	wallJumpCounter++;
}

void Player::AirSlide()
{
	const int AirSlideFrame = 20;//何フレームで終了するか
	if (!onGround  &&!airSlide&& (Input::TriggerKey(DIK_F) || Input::TriggerPadButton(SettingParam::GetAirSlideButton())))
	{
		Vector3 BaseDir = { 0,0,1 };
		if (direction.Dot({ 1,0,0 }) < 0)
			BaseDir = { 0,0,-1 };
		const float effectRotY = acosf(direction.Dot(BaseDir));
		Vector3 effectPos = position - direction * ((BoxCollider*)collider)->GetScale().z;
		ParticleEmitter::CreateShock(effectPos,{0,effectRotY,0});

		myModel->PlayAnimation("stand", false, 1, false);
		airSlide = true;
		airSlideCounter = AirSlideFrame;
		airSlideVel = direction / 3.0f;
		airSlideAccel = airSlideVel / AirSlideFrame;
		fallV = { 0,0,0,0 };
	}
	if (airSlideCounter <= 0)
		return;

	if (airSlideCounter >= 15)
	{
		Vector3 effectPos = position + direction *3 + Vector3{ 0,1,0 }*((BoxCollider*)collider)->GetScale().y / 2;
		ParticleEmitter::CreateAir(effectPos, direction);
	}
	airSlideVel -= airSlideAccel;

	position = prePos + airSlideVel;

	airSlideCounter--;

}

void Player::MoveCamera()
{
	XMMATRIX camMatWorld = XMMatrixInverse(nullptr, camera->GetMatView());
	const Vector3 cameraDirectionZ = Vector3(camMatWorld.r[2].m128_f32[0], 0, camMatWorld.r[2].m128_f32[2]).Normalize();

	//カメラのリセット処理
	if ((Input::TriggerKey(DIK_C) || Input::TriggerPadButton(SettingParam::GetResetButton())) && !rotCamera)
	{
		rotCamera = true;
		float cosA = direction.Dot(cameraDirectionZ);
		if (cosA > 1.0f)
			cosA = 1.0f;
		else if (cosA < -1.0f)
			cosA = -1.0f;
		radY = acos(cosA);
		const Vector3 CrossVec = direction.Cross(cameraDirectionZ);
		if (CrossVec.y < 0)
			radY *= -1;
		cameraRotCount = 0;
		//camera->AddPhi(radY);
	}
	
	//カメラの回転処理
	if (rotCamera)
	{
		cameraRotCount++;
		float rad = radY / RotTime;
		camera->AddPhi(rad);
		if (cameraRotCount >= RotTime)
			rotCamera = false;
	}
}

void Player::ResetPerform()
{
	//出現処理
	if (!appear)
	{
		//0-1に増加
		if (appearCounter < 60)
		{
			appearCounter++;
			sendData._Destruction = Easing::Lerp(1, 0, (float)appearCounter / 60.0f);
		}
		if (appearCounter == 60)
		{
			//出現完了
			appear = true;
			//やり直し処理終了
			reset = false;
		}
		return;
	}
	//消滅処理
	if (!resetMove)
	{
		//1-0に減少
		if (disappearCounter < 60)
		{
			disappearCounter++;
			sendData._Destruction = Easing::Lerp(0, 1, (float)disappearCounter / 60.0f);
		}
		//消滅しきったら
		if (disappearCounter == 60)
		{
			//初期値に移動させるフラグを立てる
			resetMove = true;
			//現在の位置を移動前の位置に設定
			resetStartPos =  position;
			//カメラのビュー行列の逆行列を計算
			XMMATRIX camMatWorld = XMMatrixInverse(nullptr, camera->GetMatView());
			//----------カメラの回転角度を算出---------------
			const Vector3 cameraDirectionZ = Vector3(camMatWorld.r[2].m128_f32[0], 0, camMatWorld.r[2].m128_f32[2]).Normalize();
			float cosA = Vector3(0,0,1).Dot(cameraDirectionZ);
			if (cosA > 1.0f)
				cosA = 1.0f;
			else if (cosA < -1.0f)
				cosA = -1.0f;
			radY = acos(cosA);
			const Vector3 CrossVec = Vector3(0, 0, 1).Cross(cameraDirectionZ);
			if (CrossVec.y < 0)
				radY *= -1;
			//-----------------------------------------------
			//カメラの現在の回転度合いを代入
			resetPhi = camera->GetPhi();
		}
		return;
	}
	//初期値への移動処理
	if (resetMoveCounter < 60)
	{
		resetMoveCounter++;
		//位置の更新
		position.x = Easing::EaseInOutQuart(resetStartPos.x, StartPos.x,60,resetMoveCounter);
		position.y = Easing::EaseInOutQuart(resetStartPos.y, StartPos.y, 60, resetMoveCounter);
		position.z = Easing::EaseInOutQuart(resetStartPos.z, StartPos.z, 60, resetMoveCounter);
		//カメラの回転
		const float phi = Easing::EaseInOutQuart(resetPhi, resetPhi + radY, 60, resetMoveCounter);
		camera->SetPhi(phi);
	}
	//移動しきったら初期化
	if (resetMoveCounter == 60)
	{
		Initialize();
	}

}
