#pragma once
#include <d3d12.h>
#include <DirectXMath.h>
#include <wrl/client.h>

#include "window.h"

class ShadowMap
{
public:
	ShadowMap();
	static void SetWindow(Window* window) { ShadowMap::window = window; };
	void PreDraw();
	void PostDraw();
	void Draw();
private:
	void Initialize();
	static Window* window;

	//�V�F�[�_�\�p
	struct ConstBufferDate {
		DirectX::XMFLOAT4 color;
		DirectX::XMMATRIX mat;
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
	Microsoft::WRL::ComPtr<ID3D12Resource> shadowResource;
	Microsoft::WRL::ComPtr < ID3D12Resource> depthBuffer;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvHeap;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> peraRTVHeap;//�����_�[�^�[�Q�b�g�p
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> peraSRVHeap;//�e�N�X�`���p
	ID3D12GraphicsCommandList* cmdList;
	ID3D12Device* dev;
	
	const int resourceWidth = 1920 * 1;
	const int resourceHeight = 1080 * 1;

}; 

