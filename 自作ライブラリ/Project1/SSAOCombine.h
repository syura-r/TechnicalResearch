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
		DirectX::XMFLOAT4 color;//色(RGBA)
		DirectX::XMMATRIX mat;//3D変換行列
	};
	struct VERTEX
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 uv;
	};

	VERTEX vertices[4];
	D3D12_VERTEX_BUFFER_VIEW vbView{};//頂点バッファビュー
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;//頂点バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff;//定数バッファ
};

