#pragma once
#include "Object.h"
class GSShaderTestObj :
    public Object
{
public:
	//GSShaderTestObj(const Vector3& pos);
	virtual void Draw() override;
	void DrawReady() override;
	static void CreateConstBuff();
	static void SendConstBuff();
	static void ChangeSendData();
protected:
	struct ConstBuffData
	{
		//XMFLOAT4 status;
		float _Destruction; //分解度合い
		float _ScaleFactor; //スケールの変化量
		float _PositionFactor;//ポジションの変化量
		float _RotationFactor;//回転の変化量
		int _Tessellation;//ポリゴン分割度
		int _OnEasing;//イージングするかどうか
		int _OnTargetBreak; //特定の位置のみ分解するかどうか
		int _OnCameraBreak;
		XMFLOAT3 BreakPoint; //分解中心点
		float BreakRadius; //分解半径
		XMFLOAT3 TargetPos;//カメラの注視点
		float CapsuleRadius;//カメラから注視点までのカプセル半径
		int _On4Vertex;
	};
	struct ConstLightCameraBuff
	{
		XMMATRIX viewProjection;
		XMFLOAT3 cameraPos;
	};

	static ComPtr<ID3D12Resource> constBuff[3]; // 定数バッファ
	static ConstBuffData sendData;
	static ComPtr<ID3D12Resource> constCameraBuff[3]; // 定数バッファ

	//float destruction;
	//float scaleFactor; //スケールの変化量
	//float positionFactor;//ポジションの変化量
	//float rotationFactor;//回転の変化量
	//int  tessellation;//ポリゴン分割度
	//bool onEasing;//イージングするかどうか
	//bool onTargetBreak; //特定の位置のみ分解するかどうか
	//Vector3 breakPoint; //分解中心点
	//float breakRadius; //分解半径
	//bool onCameraBreak; //特定の位置のみ分解するかどうか
	//Vector3 targetPos; //分解中心点
	//float capsuleRadius; //分解半径
	//bool on4Vertex; //特定の位置のみ分解するかどうか

	//bool drawBreakPointObj; //分解球を描画するか
	//bool draw;
	//
	//Object* breakPointObj = nullptr;
};

