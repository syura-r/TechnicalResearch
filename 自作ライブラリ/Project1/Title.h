#pragma once
#include "Scene.h"
#include"CreateFigure.h"
#include"Texture.h"
#include"Alpha.h"
#include "Object.h"
#include "Sky.h"
#include "TitleText.h"
class DebugCamera;
class Floor;
class Fluorescent;
class Sprite;
class TextureResource;

class Title :
	public Scene
{
public:
	Title();
	~Title()override;
	void Initialize() override;
	void Update()override;
	void PreDraw()override;
	void PostDraw()override;
private:
	const float lightPos[9][3] =
	{
		-6.000f,19.404f,2.137f,
		-6.000f,19.404f,30.627f,
		-6.000f,19.404f,-25.783f,
		18.000f,19.404f,2.137f,
		18.000f,19.404f,30.627f,
		18.000f,19.404f,-25.783f,
	   -26.571f,19.404f,2.137f,
	   -26.571f,19.404f,30.627f,
	   -26.571f,19.404f,-25.783f,
	};

	DebugCamera* camera = nullptr;
	Floor* floor = nullptr;
	std::vector<Object*>walls;
	Sky* sky = nullptr;
	LightGroup* lightGroup = nullptr;
	std::vector<Object*> lights;
	Sprite* backTex = nullptr;
	TextureResource* resource = nullptr;
	TitleText* titleText = nullptr;
};
