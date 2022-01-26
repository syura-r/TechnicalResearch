#include "Pencil.h"

#include "BoxCollider.h"
#include "CollisionAttribute.h"
#include "CollisionManager.h"
#include "Easing.h"
#include "OBJLoader.h"
#include "ParticleEmitter.h"
#include "PtrDelete.h"

Pencil::Pencil(const Vector3& pos)
{
	//エフェクト部分の作成
	Create(OBJLoader::GetModel("Area"));

	position = pos;
	scale = { 0.5f,0.6f,0.5f };
	color = { 0.3f,0.9f,0.1f,1 };
	//鉛筆の作成
	pencil = new Object();
	pencil->Create(OBJLoader::GetModel("pencil"));
	pencilPosition = pos + Vector3{ 0,0.7f,0 };
	BasePencilPosY = pos.y + 0.8f;
	pencilMoveCounter = 30;
	pencilRotation = { 0,0,20 };
	pencil->SetPosition(pencilPosition);
	pencil->SetRotation(pencilRotation);
	pencil->SetScale({ 3,2.3f,3 });
	HRESULT result;
	for (int i = 0; i < 3; i++)
	{
		result = DirectXLib::GetInstance()->GetDevice()->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBuffDeta) + 0xff) & ~0xff),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constBuff[i]));
		assert(SUCCEEDED(result));
	}
	offsetTime = 0;
	object->Update();
	for (int i = 0; i < 3; i++)
	{
		colliders[i] = new BoxCollider({ 0, 0.65f, 0,0 }, Vector3{ 0.25f,0.65f,0.25f });
		colliders[i]->SetRotation({ 0,30.0f * i,0 });
		colliders[i]->SetObject(this);
		colliders[i]->SetAttribute(COLLISION_ATTR_ITEM);
		colliders[i]->Update();
		CollisionManager::GetInstance()->AddCollider(colliders[i]);

	}

}

Pencil::~Pencil()
{
	PtrDelete(pencil);
	Object::~Object();
}
void Pencil::Initialize()
{
	pencilMoveCounter = 30;
	get = false;
	getFlame = false;
}

void Pencil::Update()
{
	//獲得状態なら早期リターン
	if (get)
	{
		getFlame = false;
		return;
	}
	//エフェクトのUVスクロールの更新
	offsetTime += 0.01f;
	if (offsetTime > 1.0f)
		offsetTime = 0;

	//鉛筆の回転
	pencilRotation.y += 0.3f;
	//鉛筆上昇処理
	if (pencilMoveCounter <= PencilEasingTime)
		pencilPosition.y = Easing::EaseInOutQuad(BasePencilPosY - 0.2f, BasePencilPosY + 0.2f, PencilEasingTime, pencilMoveCounter);
	//鉛筆降下処理
	else if (pencilMoveCounter <= PencilEasingTime*2)
		pencilPosition.y = Easing::EaseInOutQuad(BasePencilPosY + 0.2f, BasePencilPosY - 0.2f, PencilEasingTime, pencilMoveCounter - PencilEasingTime);
	pencilMoveCounter++;
	if (pencilMoveCounter > PencilEasingTime*2)
		pencilMoveCounter = 0;

	//鉛筆の更新
	pencil->SetPosition(pencilPosition);
	pencil->SetRotation(pencilRotation);
	pencil->Update();
	Object::Update();
	//プレイヤーとのヒットチェック
	for (int i = 0; i < 3; i++)
	{
		colliders[i]->Update();
		//当たっていたら
		if (CollisionManager::GetInstance()->CheckHitBox(*colliders[i], COLLISION_ATTR_ALLIES))
		{
			//獲得エフェクトを出現させる
			ParticleEmitter::CreateGetEffect(pencilPosition);
			//獲得フラグ、獲得フレームフラグを立てる
			get = true;
			getFlame = true;
		}
	}

}

void Pencil::Draw(const bool pause)
{
	//獲得後は描画しない
	if (get)
		return;
	//影はない
	if (Object3D::GetDrawShadow())
		return;

	//パイプラインを設定
	PipelineState::SetPipeline("DrawShadowOBJ");
	//鉛筆の描画
	pencil->Draw();

	//ポーズ状態ならエフェクトを出さない
	if(pause) return;

	//エフェクトのUVスクロール用定数バッファの更新
	ConstBuffDeta* constMap = nullptr;
	auto result = constBuff[bbIndex]->Map(0, nullptr, (void**)&constMap);
	assert(SUCCEEDED(result));
	constMap->offsetTime = offsetTime;
	constBuff[bbIndex]->Unmap(0, nullptr);

	//パイプラインを設定
	PipelineState::SetPipeline("AreaEffect", ADD);
	//定数バッファをセット
	DirectXLib::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(3, constBuff[bbIndex]->GetGPUVirtualAddress());
	//ライティング無し加算ブレンドで描画
	CustomDraw(false, false, ADD, true);
}
