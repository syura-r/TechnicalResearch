#pragma once
#include <d3d12.h>
#include <DirectXMath.h>
#include<vector>
#include<wrl.h>
#include"DirectXTex.h"
#include"DirectXLib.h"
#include"Camera.h"
#include"Input.h"
using namespace Microsoft::WRL;

using namespace DirectX;

#define DIRECTINPUT_VERSION 0x0800 //DirectInput�̃o�[�W����

struct ConstBufferDate {
	XMFLOAT4 color;
	XMMATRIX mat;
};

enum Direction
{
	Right,
	Left,
	Up,
	Down,
};

class CreateFigure
{
public:
	CreateFigure();
	~CreateFigure();
	void Initialize();
	void Craete(float r, const int vertex, float descHeight, float descWidth, XMFLOAT3 vector3, XMFLOAT4 color);
	void CreateSprite(float rotation, XMFLOAT2 trans, XMFLOAT2 scale,XMFLOAT4 color, ID3D12Resource* texbuff, XMFLOAT2 anchorpoint = {0.5f,0.5f});
	virtual void DrawSprite(XMFLOAT4 color,ID3D12DescriptorHeap* basicHeap, ID3D12PipelineState* pipelinestate, ID3D12RootSignature* rootsignature);
	void Draw(ID3D12PipelineState* pipelinestate);
	void ChangeWidth(float width, float MaxHp,bool damege);	
	virtual void Move(XMFLOAT3 position);
	bool GetIsDead();
protected:
	struct VERTEX
	{
		XMFLOAT3 pos;//xyz���W
		//XMFLOAT3 normal;//�@���x�N�g��
		XMFLOAT2 uv; //uv���W
	};
	struct ConstBufferDate {
		XMFLOAT4 color;//�F(RGBA)
		XMMATRIX mat;//3D�ϊ��s��
	};
	const int window_width = 1280;//����
	const int window_height = 720;//����
	XMFLOAT2 baseAnchor;
	float width;
	float height;
	bool isDead = false;
	float velX;
	void CreateVartex(float r, int n, XMFLOAT3 *vertices, unsigned short *indices, float descHeight, float descWidth);
	float r;
	int vertex;
	ComPtr < ID3D12Resource> texbuff;

	//std::vector<XMFLOAT3> vertices;
	//std::vector<unsigned short> indices;

	XMFLOAT3 vertices2[100];
	unsigned short indices[300];
	VERTEX vertices[4];

	float baseRotation = 0.0f;
	XMFLOAT2 basePos;
	XMFLOAT2 baseMatScale{};
	
	XMFLOAT3 spritePosition;
	float spriteRotation = 0.0f;
	XMMATRIX spriteTrans{};
	XMMATRIX spriteMatScale{};//�X�P�[�����O�s��
	XMMATRIX spriteMatWorld{};
	XMMATRIX spriteMatProjection{};
	//XMMATRIX matWorld;
	//XMMATRIX matScale;//�X�P�[�����O�s��
	//XMMATRIX matRot;//��]�s��
	//XMFLOAT3 rotation = { 0.0f,0.0f,0.0f };
	//XMMATRIX matTrans;//���s�ړ��s��
	//XMFLOAT3 position = { 0.0f, 0.0f, 0.0f };//���W��ϐ��Ƃ��Ď���
	//ConstBufferDate* constMap = nullptr;
	ConstBufferDate* constMap = nullptr;

	D3D12_HEAP_PROPERTIES heapprop{};//���_�q�[�v�ݒ�
	D3D12_RESOURCE_DESC resdesc{};  //���\�[�X�ݒ�
	ComPtr<ID3D12Resource> vertBuff;
	ComPtr<ID3D12Resource> constBuff;
	XMFLOAT3* vertMap;
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	ComPtr<ID3D12DescriptorHeap> basicDescHeap;
	D3D12_INDEX_BUFFER_VIEW ibView{};
	ID3D12Resource* indexBuff;
	//D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandleCBV;
	//D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	//ComPtr<ID3D12Resource> depthBuffer;
	//XMMATRIX matProjection; //�ˉe�ϊ��s��

	Camera* camera;
	Input* input;
};

