#pragma once
#include <DirectXMath.h>
#include <d3d12.h>

#include"Scene.h"
#include<unordered_map>
#include <wrl/client.h>

class Sprite;
class TextureResource;

class SceneManager
{
public:
	static SceneManager* GetInstance();
	void Add(Scene::SCENE name,Scene* scene);
	void Initialize();
	void Update();
	void Change(Scene::SCENE next);
	void PreDraw();
	void PostDraw();
	void End();
private:

	SceneManager() = default;
	SceneManager(const SceneManager&) = delete;
	~SceneManager() = default;
	SceneManager& operator=(const SceneManager&) = delete;
	struct ConstBufferDate
	{
		float migrateTime;
		DirectX::XMFLOAT3 pad;
	};

	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff[3];//定数バッファ

	std::unordered_map<Scene::SCENE, Scene*>scenes;
	Scene* currentScene = nullptr;
	TextureResource* resource = nullptr;
	Sprite* migrateTex = nullptr;
	bool migrateStart;
	float migrateTime;
	int migrateCounter;
	;

};

