#pragma once
#include<DirectXMath.h>
#include<d3d12.h>
#include <wrl.h>
#include<d3dx12.h>
#include"DirectionalLight.h"
#include"PointLight.h"
#include"SpotLight.h"
#include"CircleShadow.h"
#include "Vector.h"

class LightGroup
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public://�ÓI�萔
	//�������̐�
	static const int DirLightNum = 3;
	//�_�����̐�
	static const int PointLightNum = 9;
	//�X�|�b�g���C�g�̐�
	static const int SpotLightNum = 3;
	//�ۉe�̐�
	static const int CircleShadowNum = 1;
public:
	//�萔�o�b�t�@�p�\����
	struct ConstBufferData
	{
		XMFLOAT3 ambientColor;
		float pad;//�p�f�B���O
		DirectionalLight::ConstBuff dirLights[DirLightNum];
		PointLight::ConstBuff pointLights[PointLightNum];
		SpotLight::ConstBuff spotLights[SpotLightNum];
		//�ۉe�p
		CircleShadow::ConstBuff circleShadows[CircleShadowNum];
	};
public://�ÓI�����o�֐�

	static LightGroup* Create();

public://�����o�֐�
	void Initialize();
	void TransferConstBuff();
	//���s����
	void SetAmbientColor(const Vector3& color);
	void SetDirLightActive(int index, bool active);
	void SetDirLightDir(const int& index, const XMVECTOR& lightdir);
	void SetDirLightColor(const int& index, const Vector3 lightcolor);
	//�_����
	void SetPointLightActive(int index, bool active);
	void SetPointLightPos(int index, const Vector3& pos);
	void SetPointLightColor(int index, const Vector3& color);
	void SetPointLightAtten(int index, const Vector3& atten);
	//�X�|�b�g���C�g
	void SetSpotLightActive(int index, bool active);
	void SetSpotLightDir(int index, const XMVECTOR& dir);
	void SetSpotLightPos(int index, const Vector3& pos);
	void SetSpotLightColor(int index, const Vector3& color);
	void SetSpotLightAtten(int index, const Vector3& atten);
	void SetSpotLightFactorAngle(int index, const XMFLOAT2& lightFactorAngle);
	//�ۉe
	void SetCircleShadowActive(int index, bool active);
	void SetCircleShadowCasterPos(int index, const Vector3& pos);
	void SetCircleShadowDir(int index, const XMVECTOR& dir);
	void SetCircleShadowDistanceCasterLight(int index, float distanceCasterLight);
	void SetCircleShadowAtten(int index, const Vector3& atten);
	void SetCircleShadowFactorAngle(int index, const XMFLOAT2& factorAngle);

	void Update();
	void Draw(UINT rootParameterIndex);

private://�����o�ϐ�
	ComPtr<ID3D12Resource> constBuff;//�萔�o�b�t�@

	XMFLOAT3 ambientColor{ 1,1,1 };
	//���s�����̔z��
	DirectionalLight dirLights[DirLightNum];
	//�_�����̔z��
	PointLight pointLights[PointLightNum];
	//�X�|�b�g���C�g�̔z��
	SpotLight spotLights[SpotLightNum];
	//�ۉe�̔z��
	CircleShadow circleShadows[CircleShadowNum];
	bool dirty = false;

};
