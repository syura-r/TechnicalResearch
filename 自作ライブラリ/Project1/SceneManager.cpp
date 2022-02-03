#include "SceneManager.h"

#include "Easing.h"
#include "Object3D.h"
#include "PtrDelete.h"
#include "Sprite.h"
#include "TextureResource.h"


SceneManager* SceneManager::GetInstance()
{
	static SceneManager instance;
	//instance.Initialize();
	return &instance;
}

void SceneManager::Add(Scene::SCENE name, Scene * scene)
{
	if (scenes[name] != nullptr)
	{
		return;
	}
	scenes[name] = scene;
}

void SceneManager::Initialize()
{
	resource = new TextureResource("migrateTex",{1920,1080}, DXGI_FORMAT_R8G8B8A8_UNORM,{1,1,1,1},false);
	migrateTex = new Sprite();
	migrateStart = false;
	for (int i = 0; i < 3; i++)
	{
		//�萔�o�b�t�@�̍쐬
		auto result = DirectXLib::GetInstance()->GetDevice()->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDate) + 0xff) & ~0xff),
			D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
			IID_PPV_ARGS(&constBuff[i]));
		assert(SUCCEEDED(result));
	}
	migrateTime = 1.0f;
	migrateCounter = 0;
}

void SceneManager::Update()
{
	if (currentScene == nullptr)
	{
		return;
	}
	if(migrateStart)
	{
		if (migrateCounter < 60)
			migrateTime = Easing::EaseInOutQuad(1, 0, 60, migrateCounter);
		else
			migrateTime = Easing::EaseInOutQuad(0, 1, 60, migrateCounter-60);
		migrateCounter++;
		if (migrateCounter == 60)
		{
			Change(currentScene->NextScene());
			currentScene->Update();
		}
		if(migrateCounter == 120)
		{
			migrateStart = false;
			migrateCounter = 0;
		}
		return;
	}
	if (currentScene->GetIsEnd())
	{
		migrateStart = true;
	}
	currentScene->Update();
}

void SceneManager::Change(Scene::SCENE name)
{
	currentScene = scenes[name];
	currentScene->Initialize();
}

void SceneManager::PreDraw()
{
	if (currentScene == nullptr)
	{
		return;
	}
	if (((!migrateStart && currentScene->GetIsEnd()) || migrateCounter == 60) && !Object3D::GetDrawShadow())
		resource->PreDraw();
	if (!migrateStart || migrateCounter == 60)
		currentScene->PreDraw();
}

void SceneManager::PostDraw()
{
	if (currentScene == nullptr)
	{
		return;
	}
	if (!migrateStart || migrateCounter == 60)
		currentScene->PostDraw();
	if (((!migrateStart && currentScene->GetIsEnd()) || migrateCounter == 60) && !Object3D::GetDrawShadow())
	{
		resource->PostDraw();
	}
	if(migrateStart|| currentScene->GetIsEnd() && !Object3D::GetDrawShadow())
	{
		auto libPtr = DirectXLib::GetInstance();
		auto bbIndex = libPtr->GetBbIndex();
		ConstBufferDate* constMap = nullptr;
		auto result = constBuff[bbIndex]->Map(0, nullptr, (void**)&constMap);
		constMap->migrateTime = migrateTime;
		constBuff[bbIndex]->Unmap(0, nullptr);

		PipelineState::SetPipeline("Migrate");
		libPtr->GetCommandList()->SetGraphicsRootConstantBufferView(1, constBuff[bbIndex]->GetGPUVirtualAddress());
		migrateTex->NoPipelineDraw("migrateTex", { 960,540 }, 0, { 1,1 }, { 1,1,1,1 }, { 0.5f, 0.5f });

	}
}

void SceneManager::End()
{
	for (auto itr : scenes)
	{
		delete itr.second;
		itr.second = nullptr;
	}

	PtrDelete(resource);
	PtrDelete(migrateTex);

}
