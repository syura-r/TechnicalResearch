#pragma once
#include <d3d12.h>
#include <DirectXMath.h>
#include <wrl/client.h>

class Camera;

class SSAO
{
public:
	SSAO();
	void Draw();
private:
	void SetCameraBuff();
	static const int trycnt = 12;
	struct ConstBuffer
	{
		DirectX::XMFLOAT4 color;//色(RGBA)
		DirectX::XMMATRIX mat;//3D変換行列
	};
	struct ConstBuffer2 
	{
		DirectX::XMMATRIX view; //ビュー
		DirectX::XMMATRIX proj; //プロジェクション
		DirectX::XMMATRIX invproj; //逆プロジェクション
		DirectX::XMFLOAT4 randNormal[trycnt];

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
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff2[3];//定数バッファ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> depthSRVHeap;
	int bbindex = 0;
private:
	static Camera* camera;
public:
	static void SetCamera(Camera* arg_camera)
	{
		camera = arg_camera;
	}
};
