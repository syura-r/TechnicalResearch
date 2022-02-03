#pragma once
#include"Particle.h"
#include<forward_list>
#include<unordered_map>
#include<string>
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>

#include "Camera.h"
#include "ComputeShade.h"

class ParticleManager
{
private: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
public://�\����
	////���_�f�[�^
	//struct VertexPos
	//{
	//	float frame;
	//	//���S�t���O
	//	float isDead;
	//	float pad;		
	//	//�r���{�[�h���邩�ǂ���
	//	int billboardActive;

	//};
	//�萔�o�b�t�@
	struct ConstBufferData
	{
		XMMATRIX mat;	// �r���[�v���W�F�N�V�����s��
		XMMATRIX matBillboard;	// �r���{�[�h�s��
	};

public://�����o�֐�
	static ParticleManager* GetInstance();
	void Add(Particle* newParticle,const std::string& TexName);
	void Initialize();
	void Update();
	void Draw();
	inline void SetCamera(Camera* camera) { this->camera = camera; }
	void CreateConstBuff();
	void CreateModel();
	void End();
private://�����o�֐�
	ParticleManager() = default;
	ParticleManager(const ParticleManager&) = delete;
	~ParticleManager() = default;
	ParticleManager& operator=(const ParticleManager&) = delete;
	int activParticleCount = 0;
	//�X�V���ɕύX����Ȃ��f�[�^
	std::unordered_map<std::string,std::vector<ParticleParameter>>paramData;
	//std::vector<ParticleParameter> paramData;
	//�X�V���ɕύX����AVS�ɑ�����f�[�^
	//std::vector<OutputData> vertData;
	std::unordered_map<std::string, std::vector<OutputData>>vertData;

	//�e�N�X�`�����Ɋ��蓖�Ă�ꂽ�z��̔ԍ����i�[����z��
	//std::unordered_map<std::string,int>texNumbers;

private://�����o�ϐ�
	//std::unordered_map<std::string,std::vector<OutputData>> particles;
	//std::unordered_map<std::string, std::vector<ParticleParameter>> particleParams;
	//void* vertData = nullptr;
	//void* paramData = nullptr;

	ComPtr<ID3D12Resource> constBuff;
	Camera* camera = nullptr;
	ComPtr<ID3D12Resource> vertBuff;
	//ComPtr<ID3D12Resource> matBuff;
	//ComPtr<ID3D12Resource> indexBuff;

	D3D12_VERTEX_BUFFER_VIEW vbView;
	//D3D12_INDEX_BUFFER_VIEW ibView = {};
	//D3D12_VERTEX_BUFFER_VIEW matView{};//���_�o�b�t�@�r���[

	std::unique_ptr<ComputeShade> computeShade;
	std::vector<OutputData> verts;
	std::vector<ParticleParameter> params;

private: // �ÓI�����o�萔
	static const int vertexCount = 10000;		// ���_��
private:
};

