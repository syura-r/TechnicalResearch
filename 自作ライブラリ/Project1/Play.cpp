#include "Play.h"
#include <fstream>
#include <sstream>
#include"Object3D.h"
#include"Audio.h"
#include "Book.h"
#include"ParticleEmitter.h"
#include "DrawMode.h"
#include"imgui.h"
#include"TouchAbleObject.h"
#include"FBXManager.h"
#include "Input.h"
#include "MapBox.h"
#include "Chair.h"
#include "CleaningToolStorage.h"
#include "Desk.h"
#include "Easing.h"
#include "Floor.h"
#include "Fluorescent.h"
#include "LevelEditor.h"
#include "Pencil.h"
#include "Player.h"
#include "SetSquare30.h"
#include "SetSquare45.h"
#include "Sky.h"
#include "SSAO.h"
#include "TeacherDesk.h"
#include "Wall.h"

Play::Play()
{
	next = Ending;
	camera = std::make_unique<DebugCamera>();
	Object3D::SetCamera(camera.get());
	ParticleEmitter::Initialize(camera.get());
	Player::SetDebugCamera(camera.get());
	SSAO::SetCamera(camera.get());
	//���C�g����
	lightGroup.reset(LightGroup::Create());
	GSShaderTestObj::CreateConstBuff();
	//3D�I�u�W�F�N�g�Ƀ��C�g���Z�b�g
	Object3D::SetLightGroup(lightGroup.get());
	//���C�g�F��ݒ�
	lightGroup->SetDirLightActive(0, false);
	lightGroup->SetDirLightColor(0, XMFLOAT3(color0));
	menu = std::make_unique<Menu>();
	collisionManager = CollisionManager::GetInstance();
	objectManager = ObjectManager::GetInstance();
	player = new Player();
	objectManager->Add(player);
	objectManager->Add(new Floor());
	secondTimeSprite = std::make_unique<NumberSprite>(secondTime);
	minuteTimeSprite = std::make_unique<NumberSprite>(minuteTime);

	Wall* ceiling = new Wall("ceiling");
	Wall* backWall = new Wall("backWall");
	for (int i = 0; i < 2; i++)
	{
		backWall->AddCollider(BackWallColliderOffset[i].data(), BackWallColliderScale[i].data());
	}
	Wall* leftWall = new Wall("leftWall");
	for (int i = 0; i < 2; i++)
	{
		leftWall->AddCollider(LeftWallColliderOffset[i].data(), LeftWallColliderScale[i].data());
	}
	Wall* rightWall = new Wall("rightWall");
	for (int i = 0; i < 7; i++)
	{
		rightWall->AddCollider(RightWallColliderOffset[i].data(), RightWallColliderScale[i].data());
	}
	Wall* frontWall = new Wall("frontWall");
	for (int i = 0; i < 2; i++)
	{
		frontWall->AddCollider(FrontWallColliderOffset[i].data(), FrontWallColliderScale[i].data());
	}
	Wall* blackBoard = new Wall("blackBoard");
	blackBoard->SetPosition({ 0,-4.3f,0 });
	blackBoard->Update();
	for (int i = 0; i < 7; i++)
	{
		blackBoard->AddCollider(BlackBoardColliderOffset[i].data(), BlackBoardColliderScale[i].data());
	}
	Wall* locker = new Wall("locker");
	for (int i = 0; i < 16; i++)
	{
		locker->AddCollider(LockerColliderOffset[i].data(), LockerColliderScale[i].data());
	}
	objectManager->Add(ceiling);
	objectManager->Add(backWall);
	objectManager->Add(leftWall);
	objectManager->Add(rightWall);
	objectManager->Add(frontWall);
	objectManager->Add(blackBoard);
	objectManager->Add(locker);
	objectManager->Add(new CleaningToolStorage());

	for (int i = 0; i < 9; i++)
	{
		Fluorescent* light = nullptr;
		light = new Fluorescent(lightPos[i].data());
		objectManager->Add(light);
		lightGroup->SetPointLightActive(i, true);
		lightGroup->SetPointLightPos(i, lightPos[i].data() - Vector3{0,3,0});
	}
	objectManager->Add(new Sky());
	goal =  std::make_unique<Goal>();
	pauseBackTex = std::make_unique<Sprite>();
	halfPauseBackTex = std::make_unique<Sprite>();
	quarterPauseBackTex = std::make_unique<Sprite>();
	colon = std::make_unique<Sprite>();
	resource.reset(new TextureResource("PauseBack",{1920,1080}, DXGI_FORMAT_R8G8B8A8_UNORM,{1,1,1,1},false));
	halfResource.reset(new TextureResource("halfPauseBack",{960,540}, DXGI_FORMAT_R8G8B8A8_UNORM, { 1,1,1,1 }, false));
	quarterResource.reset(new TextureResource("quarterPauseBack",{480,270}, DXGI_FORMAT_R8G8B8A8_UNORM, { 1,1,1,1 }, false));

	result = std::make_unique<Result>();
	pencils[0].reset(new Pencil({ -5.31f,0.28f ,29.09f }));
	pencils[1].reset(new Pencil({ 22.79f,15.0f,-35.52f }));

//---------------------------------������------------------------------------------
	std::string filepath = "Resources/Map/Report" + std::to_string(0) + ".txt";
	std::ifstream file;
	file.open(filepath.c_str());
	if (!file.is_open())
		return;
	std::string line;
	std::vector<Vector3>positions;
	std::vector<Vector3>scales;
	std::vector<Vector3>rotations;
	std::vector<Object*>loadObjects;

	while (getline(file, line))
	{
		//1�s���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
		std::istringstream line_stream(line);

		//���p�X�y�[�X��؂�ōs�̐擪�������擾
		std::string key;
		getline(line_stream, key, ' ');

		//�N���X��
		if (key == "class")
		{
			std::string name;
			line_stream >> name;
			if (name == "Chair")
			{
				Object* ptr = new Chair();
				loadObjects.push_back(ptr);
			}
			else if (name == "Desk")
			{
				Object* ptr = new Desk();
				loadObjects.push_back(ptr);
			}
			else if (name == "TeacherDesk")
			{
				Object* ptr = new TeacherDesk();
				loadObjects.push_back(ptr);
			}
			else if (name == "SetSquare30")
			{
				Object* ptr = new SetSquare30();
				loadObjects.push_back(ptr);
			}
			else if (name == "SetSquare45")
			{
				Object* ptr = new SetSquare45();
				loadObjects.push_back(ptr);
			}
			else if (name == "Book")
			{
				Object* ptr = new Book();
				loadObjects.push_back(ptr);
			}

		}
		//�|�W�V����
		if (key == "position")
		{
			Vector3 position;
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;

			positions.push_back(position);
		}
		//�X�P�[��
		if (key == "scale")
		{
			Vector3 scale;
			line_stream >> scale.x;
			line_stream >> scale.y;
			line_stream >> scale.z;

			scales.push_back(scale);
		}
		//��]
		if (key == "rotation")
		{
			Vector3 rotation;
			line_stream >> rotation.x;
			line_stream >> rotation.y;
			line_stream >> rotation.z;

			rotations.push_back(rotation);
		}
	}
	file.close();

	int i = 0;
	for (auto& it : loadObjects) {
		it->SetPosition(positions[i]);
		it->SetScale(scales[i]);
		it->SetRotation(rotations[i]);
		ObjectManager::GetInstance()->Add(it);
		i++;
	}
//---------------------------------------------------------------------------
}


Play::~Play()
{
	LevelEditor::GetInstance()->Clear();
}

void Play::Initialize()
{
	Object3D::SetCamera(camera.get());
	Object3D::SetLightGroup(lightGroup.get());
	for (int i = 0; i < 2; i++)
	{
		pencils[i]->Initialize();
	}
	goal->Initialize();
	objectManager->Initialize();
	isEnd = false;
	pause = false;
	pauseFrame = false;
	minuteTime = 0;
	secondTime = 0;
	onResult = false;
	getAchieve = 0;
}

void Play::Update()
{
	if(onResult)
	{
		if(result->Update())
		{
			if(result->GetRetry())
			{
				next = SCENE::Play;
			}
			if (result->GetReturnTitle())
			{
				next = SCENE::Title;
			}
			isEnd = true;
		}
		return;
	}
	if (pause)
	{
		if (menu->Update())
		{
			if (menu->GetRestart())
			{
				player->Reset();
				frameCounter = 0;
				secondTime = 0;
				minuteTime = 0;
			}
			if(menu->GetReturnTitle())
			{
				next = SCENE::Title;
				isEnd = true;
			}
			pause = false;
		}
		return;
	}
	camera->Update();
	if (Input::TriggerKey(DIK_ESCAPE) || Input::TriggerPadButton(XINPUT_GAMEPAD_START))
	{
		menu->Initialize();
		pause = true;
		pauseFrame = true;
		return;
	}

	lightGroup->SetAmbientColor(XMFLOAT3(coloramb));
	lightGroup->SetDirLightDir(0, { lightDir[0],lightDir[1],lightDir[2],1 });
	lightGroup->Update();
	objectManager->Update();
	goal->Update();
	if (goal->GetGoal())
	{
		result->Initialize(secondTime, minuteTime, getAchieve);
		result->Update();
		onResult = true;
		pauseFrame = true;
		return;
	}
	for (int i = 0; i < 2; i++)
	{
		pencils[i]->Update();
		if (pencils[i]->GetGet())
			getAchieve++;
	}
	collisionManager->CheckAllCollisions();
	if (!player->GetReset())
		TimeUpdate();
}

void Play::PreDraw()
{
	if (pauseFrame && !Object3D::GetDrawShadow())
		resource->PreDraw();
	if ((!pause && !onResult) || pauseFrame)
	{
		objectManager->DrawReady();
#ifdef _DEBUG
		if (DrawMode::GetDrawImGui() && Object3D::GetDrawShadow())
		{
			ImGui::Begin("Light");
			ImGui::SliderFloat3("LightDir", lightDir, -1.0f, 1.0f);

			ImGui::End();
			Object3D::GetLightCamera()->SetLightDir({ lightDir[0],lightDir[1] ,lightDir[2] });
			LevelEditor::GetInstance()->Draw();
		}
#endif
		if (!Object3D::GetDrawShadow())
		{
			GSShaderTestObj::ChangeSendData();
			GSShaderTestObj::SendConstBuff();
		}
		objectManager->PreDraw();
		if (!pauseFrame)
			goal->Draw();
		if (!Object3D::GetDrawShadow())
		{
			secondTimeSprite->Draw(2, "number", { 1650,80 }, { 1,1 }, { 1,1,1,1 }, { 0,0.5f });
			int digits = 1;
			if (minuteTime >= 10) digits = 2;
			minuteTimeSprite->Draw(digits, "number", { 1550,80 }, { 1,1 }, { 1,1,1,1 }, { 1,0.5f });
			colon->DrawSprite("colon", { 1600,80 });
			for (int i = 0; i < 2; i++)
			{
				pencils[i]->Draw(pauseFrame);
			}
		}
	}
}

void Play::PostDraw()
{
	//if (migrate)
	//	return;
	if (!pause || pauseFrame)
		objectManager->PostDraw();
	if (pauseFrame && !Object3D::GetDrawShadow())
	{
		resource->PostDraw(false);
		
		halfResource->PreDraw(1,0, 0, 960, 540);
		pauseBackTex->DrawSprite("PauseBack", { 0,0 }, 0, { 1,1 }, { 1,1,1,1 }, { 0,0 }, "NoAlphaToCoverageSprite");
		halfResource->PostDraw(false);
		
		quarterResource->PreDraw(1,0, 0, 960, 540);
		halfPauseBackTex->DrawSprite("halfPauseBack", { 0,0 }, 0, { 1,1 }, { 1,1,1,1 }, { 0,0 },"Blur");
		quarterResource->PostDraw(false);

		halfResource->PreDraw();
		quarterPauseBackTex->DrawSprite("quarterPauseBack", { 0,0 }, 0, {2,2 }, { 1,1,1,1 }, { 0,0 }, "Blur");
		halfResource->PostDraw(false);

		resource->PreDraw(1,0,0,1920*2,1080*2);
		halfPauseBackTex->DrawSprite("halfPauseBack", { 0,0 }, 0, { 1,1 }, { 1,1,1,1 }, { 0,0 }, "NoAlphaToCoverageSprite");
		resource->PostDraw();
		
		pauseFrame = false;
	}
	if ((pause || onResult) && !Object3D::GetDrawShadow())
	{
		pauseBackTex->DrawSprite("PauseBack", { 0,0 }, 0, { 1,1 }, { 1,1,1,1 }, { 0,0 }, "NoAlphaToCoverageSprite");
		DirectXLib::GetInstance()->DepthClear();
	}
	if (pause && !Object3D::GetDrawShadow() && !isEnd)
	{
		menu->Draw();
	}
	if (onResult && !Object3D::GetDrawShadow() && !isEnd)
	{
		result->Draw();
	}

}

void Play::TimeUpdate()
{
	if(Input::TriggerKey(DIK_P))
	{
		minuteTime += 1.0f;
	}
	frameCounter++;
	if(frameCounter >= 60)
	{
		frameCounter = 0;
		secondTime += 0.1f;
		if(secondTime>=6)
		{
			secondTime = 0;
			minuteTime+=1.0f;
		}
	}
}
