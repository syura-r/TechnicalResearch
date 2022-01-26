#include "Material.h"
#include"DirectXLib.h"
#include"Texture.h"

Material::Material()
{
}


Material::~Material()
{
}

Material * Material::Create()
{
	Material* instace = new Material;
	instace->Initialize();
	return instace;
}

void Material::Initialize()
{
	HRESULT result;

	ambient = { 0.3f, 0.3f, 0.3f };
	diffuse = { 0.0f, 0.0f, 0.0f };
	specular = { 0.0f, 0.0f, 0.0f };
	alpha = 1.0f;
	auto dev = DirectXLib::GetInstance()->GetDevice();

	// �萔�o�b�t�@�̐���
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// �A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff)&~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));
	if (FAILED(result)) {
		assert(0);
	}
}

void Material::LoadTexture()
{
	//�e�N�X�`�������̏ꍇ�̓e�N�X�`�������p�̔����摜��K�p
	if (textureFileName.size() == 0)
		textureFileName = "noTex.png";
	//���Ƀe�N�X�`�����ǂݍ��܂�Ă���ꍇ�͑��I��
	if (Texture::GetTexBuff(textureFileName) != nullptr)
		return;

	HRESULT result = S_FALSE;

	Texture::LoadTexture(textureFileName, textureFileName);	
}

void Material::Update()
{
	HRESULT result;
	// �萔�o�b�t�@�փf�[�^�]��
	ConstBufferData* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result)) {
		constMap->ambient = ambient;
		constMap->diffuse = diffuse;
		constMap->specular = specular;
		constMap->alpha = alpha;
		constBuff->Unmap(0, nullptr);
	}

}
