#include "Title.h"

#include "DebugCamera.h"
#include "Floor.h"
#include "Fluorescent.h"
#include "Sprite.h"
#include "TextureResource.h"
#include "Wall.h"

Title::Title()
{
	camera = std::make_unique<DebugCamera>();
	next = SCENE::Play;
	lightGroup.reset(LightGroup::Create());
	lights.clear();
	for (int i = 0; i < 9; i++)
	{
		lights.push_back(std::make_unique<Fluorescent>(lightPos[i]));
		lightGroup->SetPointLightActive(i, true);
		lightGroup->SetPointLightPos(i, lightPos[i] + Vector3{ 0,3,0 });
	}
	std::unique_ptr<Wall>blackBoard = std::make_unique<Wall>("blackBoard");
	blackBoard->SetPosition({ 0,-4.3f,0 });
	blackBoard->Update();
	walls.push_back(std::make_unique<Wall>("ceiling"));
	walls.push_back(std::make_unique<Wall>("backWall"));
	walls.push_back(std::make_unique<Wall>("leftWall"));
	walls.push_back(std::make_unique<Wall>("rightWall"));
	walls.push_back(std::make_unique<Wall>("frontWall"));
	walls.push_back(std::move(blackBoard));
	walls.push_back(std::make_unique<Wall>("locker"));
	sky = std::make_unique<Sky>();
	floor = std::make_unique<Floor>();

	backTex = std::make_unique<Sprite>();
	resource = std::make_unique<TextureResource>("titleBackTex");
	titleText = std::make_unique<TitleText>();
}


Title::~Title()
{
}

void Title::Initialize()
{
	Object3D::SetCamera(camera.get());
	Object3D::SetLightGroup(lightGroup.get());
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
	for (auto& it : walls)
	{
		it->Update();
	}
	for (auto& it : lights)
	{
		it->Update();
	}

	sky->Update();
	floor->Update();
}

void Title::PreDraw()
{
	auto endit = walls.end();
	for (auto& it : walls)
	{
		it->DrawReady();
	}
	for (auto& it : lights)
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
	for (auto& it : walls)
	{
		it->Draw();
	}
	for (auto& it : lights)
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
