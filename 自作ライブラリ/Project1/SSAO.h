#pragma once
#include <array>
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
		DirectX::XMFLOAT4 color;//�F(RGBA)
		DirectX::XMMATRIX mat;//3D�ϊ��s��
	};
	struct ConstBuffer2 
	{
		DirectX::XMMATRIX view; //�r���[
		DirectX::XMMATRIX proj; //�v���W�F�N�V����
		DirectX::XMMATRIX invproj; //�t�v���W�F�N�V����
		DirectX::XMFLOAT4 randNormal[trycnt];

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
	std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, 3> constBuff2;//�萔�o�b�t�@
	//Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> depthSRVHeap;
	int bbindex = 0;
private:
	static Camera* camera;
public:
	static void SetCamera(Camera* arg_camera)
	{
		camera = arg_camera;
	}
};
