#pragma once
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
	Window* win = nullptr;
	DirectXLib* directX = nullptr;
	ComputeWrapper* computeWrapper = nullptr;
	//DebugCamera* camera = nullptr;
	Vector3 cameraPos;
	//PostEffect* postEffect = nullptr;
	LightCamera* lightCamera = nullptr;
	TextureResource* shadowMap = nullptr;
	SSAO* ssao = nullptr;
	SSAOCombine* ssaoCombine = nullptr;

	TextureResource* normalResource = nullptr;
	TextureResource* halfNormalResource = nullptr;
	TextureResource* mainResource = nullptr;
	TextureResource* depthResource = nullptr;
	TextureResource* halfDepthResource = nullptr;

	TextureResource* ssaoResource = nullptr;
	Sprite* depthTex = nullptr;
	Sprite* normalTex = nullptr;

	float dir[3] = { 1.0f,-0.60f,0.0f };

	float distance = 150;

	//ÉçÅ[ÉhíÜ
	bool nowLoading;

	Sprite* loadTex = nullptr;
	Sprite* loadDot = nullptr;

	int loadAssetLevel = 0;
	int createPipelineLevel = 0;

	bool loadAssetFinish = false;
	bool createPipelineFinish = false;
};

