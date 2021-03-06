#pragma once
#include <array>

#include "BoxCollider.h"
#include "DebugCamera.h"
#include "FBXModel.h"
#include "Object.h"
class Player :
	public Object
{
public:
	Player();
	void Initialize()override;
	void Update()override;
	void Draw() override;
	void DrawReady() override;
	static void SetDebugCamera(DebugCamera* cameraPtr) { camera = cameraPtr; }
	void Reset();
	bool GetReset() { return reset; }
private:
	struct ConstBuffData
	{
		float _Destruction; //分解度合い
		float _ScaleFactor; //スケールの変化量
		float _PositionFactor; //ポジションの変化量
		float _RotationFactor; //回転の変化量
		int _Tessellation;//ポリゴン分割度
		int _OnEasing;//イージングで分解するか
		XMFLOAT2 pad;
	};
	struct ConstLightCameraBuff
	{
		XMMATRIX viewProjection;
		XMFLOAT3 cameraPos;
	};

	std::array<ComPtr<ID3D12Resource>, 3> constBuff; // 定数バッファ
	ConstBuffData sendData;
	ComPtr<ID3D12Resource> constCameraBuff; // 定数バッファ

	const unsigned short WALK = 0b1 << 0;
	const unsigned short RUN = 0b1 << 1;
	const unsigned short JUMP = 0b1 << 2;
	const unsigned short SECONDJUMP = 0b1 << 3;
	const unsigned short AIRSLIDE = 0b1 << 4;

	unsigned short attribute = 0b0;

	const float val = 0.4f;
	
	const float secondJumpVYFist = 0.4f*val;//二段ジャンプ時上向き初速
	const float jumpVYFist = 0.5f * val;//ジャンプ時上向き初速
	const float wallJumpVYFist = 0.43f * val;//壁ジャンプ時上向き初速

	//下向き加速
	const float fallAcc = -0.02f * val;
	const float fallVYMin = -0.5f;

	int walkDustCounter = 0;
	
	//初期位置
	const Vector3 StartPos = { 23,2,-20 };

	//移動処理
	void Move();
	//当たり判定
	void CheckHit();
	//壁ジャンプ処理
	void WallJump();
	//エアスライド処理
	void AirSlide();
	//カメラの制御
	void MoveCamera();
	//出現消滅時の演出処理
	void ResetPerform();
	//壁ジャンプフラグ
	bool wallJump = false;
	int wallJumpCounter;
	//壁ジャンプ用
	Vector3 wallRejectVec = {};
	Vector3 wallJumpVel;
	Vector3 wallJumAccel;
	//エアスライドフラグ
	bool airSlide = false;
	int airSlideCounter;
	Vector3 airSlideVel;
	Vector3 airSlideAccel;
	//接地フラグ
	bool onGround = true;
	//ジャンプフラグ
	bool jump = false;
	//二段ジャンプフラグ
	bool secondJump = false;
	//落下ベクトル
	XMVECTOR fallV;
	//現在向いてる方向
	Vector3 direction;
	//移動速度
	float speed = 0.09f;
	const float walkSpeed = 0.09f;
	const float runSpeed = 0.125f;
	//走りフラグ
	bool run = false;
	//回転速度
	float rotateSpeed = 7.5f;
	Vector3 prePos;
	//カメラ回転中
	bool rotCamera;
	//カメラの回転度合い
	float radY;
	int cameraRotCount;
	const int RotTime = 10;

	bool reset = true;
	bool appear;
	bool resetMove;
	int appearCounter;//出現時の演出用カウンター
	int disappearCounter;//消滅時の演出用カウンター
	int resetMoveCounter;
	Vector3 resetStartPos;
	float resetPhi;
	
	//BoxCollider* boxCollider;

	FBXModel* myModel = nullptr;
private://静的メンバ変数
	static DebugCamera* camera;

};

