#pragma once
#include<DirectXTex.h>
#include<vector>
#include<unordered_map> 
#include<d3dx12.h>
#include<string>

#include "DirectXLib.h"
using namespace Microsoft::WRL;
using namespace DirectX;

class Texture
{
public:
	///<summary>
	///2D�摜��ǂݍ���
	///</summary>
	///<param name="name">���X�g�̃L�[�p�̖��O</param>
	///<param name="texName">�ǂݍ��݂����e�N�X�`���̃t�@�C����</param>
	static void LoadTexture(const std::string& name, const std::string& texName);
	///<summary>
	///2D�摜�����삵�ēo�^
	///</summary>
	///<param name="name">���X�g�̃L�[�p�̖��O</param>
	///<param name="width">�쐬�����m�̉���</param>
	///<param name="height">�쐬�����m�̏c��</param>
	///<param name="color">�F�z��̃|�C���^</param>
	static void CreateTexture(const std::string& name, const int& width, const int& height, XMFLOAT4* color);
	///<summary>
	///�O���ō쐬�����e�N�X�`���o�b�t�@�����X�g�ɓo�^
	///</summary>
	///<param name="name">���X�g�̃L�[�p�̖��O</param>
	///<param name="texBuff">�o�^����e�N�X�`���o�b�t�@</param>
	static void AddTexture(const std::string& name, ID3D12Resource* texBuff);
	///<summary>
	///����2D�摜�̐F��ύX����
	///</summary>
	///<param name="name">���X�g�̃L�[�p�̖��O</param>
	///<param name="color">�F�z��̃|�C���^</param>
	static void SendColor(const std::string& name, XMFLOAT4* color);
	static void Initialize();
	static const char& GetTexIndex(const std::string& name) { return texIndexes[name]; };
	inline static ComPtr < ID3D12Resource> GetTexBuff(std::string name) { return texbuffs[name]; };
	static TexMetadata& GetMetadate(std::string name) { return metadetes[name]; };
	inline static ComPtr < ID3D12DescriptorHeap > GetBasicDescHeap() {
		if (basicDescHeape == nullptr)
			assert(0);
		return basicDescHeape;
	};
	inline static CD3DX12_GPU_DESCRIPTOR_HANDLE GetGpuDescHandleSRV(const std::string& name)
	{
		if (texIndexes.find(name) == texIndexes.end())
			assert(0);
		CD3DX12_GPU_DESCRIPTOR_HANDLE descHeap = CD3DX12_GPU_DESCRIPTOR_HANDLE(basicDescHeape->GetGPUDescriptorHandleForHeapStart(),
			texIndexes[name], DirectXLib::GetInstance()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));
		return descHeap;
	};
private:
	static std::unordered_map<std::string, TexMetadata> metadetes;
	static std::unordered_map<std::string, ComPtr <ID3D12Resource> >texbuffs;
	static ComPtr < ID3D12DescriptorHeap>basicDescHeape;
	static std::unordered_map<std::string, unsigned char> texIndexes;
	static unsigned char texIndex;

	//����摜�̕��ƍ�����ۑ����Ă����ϐ�
	static std::unordered_map<std::string, XMINT2> createTexSize;
};

