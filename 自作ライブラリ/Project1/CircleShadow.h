#pragma once
#include<DirectXMath.h>

class CircleShadow
{
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public://�o�b�t�@�p�f�[�^
	struct ConstBuff
	{
		XMVECTOR dir;
		XMFLOAT3 casterpos;
		float distanceCasterLight;
		XMFLOAT3 atten;
		float pad;//�p�f�B���O
		XMFLOAT2 factorAngleCos;
		unsigned int active;
		float pad2;//�p�f�B���O
	};
public://�����o�֐�
	inline void SetDir(const XMVECTOR& dir) { this->dir = DirectX::XMVector3Normalize(dir); }
	inline const XMVECTOR& GetDir() { return dir; }

	inline void SetCasterPos(const XMFLOAT3& pos) {
		this->casterPos = pos;
	}
	inline const Vector3& GetCasterPos() { return casterPos; }

	inline void SetDistanceCasterLight(float  distanceCasterLight) {
		this->distanceCasterLight = distanceCasterLight;
	}
	inline float GetDistanceCasterLight() { return distanceCasterLight; }

	inline void SetAtten(const Vector3& atten) {
		this->atten = atten;
	}
	inline const Vector3& GetAtten() { return atten; }

	inline void SetFactorAngle(const XMFLOAT2& factorAngle)
	{
		this->factorAngleCos.x = cosf(DirectX::XMConvertToRadians(factorAngle.x));
		this->factorAngleCos.y = cosf(DirectX::XMConvertToRadians(factorAngle.y));
	}
	inline const XMFLOAT2 GetFactorAngleCos() { return factorAngleCos; }

	inline void SetActive(bool active) { this->active = active; }
	inline bool IsActive() { return active; }

private://�����o�ϐ�
	XMVECTOR dir = { 1,0,0,0 };//����(�P�ʃx�N�g��)
	float distanceCasterLight = 100.0f;//�L���X�^�[�ƃ��C�g�̋���
	Vector3 casterPos = { 0,0,0 };//�L���X�^�[���W(���[���h���W�n)
	Vector3 atten = { 0.5f,0.6f,0.0f };//���C�g�̋��������W��
	XMFLOAT2 factorAngleCos = { 0.2f,0.5f };//���C�g�����p�x(�J�n�p�x�A�I���p�x)
	bool active = false;



};