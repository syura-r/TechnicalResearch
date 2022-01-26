#pragma once
#include<DirectXMath.h>

class PointLight
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
		XMFLOAT3 lightpos;
		float pad1;//�p�f�B���O
		XMFLOAT3 lightcolor;
		float pad2;//�p�f�B���O
		XMFLOAT3 lightatten;
		UINT active;
	};
public://�����o�֐�
	inline void SetLightPos(const Vector3& pos){
		this->lightpos = pos;}
	inline const Vector3& GetLightPos(){ return lightpos; }

	inline void SetLightColor(const Vector3&  color) {
		this->lightcolor = color;}
	inline const Vector3& GetLightColor() { return lightcolor; }

	inline void SetLightAtten(const Vector3& atten) {
		this->lightAtten = atten;
	}
	inline const Vector3& GetLightAtten() { return lightAtten; }
	inline void SetActive(bool active) { this->active = active; }
	inline bool IsActive() { return active; }

private://�����o�ϐ�
	Vector3 lightpos = { 0,0,0 };//���C�g���W(���[���h���W�n)
	Vector3 lightcolor = { 1,1,1 };//���C�g�̐F
	Vector3 lightAtten = { 0.001f,0.001f,0.001f };//���C�g�̋��������W��
	bool active = false;
};
