#include "Sprite.h"
#include<math.h>
#include<d3dx12.h>
#include"Texture.h"
Window* Sprite::window = nullptr;
XMMATRIX Sprite::spriteMatProjection = {};
int Sprite::bbIndex = 0;
Sprite::Sprite()
{
	CreateSprite();
}

Sprite::~Sprite()
{
}

void Sprite::StaticInitialize(Window * window)
{
	Sprite::window = window;
	// �ˉe�s��v�Z
	spriteMatProjection = XMMatrixOrthographicOffCenterLH(
		0.0f, (float)window->GetWindowWidth(),
		(float)window->GetWindowHeight(), 0.0f,
		0.0f, 1.0f);
}

void Sprite::CreateSprite()
{
	HRESULT result;
	vertices[0].uv = { 0.0f,1.0f };
	vertices[1].uv = { 0.0f,0.0f };
	vertices[2].uv = { 1.0f,1.0f };
	vertices[3].uv = { 1.0f,0.0f };

	auto dev = DirectXLib::GetInstance()->GetDevice();
	
	//���_�o�b�t�@�̐���
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices)),
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertBuff)
	);
	assert(SUCCEEDED(result));
	
	//���_�o�b�t�@�r���[�̍쐬
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(vertices);
	vbView.StrideInBytes = sizeof(vertices[0]);

	for (int i = 0; i < 3; i++)
	{
		//�萔�o�b�t�@�̍쐬
		result = dev->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDate) + 0xff) & ~0xff),
			D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
			IID_PPV_ARGS(&constBuff[i]));
		assert(SUCCEEDED(result));
	}
}

void Sprite::DrawSprite(const std::string & name, const XMFLOAT2 & position, const float & rotation, const XMFLOAT2 & scale, const XMFLOAT4 & color, const XMFLOAT2 & anchorPoint, const std::string& pipelineName, BLENDTYPE type)
{
	auto cmdList = DirectXLib::GetInstance()->GetCommandList();
	
	D3D12_RESOURCE_DESC resDesc = Texture::GetTexBuff(name).Get()->GetDesc();
	float width = (float)resDesc.Width; //�摜�̉���
	float height = (float)resDesc.Height; //�摜�̏c��
	HRESULT result;

	spriteRotation = rotation;
	spritePosition = XMMatrixTranslation(position.x, position.y, 0);
	spriteMatScale = XMMatrixScaling(scale.x, scale.y, 1);

	if (rect)
	{
		vertices[0].pos = { (0.0f - anchorPoint.x) *rectTexSize.x,(1.0f - anchorPoint.y)*rectTexSize.y,0.0f };
		vertices[1].pos = { (0.0f - anchorPoint.x) *rectTexSize.x,(0.0f - anchorPoint.y)*rectTexSize.y,0.0f };
		vertices[2].pos = { (1.0f - anchorPoint.x) *rectTexSize.x,(1.0f - anchorPoint.y)*rectTexSize.y,0.0f };
		vertices[3].pos = { (1.0f - anchorPoint.x) *rectTexSize.x,(0.0f - anchorPoint.y)*rectTexSize.y,0.0f };
	}
	else
	{
		vertices[0].pos = { (0.0f - anchorPoint.x) * width,(1.0f - anchorPoint.y) * height,0.0f };
		vertices[1].pos = { (0.0f - anchorPoint.x) * width,(0.0f - anchorPoint.y) * height,0.0f };
		vertices[2].pos = { (1.0f - anchorPoint.x) * width,(1.0f - anchorPoint.y) * height,0.0f };
		vertices[3].pos = { (1.0f - anchorPoint.x) * width,(0.0f - anchorPoint.y) * height,0.0f };
	}
	VERTEX* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	memcpy(vertMap, vertices, sizeof(vertices));
	vertBuff->Unmap(0, nullptr);

	//���[���h�s��̍X�V
	spriteMatWorld = XMMatrixIdentity();
	spriteMatWorld *= spriteMatScale;
	spriteMatWorld *= XMMatrixRotationZ(XMConvertToRadians(spriteRotation));
	spriteMatWorld *= spritePosition;

	ConstBufferDate* constMap = nullptr;
	result = constBuff[bbIndex]->Map(0, nullptr, (void**)&constMap);
	constMap->mat = spriteMatWorld * spriteMatProjection;//�s��̍���
	constMap->color = color;
	constBuff[bbIndex]->Unmap(0, nullptr);

	PipelineState::SetPipeline(pipelineName,type);

	//�f�X�N���v�^�q�[�v�̔z��
	ID3D12DescriptorHeap* ppHeaps[] = { Texture::GetBasicDescHeap().Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	cmdList->SetGraphicsRootConstantBufferView(0, constBuff[bbIndex]->GetGPUVirtualAddress());
	//�C���f�b�N�X�o�b�t�@�̃Z�b�g�R�}���h
	cmdList->SetGraphicsRootDescriptorTable(1, Texture::GetGpuDescHandleSRV(name)); 

	cmdList->DrawInstanced(4, 1, 0, 0);

}

void Sprite::NoPipelineDraw(const std::string& name, const XMFLOAT2& position, const float& rotation, const XMFLOAT2& scale, const XMFLOAT4& color, const XMFLOAT2& anchorPoint)
{
	auto cmdList = DirectXLib::GetInstance()->GetCommandList();

	D3D12_RESOURCE_DESC resDesc = Texture::GetTexBuff(name).Get()->GetDesc();
	float width = (float)resDesc.Width; //�摜�̉���
	float height = (float)resDesc.Height; //�摜�̏c��
	HRESULT result;

	spriteRotation = rotation;
	spritePosition = XMMatrixTranslation(position.x, position.y, 0);
	spriteMatScale = XMMatrixScaling(scale.x, scale.y, 1);

	if (rect)
	{
		vertices[0].pos = { (0.0f - anchorPoint.x) * rectTexSize.x,(1.0f - anchorPoint.y) * rectTexSize.y,0.0f };
		vertices[1].pos = { (0.0f - anchorPoint.x) * rectTexSize.x,(0.0f - anchorPoint.y) * rectTexSize.y,0.0f };
		vertices[2].pos = { (1.0f - anchorPoint.x) * rectTexSize.x,(1.0f - anchorPoint.y) * rectTexSize.y,0.0f };
		vertices[3].pos = { (1.0f - anchorPoint.x) * rectTexSize.x,(0.0f - anchorPoint.y) * rectTexSize.y,0.0f };
	}
	else
	{
		vertices[0].pos = { (0.0f - anchorPoint.x) * width,(1.0f - anchorPoint.y) * height,0.0f };
		vertices[1].pos = { (0.0f - anchorPoint.x) * width,(0.0f - anchorPoint.y) * height,0.0f };
		vertices[2].pos = { (1.0f - anchorPoint.x) * width,(1.0f - anchorPoint.y) * height,0.0f };
		vertices[3].pos = { (1.0f - anchorPoint.x) * width,(0.0f - anchorPoint.y) * height,0.0f };
	}
	VERTEX* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	memcpy(vertMap, vertices, sizeof(vertices));
	vertBuff->Unmap(0, nullptr);

	//���[���h�s��̍X�V
	spriteMatWorld = XMMatrixIdentity();
	spriteMatWorld *= spriteMatScale;
	spriteMatWorld *= XMMatrixRotationZ(XMConvertToRadians(spriteRotation));
	spriteMatWorld *= spritePosition;

	ConstBufferDate* constMap = nullptr;
	result = constBuff[bbIndex]->Map(0, nullptr, (void**)&constMap);
	constMap->mat = spriteMatWorld * spriteMatProjection;//�s��̍���
	constMap->color = color;
	constBuff[bbIndex]->Unmap(0, nullptr);

	//�f�X�N���v�^�q�[�v�̔z��
	ID3D12DescriptorHeap* ppHeaps[] = { Texture::GetBasicDescHeap().Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	cmdList->SetGraphicsRootConstantBufferView(0, constBuff[bbIndex]->GetGPUVirtualAddress());
	//�C���f�b�N�X�o�b�t�@�̃Z�b�g�R�}���h
	cmdList->SetGraphicsRootDescriptorTable(2, Texture::GetGpuDescHandleSRV(name));

	cmdList->DrawInstanced(4, 1, 0, 0);
}

void Sprite::SpriteSetTextureRect(const std::string & name, const float & tex_x, const float & tex_y, const float & tex_width, const float & tex_height)
{
	HRESULT result;

	D3D12_RESOURCE_DESC resDesc = Texture::GetTexBuff(name).Get()->GetDesc();

	float tex_left = tex_x / resDesc.Width;
	float tex_right = (tex_x + tex_width) / resDesc.Width;
	float tex_top = tex_y / resDesc.Height;
	float tex_bottom = (tex_y + tex_height) / resDesc.Height;

	vertices[0].uv = { tex_left,tex_bottom };
	vertices[1].uv = { tex_left,tex_top };
	vertices[2].uv = { tex_right,tex_bottom };
	vertices[3].uv = { tex_right,tex_top };

	rectTexSize.x = tex_width;
	rectTexSize.y = tex_height;


	rect = true;
}
