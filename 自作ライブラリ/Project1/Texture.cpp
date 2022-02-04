#include "Texture.h"
#include"DirectXLib.h"
std::unordered_map<std::string, TexMetadata> Texture::metadetes = {};
std::unordered_map<std::string, ComPtr <ID3D12Resource> > Texture::texbuffs = {};
ComPtr < ID3D12DescriptorHeap> Texture::basicDescHeape = nullptr;
std::unordered_map<std::string, unsigned char>Texture::texIndexes = {};
unsigned char Texture::texIndex = 0;
std::unordered_map<std::string, XMINT2> Texture::createTexSize = {};

void Texture::LoadTexture(const std::string& name, const std::string& texName)
{
	HRESULT result;
	auto dev = DirectXLib::GetInstance()->GetDevice();
	
	TexMetadata metadata{};
	ScratchImage scratchlmg{};
	// �t�@�C���p�X������
	std::string filepath = "Resources/Texture/" + texName;
	wchar_t wfilepath[128];

	// ���j�R�[�h������ɕϊ�
	MultiByteToWideChar(CP_ACP, 0, filepath.c_str(), -1, wfilepath, _countof(wfilepath));

	result = LoadFromWICFile(
		wfilepath,
		WIC_FLAGS_NONE,
		&metadata, scratchlmg);

	const Image* img = scratchlmg.GetImage(0, 0, 0);//���f�[�^���o	

	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format,
		metadata.width,
		(UINT)metadata.height,
		(UINT16)metadata.arraySize,
		(UINT16)metadata.mipLevels
	);

	ComPtr < ID3D12Resource> texbuff;
	result = dev->CreateCommittedResource(//GPU���\�[�X�̐���
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK,
			D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,//�e�N�X�`���p�w��
		nullptr,
		IID_PPV_ARGS(&texbuff));

	//�f�[�^�̓]��
	//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
	result = texbuff->WriteToSubresource(
		0,
		nullptr,//�S�̈�փR�s�[
		img->pixels,//���f�[�^�ւ̃A�h���X
		(UINT)img->rowPitch,//1���C���T�C�Y
		(UINT)img->slicePitch//�S�T�C�X
	);

	metadetes[name] = metadata;
	texbuffs[name] = texbuff;

	//�V�F�[�_���\�[�X�r���[�쐬
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};//�ݒ�\����
	srvDesc.Format = metadata.format;//RGBA
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = 1;


	dev->CreateShaderResourceView(texbuffs[name].Get(),//�r���[�Ɗ֘A�t����o�b�t�@
		&srvDesc,//�e�N�X�`���ݒ���
		CD3DX12_CPU_DESCRIPTOR_HANDLE(basicDescHeape->GetCPUDescriptorHandleForHeapStart(),
			texIndex,dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));

	texIndexes[name] = texIndex;

	texIndex++;

}


void Texture::CreateTexture(const std::string& name, const int& width, const int& height, XMFLOAT4* color)
{
	HRESULT result;
	auto dev = DirectXLib::GetInstance()->GetDevice();

	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		(UINT64)width,
		(UINT)height,
		1,
		1
	);


	ComPtr < ID3D12Resource> texbuff;
	result = dev->CreateCommittedResource(//GPU���\�[�X�̐���
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK,
			D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,//�e�N�X�`���p�w��
		nullptr,
		IID_PPV_ARGS(&texbuff));

	result = texbuff->WriteToSubresource(
		0,
		nullptr,//�S�̈�փR�s�[
		color,//���f�[�^�ւ̃A�h���X
		sizeof(XMFLOAT4) * width,//1���C���T�C�Y
		sizeof(XMFLOAT4) * width * height//�S�T�C�X
	);

	texbuffs[name] = texbuff;
	createTexSize[name].x = width;
	createTexSize[name].y = height;


	//�V�F�[�_���\�[�X�r���[�쐬
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};//�ݒ�\����
	srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;//RGBA
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = 1;


	dev->CreateShaderResourceView(texbuffs[name].Get(),//�r���[�Ɗ֘A�t����o�b�t�@
		&srvDesc,//�e�N�X�`���ݒ���
		CD3DX12_CPU_DESCRIPTOR_HANDLE(basicDescHeape->GetCPUDescriptorHandleForHeapStart(),
			texIndex, dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));

	texIndexes[name] = texIndex;

	texIndex++;
}

void Texture::AddTexture(const std::string& name, ID3D12Resource* texBuff)
{
	if (texbuffs[name] != nullptr)
		assert(0);
	auto dev = DirectXLib::GetInstance()->GetDevice();

	texbuffs[name] = texBuff;
	
	//�V�F�[�_���\�[�X�r���[�쐬
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};//�ݒ�\����
	srvDesc.Format = texBuff->GetDesc().Format;//RGBA
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = 1;


	dev->CreateShaderResourceView(texbuffs[name].Get(),//�r���[�Ɗ֘A�t����o�b�t�@
		&srvDesc,//�e�N�X�`���ݒ���
		CD3DX12_CPU_DESCRIPTOR_HANDLE(basicDescHeape->GetCPUDescriptorHandleForHeapStart(),
			texIndex, dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));

	texIndexes[name] = texIndex;

	texIndex++;

}

void Texture::SendColor(const std::string& name, XMFLOAT4* color)
{
	HRESULT result;
	result = texbuffs[name]->WriteToSubresource(
		0,
		nullptr,//�S�̈�փR�s�[
		color,//���f�[�^�ւ̃A�h���X
		sizeof(XMFLOAT4) * createTexSize[name].x,//1���C���T�C�Y
		sizeof(XMFLOAT4) * createTexSize[name].x * createTexSize[name].y//�S�T�C�X
	);
}


void Texture::Initialize()
{
	HRESULT result;
	//�ݒ�\����
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;  //�V�F�[�_�[���猩����
	descHeapDesc.NumDescriptors = 255;
	//����
	result = DirectXLib::GetInstance()->GetDevice()->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&basicDescHeape));
	assert(SUCCEEDED(result));
}

