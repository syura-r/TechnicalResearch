#pragma once
#include "Scene.h"
#include"LightGroup.h"
#include"CollisionManager.h"
#include "ComputeShade.h"
#include "DebugCamera.h"
#include "Fluorescent.h"
#include "Goal.h"
#include"Grass.h"
#include "Menu.h"
#include "NumberSprite.h"
#include "ObjectManager.h"
#include "Player.h"
#include "Result.h"
#include "Sprite.h"
class Pencil;
class TouchAbleObject;
class Play :public Scene
{
public:
	Play();
	~Play()override;
	void Initialize()override;
	void Update()override;
	void PreDraw()override;
	void PostDraw()override;
private:
	void TimeUpdate();
	std::unique_ptr<LightGroup> lightGroup;
	XMFLOAT3 pos = { 1,0,0 };
	float coloramb[3] = { 1,1,1 };
	float color0[3] = { 1,1,1 };
	float color1[3] = { 1,1,1 };
	float color2[3] = { 1,1,1 };
	float lightDir[3] = {1.0f,-0.6f,0.0f};
	bool targetSphere = false;
	CollisionManager* collisionManager = nullptr;
	ObjectManager* objectManager = nullptr;
	std::unique_ptr<NumberSprite> secondTimeSprite;
	std::unique_ptr<NumberSprite> minuteTimeSprite;
	std::unique_ptr<Goal> goal;
	Player* player =nullptr;
	std::unique_ptr<Menu> menu;
	//ポーズ中
	bool pause = false;
	//ポーズボタンを押したフレーム（背景を別のリソースに描画させるフレーム）
	bool pauseFrame = false;
	std::unique_ptr<TextureResource> resource;
	std::unique_ptr<TextureResource> halfResource;
	std::unique_ptr<TextureResource> quarterResource;

	std::unique_ptr<Pencil> pencils[2];
	
	//ポーズ中の背景
	std::unique_ptr<Sprite> pauseBackTex;
	std::unique_ptr<Sprite> halfPauseBackTex;
	std::unique_ptr<Sprite> quarterPauseBackTex;
	std::unique_ptr<Sprite> colon;
	
	float minuteTime = 0;
	float secondTime = 0;
	int frameCounter = 0;;
	int getAchieve = 0;
	bool onResult = false;
	std::unique_ptr<Result> result;
	std::unique_ptr<DebugCamera> camera;
	float lightPos[9][3] =
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

	float LockerColliderOffset[16][3] =
	{
		33.547f, 0.805f, -9.810f,
		33.547f, 4.344f, -9.810f,
		33.547f, 7.514f, -9.810f,
		33.547f, 10.656f, -9.810f,
		33.547f, 5.556f, 18.218f,
		33.547f, 5.556f, 12.618f,
		33.547f, 5.556f, 7.018f,
		33.547f, 5.556f, 1.418f,
		33.547f, 5.556f, -4.182f,
		33.547f, 5.556f, -9.782f,
		33.547f, 5.556f, -15.382f,
		33.547f, 5.556f, -20.982f,
		33.547f, 5.556f, -26.582f,
		33.547f, 5.556f, -32.182f,
		33.547f, 5.556f, -37.782f,
		36.090f, 5.543f, -9.810f
	};
	float LockerColliderScale[16][3]
	{
		3.060f , 0.656f , 28.309f,
		3.060f , 0.253f , 28.309f,
		3.060f , 0.253f , 28.309f,
		3.060f , 0.253f , 28.309f,
		3.060f , 4.973f , 0.337f,
		3.060f , 4.973f , 0.337f,
		3.060f , 4.973f , 0.337f,
		3.060f , 4.973f , 0.337f,
		3.060f , 4.973f , 0.337f,
		3.060f , 4.973f , 0.337f,
		3.060f , 4.973f , 0.337f,
		3.060f , 4.973f , 0.337f,
		3.060f , 4.973f , 0.337f,
		3.060f , 4.973f , 0.337f,
		3.060f , 4.973f , 0.337f,
		0.254f , 5.379f , 28.309f
	};

	float FrontWallColliderOffset[2][3] =
	{
			-39.732f, 13.143f, 29.882f,
			-42.742f, 12.857f, -5.200f,
	};
	float FrontWallColliderScale[2][3] =
	{
		2.743f, 13.029f, 1.966f,
		1.114f, 12.514f, 35.5f,
	};

	float BlackBoardColliderOffset[7][3] =
	{
		-40.674f, 4.023f,  -5.194f,
		-40.480f, 3.450f,  -5.194f,
		-41.423f, 10.057f, -5.194f,
		-41.166f, 10.194f, 16.020f,
		-41.166f, 10.194f, -26.379f,
		-41.166f, 16.217f, -5.166f,
		-41.143f, 15.897f, -24.29f,
	};
	float BlackBoardColliderScale[7][3]
	{
		0.857f, 0.171f, 21.026f,
		0.600f, 0.429f, 2.051f,
		0.171f, 5.743f, 20.855f,
		0.600f, 6.343f, 0.171f,
		0.600f, 6.343f, 0.171f,
		0.600f, 0.343f, 21.368f,
		0.686f, 0.514f, 1.026f,
	};
	float BackWallColliderOffset[2][3]
	{
		34.661f , 13.143f, -40.686f,
		37.702f , 13.143f, -5.556f,

	};
	float BackWallColliderScale[2][3]
	{
		2.743f, 13.029f, 2.53f,
		1.371f, 12.943f, 35.5f,

	};
	float RightWallColliderOffset[7][3] =
	{
		-3.3000f, 10.674f, 32.388f,
		-24.686f, 8.2510f, 32.366f,
		-32.691f, 8.2420f, 32.675f,
		-36.725f, 10.534f, 32.378f,
		17.9020f, 8.2200f, 32.578f,
		26.3020f, 8.2070f, 32.424f,
		33.514f , 13.143f, 29.563f,

	};
	float RightWallColliderScale[7][3]
	{
		17.079f, 10.337f, 0.672f,
		4.2700f, 7.8650f, 0.560f,
		3.7250f, 7.9080f, 0.522f,
		0.3270f, 10.196f, 0.652f,
		4.1100f, 7.9080f, 0.457f,
		4.2480f, 7.9080f, 0.587f,
		2.943f, 13.029f, 2.393f,
	};
	float LeftWallColliderOffset[2][3] = {
		-3.3770f, 12.913f, -44.00f,
		-39.292f, 13.143f, -40.755f,

	};

	float LeftWallColliderScale[2][3] = {
		36.000f, 12.614f, 1.026f,
		2.486f, 13.029f, 2.222f,
	};


	
	float lightAtten[3] = { 1,1,1 };
};

