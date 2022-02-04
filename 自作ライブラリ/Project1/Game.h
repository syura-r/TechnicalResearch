#pragma once
#include <memory>

#include"Vector.h"
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxguid.lib")

class Window;
class DirectXLib;
class Game;
class ComputeWrapper;
class LightCamera;
class SceneManager;
class Sprite;
class SSAO;
class SSAOCombine;
class TextureResource;
class Vector;
class DirectXLib;
class Game
{
public:
	static Game* GetInstance();
	~Game();
	void Initialize();
	void Run();
	void End();
private:
	Game();
	static Game* instance;
	void RoadAsset();
	void CreatePipeline();
	void LoadFinish();
	void DrawLoadTex();
	SceneManager* sceneManeger = nullptr;
	std::unique_ptr<Window> win;
	DirectXLib* directX = nullptr;
	ComputeWrapper* computeWrapper = nullptr;
	//DebugCamera* camera = nullptr;
	Vector3 cameraPos;
	//Bloom* postEffect = nullptr;
	std::unique_ptr <LightCamera> lightCamera = nullptr;
	std::unique_ptr <TextureResource> shadowMap = nullptr;
	std::unique_ptr <SSAO> ssao = nullptr;
	std::unique_ptr <SSAOCombine> ssaoCombine = nullptr;

	std::unique_ptr < TextureResource> normalResource = nullptr;
	std::unique_ptr <TextureResource> halfNormalResource = nullptr;
	std::unique_ptr <TextureResource> mainResource = nullptr;
	std::unique_ptr <TextureResource> depthResource = nullptr;
	std::unique_ptr <TextureResource> halfDepthResource = nullptr;

	std::unique_ptr < TextureResource> ssaoResource = nullptr;
	std::unique_ptr < Sprite> depthTex = nullptr;
	std::unique_ptr < Sprite> normalTex = nullptr;

	float dir[3] = { 1.0f,-0.60f,0.0f };

	float distance = 150;

	//ÉçÅ[ÉhíÜ
	bool nowLoading;

	std::unique_ptr < Sprite> loadTex = nullptr;
	std::unique_ptr < Sprite> loadDot = nullptr;

	int loadAssetLevel = 0;
	int createPipelineLevel = 0;

	bool loadAssetFinish = false;
	bool createPipelineFinish = false;
};

