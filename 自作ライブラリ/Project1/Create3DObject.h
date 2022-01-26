#pragma once

#include <DirectXMath.h>
#include <d3d12.h>
#include"DirectXTex.h"
#include"Input.h"
#include<wrl.h>
#include"Camera.h"
#include"DirectXLib.h"
using namespace Microsoft::WRL;
using namespace DirectX;

#define DIRECTINPUT_VERSION 0x0800 //DirectInput�̃o�[�W����

class DirectXLib;
class Create3DObject
{

public:
	Create3DObject();
	~Create3DObject();
	virtual void CreatePyramid(const int vertex, const float topHeight, const int R, XMFLOAT3 scale, XMFLOAT3 rot, XMFLOAT3 trans, bool projective, XMFLOAT4 color, ID3D12Resource* texbuff, TexMetadata& metadata);
	virtual void CreateBox(const int vertex, const float TopHeight, const int Width, XMFLOAT3 scale, XMFLOAT3 rot, XMFLOAT3 trans, bool projective, XMFLOAT4 color, ID3D12Resource* texbuff, TexMetadata& metadata);
	virtual void DrawPyramid(XMFLOAT4 color,ID3D12PipelineState* pipelinestate, ID3D12RootSignature * rootsignature);
	virtual void DrawBox(XMFLOAT4 color, ID3D12PipelineState* pipelinestate, ID3D12RootSignature * rootsignature);
	XMFLOAT3 GetPosition();
	virtual void Move();
	virtual void Update();
	virtual void SetDirection();
	//virtual void Hit(Create3DObject* object);
	virtual void Damege();
	bool GetIsDead();
	void Die();
	float GetR();
	int GetWidth();
protected:
	Camera* camera;
	DirectXLib* device;
	Input* input;
	const int window_width = 1280;//����
	const int window_height = 720;//����
	float r;
	bool isDead = false;
	int vertex;
	int height, width;
	int hp;
	bool isDamege = false;
	XMFLOAT4 baseColor;
	int damegeCounter = 20;
	XMFLOAT3 basePosition;
	XMFLOAT3 baseRot;
	XMMATRIX baseScale;

	struct Vertex
	{
		XMFLOAT3 pos;//xyz���W
		XMFLOAT3 normal;//�@���x�N�g��
		XMFLOAT2 uv; //uv���W
	};
	struct ConstBufferDate {
		XMFLOAT4 color;//�F(RGBA)
		XMMATRIX mat;//3D�ϊ��s��
	};
	Vertex vertices[200];
	unsigned short indices[600];
	XMMATRIX matWorld;
	XMMATRIX matScale;//�X�P�[�����O�s��
	XMMATRIX matRot;//��]�s��
	XMFLOAT3 rotation = { 0.0f,0.0f,0.0f };
	XMMATRIX matTrans;//���s�ړ��s��
	XMFLOAT3 position = { 0.0f, 0.0f, 0.0f };//���W��ϐ��Ƃ��Ď���


	ComPtr<ID3D12Resource> vertBuff;
	//GPU��̃o�b�t�@�ɑΉ��������z���������擾
	Vertex* vertMap = nullptr;
	//�C���f�b�N�X�o�b�t�@�r���[�̍쐬�ƃZ�b�g
	D3D12_INDEX_BUFFER_VIEW ibView{};
	ConstBufferDate* constMap = nullptr;
	XMMATRIX matProjection; //�ˉe�ϊ��s��
	ComPtr<ID3D12Resource> indexBuff;
	ComPtr<ID3D12Resource> constBuff;
	ComPtr<ID3D12Resource> depthBuffer;

	//���_�o�b�t�@�r���[�̍쐬
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	ComPtr<ID3D12DescriptorHeap> basicDescHeap;

	//2�̃r���[��GPU���̃A�h���X���L�^����
	D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandleCBV;
	D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;

};

