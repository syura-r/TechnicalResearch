#pragma once
#include <d3d12.h>
#include <DirectXMath.h>
#include<wrl.h>
#include"DirectXLib.h"
#include"window.h"
#include"PipelineState.h"
using namespace Microsoft::WRL;

using namespace DirectX;

class Sprite
{
public:
	Sprite();
	~Sprite();
	//�ÓI�����o�֐�
	static void StaticInitialize(Window* window);
	void DrawSprite(const std::string& name,const XMFLOAT2& position,const float& rotation = 0.0f,const XMFLOAT2& scale = { 1.0f,1.0f },const XMFLOAT4& color = { 1,1,1,1 },const XMFLOAT2& anchorPoint = {0.5f,0.5f},const std::string& pipelineName = "Sprite", BLENDTYPE type = ALPHA);
	void NoPipelineDraw(const std::string& name, const XMFLOAT2& position, const float& rotation, const XMFLOAT2& scale, const XMFLOAT4& color, const XMFLOAT2& anchorPoint);

	void SpriteSetTextureRect(const std::string& name,const float& tex_x,const float& tex_y,const float& tex_width,const float& tex_height);
	static void SetBbIndex()
	{
		bbIndex = DirectXLib::GetInstance()->GetBbIndex();
	}
protected:
	void CreateSprite();

	//�ÓI�����o�ϐ�
	static Window* window;
	static XMMATRIX spriteMatProjection;
	static int bbIndex;

	//�V�F�[�_�\�p
	struct ConstBufferDate {
		XMFLOAT4 color;
		XMMATRIX mat;
	};
	struct VERTEX
	{
		XMFLOAT3 pos;
		XMFLOAT2 uv;
	};


	float spriteRotation = 0.0f;
	XMMATRIX spritePosition{};
	XMMATRIX spriteMatScale{};
	XMMATRIX spriteMatWorld{};
	VERTEX vertices[4];
	ComPtr<ID3D12Resource> vertBuff;//���_�o�b�t�@
	ComPtr<ID3D12Resource> constBuff[3];//�萔�o�b�t�@
	D3D12_VERTEX_BUFFER_VIEW vbView{};//���_�o�b�t�@�r���[
	
	bool rect = false;//�؂�o�������Ă��邩�ǂ���
	XMFLOAT2 rectTexSize;

};

