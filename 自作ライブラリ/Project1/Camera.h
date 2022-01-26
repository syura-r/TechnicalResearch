#pragma once
#include <DirectXMath.h>
#include <d3d12.h>

#include "Vector.h"
using namespace DirectX;

class Camera
{
public:
	Camera();	
	Camera(const XMFLOAT3& eye, const XMFLOAT3& target, const XMFLOAT3& up,float arg_near = 0.1f,float arg_far = 1000.0f);
	~Camera();

	virtual void Update();
	void UpdateViewMatrix();
	inline const Vector3& GetEye() { return eye; }
	inline const Vector3& GetTarget() { return target; }
	inline const Vector3& GetUp() { return up; }

	inline const XMMATRIX& GetMatView() { return matView; }
	inline const XMMATRIX& GetMatProjection() { return matProjection; }
	inline const XMMATRIX& GetMatViewProjection() { 
		matViewProjection = matView * matProjection;
		return matViewProjection; }
	inline const XMMATRIX& GetMatBillboard() { return matBillboard; }
	inline const XMMATRIX& GetMatBillboardY() { return matBillboardY; }
	inline const float GetNear() { return nearDist; }
	inline const float GetFar() { return farDist; }
	inline const float GetPhi() { return phi; }
	inline const float GetTheta()
	{
		if (theta < 0)
			return 0;
		return theta;
	}

	void SetMatrixView(const XMFLOAT3& eye, const XMFLOAT3& target, const XMFLOAT3& up);
	void MoveVector(const XMVECTOR& moveeye = { }, const XMVECTOR& movetarget = { }, const XMVECTOR& movrup = { });
	void SetTarget(const XMFLOAT3& target) { this->target = target; viewDirty = true; }
protected:
	XMMATRIX matView;//�r���[�ϊ��s��
	float angle = 0.0f;
	float nearDist = 0.1f;
	float farDist = 1000.0f;
	float phi;
	float theta;

	Vector3 eye;  //���_���W
	Vector3 target;  //���_���W
	Vector3 up;  //���_���W
	// �r���[�s��_�[�e�B�t���O
	bool viewDirty = false;
	//�r���{�[�h�s��
	XMMATRIX matBillboard = DirectX::XMMatrixIdentity();
	// Y�����r���{�[�h�s��
	XMMATRIX matBillboardY = DirectX::XMMatrixIdentity();
	// �ˉe�s��
	XMMATRIX matProjection = DirectX::XMMatrixIdentity();
	// �r���[�ˉe�s��
	XMMATRIX matViewProjection = DirectX::XMMatrixIdentity();

};

