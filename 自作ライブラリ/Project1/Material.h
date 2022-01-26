#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <string>

class Material
{
public:
	Material();
	~Material();

private://�ȗ��p
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public://�萔�o�b�t�@�p�\����
	struct ConstBufferData
	{
		XMFLOAT3 ambient;
		float pad1;
		XMFLOAT3 diffuse;
		float pad2;
		XMFLOAT3 specular;
		float alpha;
	};

public://�}�e���A�����(�p�u���b�N�ł����̂��H)
	std::string name;
	XMFLOAT3 ambient;
	XMFLOAT3 diffuse;
	XMFLOAT3 specular;
	float alpha;
	std::string textureFileName;


public://�ÓI�����o�֐�
	static Material* Create();//����


public://�����o�֐�
	void Initialize();//������
	ID3D12Resource* GetConstBuffer() { return constBuff.Get(); };
	void LoadTexture();
	void Update();


private://GPU�Ƃ̂����ŕK�v�ȕϐ�	
	ComPtr<ID3D12Resource> texbuff;//�e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource> constBuff;//�萔�o�b�t�@
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;//�V�F�[�_�[���\�[�X�r���[�n���h��CPU
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;//�V�F�[�_�[���\�[�X�r���[�n���h��GPU
};

