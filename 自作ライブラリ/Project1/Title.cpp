#include "Title.h"

#include "DebugCamera.h"
#include "Floor.h"
#include "Fluorescent.h"
#include "PtrDelete.h"
#include "Sprite.h"
#include "TextureResource.h"
#include "Wall.h"

Title::Title()
{
	camera = new DebugCamera();
	next = SCENE::Play;
	lightGroup = LightGroup::Create();
	lights.clear();
	for (int i = 0; i < 9; i++)
	{
		lights.push_back(new Fluorescent(lightPos[i]));
		lightGroup->SetPointLightActive(i, true);
		lightGroup->SetPointLightPos(i, lightPos[i] + Vector3{ 0,3,0 });
	}
	Wall* ceiling = new Wall("ceiling");
	Wall* backWall = new Wall("backWall");
	Wall* leftWall = new Wall("leftWall");
	Wall* rightWall = new Wall("rightWall");
	Wall* frontWall = new Wall("frontWall");
	Wall* blackBoard = new Wall("blackBoard");
	blackBoard->SetPosition({ 0,-4.3f,0 });
	blackBoard->Update();
	Wall* locker = new Wall("locker");

	walls.push_back(ceiling);
	walls.push_back(backWall);
	walls.push_back(leftWall);
	walls.push_back(rightWall);
	walls.push_back(frontWall);
	walls.push_back(blackBoard);
	walls.push_back(locker);
	sky = new Sky();
	floor = new Floor();

	backTex = new Sprite();
	resource = new TextureResource("titleBackTex");
	titleText = new TitleText();
}


Title::~Title()
{
	for (auto it : walls)
	{
		PtrDelete(it);
	}
	for (auto it : lights)
	{
		PtrDelete(it);
	}

	PtrDelete(sky);
	PtrDelete(camera);
	PtrDelete(floor);
	PtrDelete(lightGroup);
	PtrDelete(backTex);
	PtrDelete(resource);
	PtrDelete(titleText);
}

void Title::Initialize()
{
	Object3D::SetCamera(camera);
	Object3D::SetLightGroup(lightGroup);
	camera->SetDistance(0.1f);
	camera->SetTarget({ 0,0,0 });
	isEnd = false;
	titleText->Initialize();
}

void Title::Update()
{
	if(titleText->Update())
	{
		if (titleText->GetStart())
		{
			ShutDown();
		}
		if (titleText->GetEnd())
		{
			
		}
	}
	lightGroup->SetAmbientColor(XMFLOAT3(1,1,1));
	lightGroup->Update();
	camera->AddPhi(0.0002f);
	camera->Update();
	for (auto it : walls)
	{
		it->Update();
	}
	for (auto it : lights)
	{
		it->Update();
	}

	sky->Update();
	floor->Update();
}

void Title::PreDraw()
{
	auto endit = walls.end();
	for (auto it : walls)
	{
		it->DrawReady();
	}
	for (auto it : lights)
	{
		it->DrawReady();
	}
	sky->DrawReady();
	floor->DrawReady();
	if (!Object3D::GetDrawShadow())
	{
		GSShaderTestObj::ChangeSendData();
		GSShaderTestObj::SendConstBuff();
		resource->PreDraw();
	}

	PipelineState::SetPipeline(walls[0]->GetPipelineName());
	for (auto it : walls)
	{
		it->Draw();
	}
	for (auto it : lights)
	{
		it->Draw();
	}

	PipelineState::SetPipeline(sky->GetPipelineName());
	sky->Draw();
	PipelineState::SetPipeline(floor->GetPipelineName());
	floor->Draw();
	if (!Object3D::GetDrawShadow())
	{
		resource->PostDraw();
		//DirectXLib::GetInstance()->ReRenderTarget();
		backTex->DrawSprite("titleBackTex" + std::to_string(DirectXLib::GetInstance()->GetBbIndex()), { 0,0 }, 0, { 1,1 }, { 1,1,1,1 }, { 0,0 },"FisheyeLens");
	}
}

void Title::PostDraw()
{
	if(!Object3D::GetDrawShadow())
	{
		titleText->Draw();
	}
}
