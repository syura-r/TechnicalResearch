#pragma once
#include <d3d12.h>
#include <DirectXMath.h>
#include <wrl/client.h>

class SSAOCombine
{
public:
	SSAOCombine();
	void Draw();
private:
	struct ConstBuffer
	{
		DirectX::XMFLOAT4 color;//�F(RGBA)
		DirectX::XMMATRIX mat;//3D�ϊ��s��
	};
	struct VERTEX
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 uv;
	};

	VERTEX vertices[4];
	D3D12_VERTEX_BUFFER_VIEW vbView{};//���_�o�b�t�@�r���[
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;//���_�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff;//�萔�o�b�t�@
};

