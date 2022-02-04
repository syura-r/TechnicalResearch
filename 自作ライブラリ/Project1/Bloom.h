#pragma once
#include <array>
#include <DirectXMath.h>
#include <d3d12.h>
#include <wrl/client.h>

#include "window.h"

class Bloom {
public:
	Bloom();
	static void SetWindow(Window* window) { Bloom::window = window; };
	void PreDraw();
	void PostDraw();
	void Draw();
private:
	void Initialize();
	void DrawShrinkTextureForBlur();
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

	std::array<VERTEX,4> vertices;
	D3D12_VERTEX_BUFFER_VIEW vbView{};//���_�o�b�t�@�r���[
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;//���_�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff;//�萔�o�b�t�@
	std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, 2> peraResource;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> peraRTVHeap;//�����_�[�^�[�Q�b�g�p
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> peraSRVHeap;//�e�N�X�`���p

	std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, 2> bloomBuffer;//�u���[���p�o�b�t�@

};
