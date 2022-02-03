#pragma once
#include "BoxCollider.h"
#include "Object.h"
class Pencil :
    public Object
{
public:
	Pencil(const Vector3& pos);
	void Initialize() override;
	void Update() override;
	void Draw(const bool pause);
	bool GetGet() { return getFlame; }
private:
	bool get = false;
	bool getFlame = false;

	std::unique_ptr<Object> pencil;

	float BasePencilPosY;
	int pencilMoveCounter;
	Vector3 pencilPosition;
	Vector3 pencilRotation;

	const int PencilEasingTime = 180;
	
	struct ConstBuffDeta
	{
		float offsetTime;
		XMFLOAT3 pad;
	};
	float offsetTime;
	ComPtr<ID3D12Resource> constBuff[3]; // 定数バッファ
	std::unique_ptr <BoxCollider> colliders[3];

};

