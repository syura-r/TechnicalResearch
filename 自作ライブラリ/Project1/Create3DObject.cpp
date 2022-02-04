#include "Create3DObject.h"
#include<math.h>
#include<d3dx12.h>


Create3DObject::Create3DObject()
{
}


Create3DObject::~Create3DObject()
{
}

void Create3DObject::CreatePyramid(const int vertex, const float topHeight, const int R, XMFLOAT3 scale, XMFLOAT3 rot, XMFLOAT3 trans, bool projective, XMFLOAT4 color, ID3D12Resource* texbuff, TexMetadata& metadata)
{
	HRESULT result;
	ID3D12Device * dev = DirectXLib::GetInstance()->GetDevice();
	baseColor = color;
	basePosition = trans;
	this->vertex = vertex;
	this->r = topHeight / 2.0f;
	for (int i = 0; i < vertex; i++)
	{
		vertices[i].pos.x = R * sinf((XM_2PI / (vertex - 1)) * i) /** ((float)window_height / (float)window_width)*/;
		vertices[i].pos.y = R * cosf((XM_2PI / (vertex - 1)) * i);
		vertices[i].pos.z = 0.0f;
	}
	vertices[vertex - 1].pos = { 0,0,0 };
	vertices[vertex].pos = { 0,0,-topHeight };
	XMFLOAT2 uv = { 0,0 };
	for (int i = 0; i < vertex + 1; i++)
	{
		uv.x = (1.0f / vertex)*i;
		uv.y = (1.0f / vertex)*i;
		vertices[i].uv = uv;
	}

	for (int i = 0; i < (vertex - 1) * 2; i++)
	{
		int k = i * 3;
		if (i < vertex - 1)
		{
			if (i == vertex - 2)
			{
				indices[k] = 0;
				indices[k + 1] = vertex - 2;
			}
			else
			{
				indices[k] = i + 1;
				indices[k + 1] = i;
			}
			indices[k + 2] = vertex - 1;
		}
		else
		{
			indices[k] = i - vertex + 1;
			if (i == (vertex - 1) * 2 - 1)
			{
				indices[k + 1] = 0;
			}
			else
			{
				indices[k + 1] = indices[k] + 1;
			}
			indices[k + 2] = vertex;
		}
	}
	for (int i = 0; i < (vertex -1)*2; i++)
	{
		//�O�p�`1���Ɍv�Z���Ă���
		//�O�p�`�̃C���f�b�N�X�����o���āA�ꎞ�I�ȕϐ��ɓ����
		unsigned short index0 = indices[i * 3 + 0];
		unsigned short index1 = indices[i * 3 + 1];
		unsigned short index2 = indices[i * 3 + 2];
		//�O�p�`���\�����钸�_���W���x�N�g���ɑ��
		XMVECTOR p0 = XMLoadFloat3(&vertices[index0].pos);
		XMVECTOR p1 = XMLoadFloat3(&vertices[index1].pos);
		XMVECTOR p2 = XMLoadFloat3(&vertices[index2].pos);
		//p0->p1�x�N�g��,p0->p2�x�N�g�����v�Z(�x�N�g���̌��Z)
		XMVECTOR v1 = XMVectorSubtract(p1, p0);
		XMVECTOR v2 = XMVectorSubtract(p2, p0);
		//�O�ς͗������琂���ȃx�N�g��
		XMVECTOR normal = XMVector3Cross(v1, v2);
		//���K��(����1�ɂ���)
		normal = XMVector3Normalize(normal);
		//���߂��@���𒸓_�f�[�^�ɑ��
		XMStoreFloat3(&vertices[index0].normal, normal);
		XMStoreFloat3(&vertices[index1].normal, normal);
		XMStoreFloat3(&vertices[index2].normal, normal);
	}
	matWorld = XMMatrixIdentity();
	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matWorld *= matScale;//���[���h�s��ɃX�P�[�����O�𔽉f
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationY(XMConvertToRadians(rot.y));//y�������45�x��]
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rot.z));//Z�������45�x��]
	matRot *= XMMatrixRotationX(XMConvertToRadians(rot.x));//x�������45�x��]
	matWorld *= matRot;//���[���h�s��ɉ�]�𔽉f
	matTrans = XMMatrixTranslation(trans.x, trans.y, trans.z); //(50,0,0)���s�ړ�
	matWorld *= matTrans;//���[���h�s��ɕ��s�ړ��𔽉f
	this->position = trans;
	this->rotation = rot;
	baseRot = rot;
	baseScale = matScale;

	D3D12_HEAP_PROPERTIES heapprop{};//���_�q�[�v�ݒ�
	heapprop.Type = D3D12_HEAP_TYPE_UPLOAD;//GPU�ւ̓]���p

	D3D12_RESOURCE_DESC resdesc{};  //���\�[�X�ݒ�
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = sizeof(vertices); //���_��񂪓��镪�̃T�C�Y
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.SampleDesc.Count = 1;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//GPU���\�[�X�̐���
	result = dev->CreateCommittedResource(
		&heapprop, //�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resdesc,//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));

	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(vertices);
	vbView.StrideInBytes = sizeof(vertices[0]);


	//�ݒ�\����
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;  //�V�F�[�_�[���猩����
	descHeapDesc.NumDescriptors = 3;  //SRV1��CBV2��
	 //����
	result = dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&basicDescHeap));

	//�f�X�N���v�^�q�[�v�̐擪�n���h�����擾���Ă���
	D3D12_CPU_DESCRIPTOR_HANDLE basicHeapHandle = basicDescHeap->GetCPUDescriptorHandleForHeapStart();

	D3D12_HEAP_PROPERTIES cbheapprop{};   //�q�[�v�ݒ�
	cbheapprop.Type = D3D12_HEAP_TYPE_UPLOAD;    //GPU�ւ̓]���p

	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC depthResDesc{};
	depthResDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthResDesc.Width = window_width;//�����_�[�^�[�Q�b�g�ɍ��킹��
	depthResDesc.Height = window_height;//�����_�[�^�[�Q�b�g�ɍ��킹��
	depthResDesc.DepthOrArraySize = 1;
	depthResDesc.Format = DXGI_FORMAT_D32_FLOAT;//�[�x�t�H�[�}�b�g
	depthResDesc.SampleDesc.Count = 1;
	depthResDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;//�f�v�X�X�e���V��
	//�[�x�l�p�q�[�v�v���p�e�B
	D3D12_HEAP_PROPERTIES depthHeapProp{};
	depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
	//�[�x�l�̃N���A�ݒ�
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f;//�[�x�l1.0f(�ő�l)�ŃN���A
	depthClearValue.Format = DXGI_FORMAT_D32_FLOAT;//�[�x�l�t�H�[�}�b�g

	//���\�[�X�ݒ�
	result = dev->CreateCommittedResource(
		&depthHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,//�[�x�l�������݂Ɏg�p
		&depthClearValue,
		IID_PPV_ARGS(&depthBuffer));

	//�[�x�r���[�p�f�X�N���v�^�q�[�v�쐬
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1;//�[�x�r���[��1��
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;//�f�v�X�X�e���V���r���[
	ComPtr<ID3D12DescriptorHeap> dsvHeap;
	result = dev->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));
	//�[�x�r���[�쐬
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;//�[�x�l�t�H�[�}�b�g
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dev->CreateDepthStencilView(
		depthBuffer.Get(),
		&dsvDesc,
		dsvHeap->GetCPUDescriptorHandleForHeapStart());



	D3D12_RESOURCE_DESC cbresdesc{};		//���\�[�X�ݒ�
	cbresdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbresdesc.Width = (sizeof(ConstBufferDate) + 0xff)&~0xff; //256�o�C�g�A���C�������g
	cbresdesc.Height = 1;
	cbresdesc.DepthOrArraySize = 1;
	cbresdesc.MipLevels = 1;
	cbresdesc.SampleDesc.Count = 1;
	cbresdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//GPU���\�[�X�̐���
	result = dev->CreateCommittedResource(
		&cbheapprop,//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&cbresdesc,//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));


	result = constBuff->Map(0, nullptr, (void**)&constMap);  //�}�b�s���O
	constMap->color = color;		//RGBA�ŊD�F
	//�������e
	if (projective == true)
	{
		matProjection = XMMatrixPerspectiveFovLH(
			XMConvertToRadians(60.0f),
			(float)window_width / window_height,
			0.1f, 1000.0f
		);
	}
	else
	{
		matProjection = XMMatrixOrthographicOffCenterLH(
			-100, 100, -60, 60, 0.1f, 1000.0f
		);
	}
	constMap->mat = matWorld * camera->GetMatView() * matProjection;
	constBuff->Unmap(0, nullptr);		//�}�b�s���O����




	//�萔�o�b�t�@�r���[�̍쐬
	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc{};
	cbvDesc.BufferLocation = constBuff->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = (UINT)constBuff->GetDesc().Width;
	//dev->CreateConstantBufferView(
	//	&cbvDesc, basicDescHeap->GetCPUDescriptorHandleForHeapStart());
	dev->CreateConstantBufferView(&cbvDesc, basicHeapHandle);
	//������Ԃ񂾂��A�h���X�����炷
	basicHeapHandle.ptr += dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//�V�F�[�_���\�[�X�r���[�쐬
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};//�ݒ�\����
	srvDesc.Format = metadata.format;//RGBA
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = 1;

	dev->CreateShaderResourceView(texbuff,//�r���[�Ɗ֘A�t����o�b�t�@
		&srvDesc,//�e�N�X�`���ݒ���
		basicHeapHandle
	);

	//�C���f�b�N�X�o�b�t�@�̊m��
	resdesc.Width = sizeof(indices);//�C���f�b�N�X��񂪓��镪�̃T�C�Y
	//GPU���\�[�X�̐���
	result = dev->CreateCommittedResource(
		&heapprop,//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resdesc,//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));
	//GPU��̃o�b�t�@�ɑΉ��������z���������擾
	unsigned short* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	//�S�C���f�b�N�X�ɑ΂���
	for (int i = 0; i < _countof(indices); i++)
	{
		indexMap[i] = indices[i];  //�C���f�b�N�X���R�s�[
	}
	indexBuff->Unmap(0, nullptr);

	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeof(indices);

	//�f�X�N���v�^�q�[�v�̐擪�A�h���X(GPU)���L�^
	D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandleStart = basicDescHeap->GetGPUDescriptorHandleForHeapStart();
	//�f�X�N���v�^1���̃o�b�t�@�T�C�Y���擾
	UINT descHandleIncrementSize = dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	//CBV1��
	gpuDescHandleCBV = gpuDescHandleStart;
	gpuDescHandleCBV.ptr += descHandleIncrementSize * 0;
	//SRV
	gpuDescHandleSRV = gpuDescHandleStart;
	gpuDescHandleSRV.ptr += descHandleIncrementSize * 1;


}

void Create3DObject::CreateBox(const int vertex, const float TopHeight, const int Width, XMFLOAT3 scale, XMFLOAT3 rot, XMFLOAT3 trans, bool projective, XMFLOAT4 color, ID3D12Resource * texbuff, TexMetadata & metadata)
{
	ID3D12Device * dev = DirectXLib::GetInstance()->GetDevice();
	HRESULT result;
	baseColor = color;
	this->vertex = vertex;
	this->width = Width;
	this->height = Width;
	this->r = (float)Width;
		int radius = (int)Width / (int)sin(XM_2PI / vertex);
		for (int i = 0; i < vertex; i++)
		{
			vertices[i].pos.x = radius * sinf((XM_2PI / vertex) * i);
			vertices[i].pos.y = radius * cosf((XM_2PI / vertex) * i);
			vertices[i].pos.z = TopHeight * 0.5f;
		}
		vertices[vertex].pos = { 0,0,TopHeight * 0.5f };
		for (int i = vertex + 1; i < (vertex + 1) * 2 - 1; i++)
		{
			vertices[i].pos.x = radius * sinf((XM_2PI / vertex) * (i - vertex - 1));
			vertices[i].pos.y = radius * cosf((XM_2PI / vertex) * (i - vertex - 1));
			vertices[i].pos.z = -TopHeight * 0.5f;
		}
		vertices[(vertex + 1) * 2 - 1].pos = { 0,0,-TopHeight * 0.5f };
		XMFLOAT2 uv = { 0,0 };
		for (int i = 0; i < (vertex + 1)*2; i++)
		{
			uv.x = (1.0f / vertex)*i;
			uv.y = (1.0f / vertex)*i;
			vertices[i].uv = uv;
		}
		int k = 0;
		for (int i = 0; i < vertex * 2; i++)
		{
			if (i <= vertex - 1)
			{
				if (i == vertex - 1)
				{
					indices[k] = 0;
					indices[k + 1] = vertex - 1;
				}
				else
				{
					indices[k] = i + 1;
					indices[k + 1] = i;
				}
				indices[k + 2] = vertex;
			}
			else if (i <= vertex * 2 - 1)
			{
				if (i == vertex * 2 - 1)
				{
					indices[k] = (vertex + 1) * 2 - 2;
					indices[k + 1] = vertex + 1;
				}
				else
				{
					indices[k] = i + 1;
					indices[k + 1] = i + 2;
				}
				indices[k + 2] = (vertex + 1) * 2 - 1;
			}
			k += 3;
		}
		for (int i = 0; i < vertex; i++)
		{
			if (i == vertex - 1)
			{
				indices[k] = 0;
				indices[k + 1] = vertex + 1;
				indices[k + 2] = vertex - 1;
				indices[k + 3] = vertex + 1;
				indices[k + 4] = (vertex + 1) * 2 - 2;
				indices[k + 5] = vertex - 1;
			}
			else
			{
				indices[k] = vertex + 1 + i;
				indices[k + 1] = i;
				indices[k + 2] = vertex + 2 + i;
				indices[k + 3] = i;
				indices[k + 4] = i + 1;
				indices[k + 5] = vertex + 2 + i;
				k += 6;
			}
		}
	//}
	for (int i = 0; i < vertex * 2 * 2; i++)
	{
		//�O�p�`1���Ɍv�Z���Ă���
		//�O�p�`�̃C���f�b�N�X�����o���āA�ꎞ�I�ȕϐ��ɓ����
		unsigned short index0 = indices[i * 3 + 0];
		unsigned short index1 = indices[i * 3 + 1];
		unsigned short index2 = indices[i * 3 + 2];
		//�O�p�`���\�����钸�_���W���x�N�g���ɑ��
		XMVECTOR p0 = XMLoadFloat3(&vertices[index0].pos);
		XMVECTOR p1 = XMLoadFloat3(&vertices[index1].pos);
		XMVECTOR p2 = XMLoadFloat3(&vertices[index2].pos);
		//p0->p1�x�N�g��,p0->p2�x�N�g�����v�Z(�x�N�g���̌��Z)
		XMVECTOR v1 = XMVectorSubtract(p1, p0);
		XMVECTOR v2 = XMVectorSubtract(p2, p0);
		//�O�ς͗������琂���ȃx�N�g��
		XMVECTOR normal = XMVector3Cross(v1, v2);
		//���K��(����1�ɂ���)
		normal = XMVector3Normalize(normal);
		//���߂��@���𒸓_�f�[�^�ɑ��
		XMStoreFloat3(&vertices[index0].normal, normal);
		XMStoreFloat3(&vertices[index1].normal, normal);
		XMStoreFloat3(&vertices[index2].normal, normal);
	}
	matWorld = XMMatrixIdentity();
	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matWorld *= matScale;//���[���h�s��ɃX�P�[�����O�𔽉f
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationY(XMConvertToRadians(rot.y));//y�������45�x��]
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rot.z));//Z�������45�x��]
	matRot *= XMMatrixRotationX(XMConvertToRadians(rot.x));//x�������45�x��]
	matWorld *= matRot;//���[���h�s��ɉ�]�𔽉f
	matTrans = XMMatrixTranslation(trans.x, trans.y, trans.z); //(50,0,0)���s�ړ�
	matWorld *= matTrans;//���[���h�s��ɕ��s�ړ��𔽉f
	this->position = trans;
	this->rotation = rot;

	D3D12_HEAP_PROPERTIES heapprop{};//���_�q�[�v�ݒ�
	heapprop.Type = D3D12_HEAP_TYPE_UPLOAD;//GPU�ւ̓]���p

	D3D12_RESOURCE_DESC resdesc{};  //���\�[�X�ݒ�
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = sizeof(vertices); //���_��񂪓��镪�̃T�C�Y
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.SampleDesc.Count = 1;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//GPU���\�[�X�̐���
	result = dev->CreateCommittedResource(
		&heapprop, //�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resdesc,//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));

	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(vertices);
	vbView.StrideInBytes = sizeof(vertices[0]);


	//�ݒ�\����
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;  //�V�F�[�_�[���猩����
	descHeapDesc.NumDescriptors = 3;  //SRV1��CBV2��
	 //����
	result = dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&basicDescHeap));

	//�f�X�N���v�^�q�[�v�̐擪�n���h�����擾���Ă���
	D3D12_CPU_DESCRIPTOR_HANDLE basicHeapHandle = basicDescHeap->GetCPUDescriptorHandleForHeapStart();

	D3D12_HEAP_PROPERTIES cbheapprop{};   //�q�[�v�ݒ�
	cbheapprop.Type = D3D12_HEAP_TYPE_UPLOAD;    //GPU�ւ̓]���p

	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC depthResDesc{};
	depthResDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthResDesc.Width = window_width;//�����_�[�^�[�Q�b�g�ɍ��킹��
	depthResDesc.Height = window_height;//�����_�[�^�[�Q�b�g�ɍ��킹��
	depthResDesc.DepthOrArraySize = 1;
	depthResDesc.Format = DXGI_FORMAT_D32_FLOAT;//�[�x�t�H�[�}�b�g
	depthResDesc.SampleDesc.Count = 1;
	depthResDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;//�f�v�X�X�e���V��
	//�[�x�l�p�q�[�v�v���p�e�B
	D3D12_HEAP_PROPERTIES depthHeapProp{};
	depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
	//�[�x�l�̃N���A�ݒ�
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f;//�[�x�l1.0f(�ő�l)�ŃN���A
	depthClearValue.Format = DXGI_FORMAT_D32_FLOAT;//�[�x�l�t�H�[�}�b�g

	//���\�[�X�ݒ�
	result = dev->CreateCommittedResource(
		&depthHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,//�[�x�l�������݂Ɏg�p
		&depthClearValue,
		IID_PPV_ARGS(&depthBuffer));

	//�[�x�r���[�p�f�X�N���v�^�q�[�v�쐬
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1;//�[�x�r���[��1��
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;//�f�v�X�X�e���V���r���[
	ComPtr<ID3D12DescriptorHeap> dsvHeap;
	result = dev->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));
	//�[�x�r���[�쐬
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;//�[�x�l�t�H�[�}�b�g
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dev->CreateDepthStencilView(
		depthBuffer.Get(),
		&dsvDesc,
		dsvHeap->GetCPUDescriptorHandleForHeapStart());



	D3D12_RESOURCE_DESC cbresdesc{};		//���\�[�X�ݒ�
	cbresdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbresdesc.Width = (sizeof(ConstBufferDate) + 0xff)&~0xff; //256�o�C�g�A���C�������g
	cbresdesc.Height = 1;
	cbresdesc.DepthOrArraySize = 1;
	cbresdesc.MipLevels = 1;
	cbresdesc.SampleDesc.Count = 1;
	cbresdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//GPU���\�[�X�̐���
	result = dev->CreateCommittedResource(
		&cbheapprop,//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&cbresdesc,//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));


	result = constBuff->Map(0, nullptr, (void**)&constMap);  //�}�b�s���O
	constMap->color = color;		//RGBA�ŊD�F
	//�������e
	if (projective == true)
	{
		matProjection = XMMatrixPerspectiveFovLH(
			XMConvertToRadians(60.0f),
			(float)window_width / window_height,
			0.1f, 1000.0f
		);
	}
	else
	{
		matProjection = XMMatrixOrthographicOffCenterLH(
			-100, 100, -60, 60, 0.1f, 1000.0f
		);
	}
	constMap->mat = matWorld * camera->GetMatView() * matProjection;
	constBuff->Unmap(0, nullptr);		//�}�b�s���O����




	//�萔�o�b�t�@�r���[�̍쐬
	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc{};
	cbvDesc.BufferLocation = constBuff->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = (UINT)constBuff->GetDesc().Width;
	//dev->CreateConstantBufferView(
	//	&cbvDesc, basicDescHeap->GetCPUDescriptorHandleForHeapStart());
	dev->CreateConstantBufferView(&cbvDesc, basicHeapHandle);
	//������Ԃ񂾂��A�h���X�����炷
	basicHeapHandle.ptr += dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//�V�F�[�_���\�[�X�r���[�쐬
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};//�ݒ�\����
	srvDesc.Format = metadata.format;//RGBA
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = 1;

	dev->CreateShaderResourceView(texbuff,//�r���[�Ɗ֘A�t����o�b�t�@
		&srvDesc,//�e�N�X�`���ݒ���
		basicHeapHandle
	);

	//�C���f�b�N�X�o�b�t�@�̊m��
	resdesc.Width = sizeof(indices);//�C���f�b�N�X��񂪓��镪�̃T�C�Y
	//GPU���\�[�X�̐���
	result = dev->CreateCommittedResource(
		&heapprop,//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resdesc,//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));
	//GPU��̃o�b�t�@�ɑΉ��������z���������擾
	unsigned short* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	//�S�C���f�b�N�X�ɑ΂���
	for (int i = 0; i < _countof(indices); i++)
	{
		indexMap[i] = indices[i];  //�C���f�b�N�X���R�s�[
	}
	indexBuff->Unmap(0, nullptr);

	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeof(indices);

	//�f�X�N���v�^�q�[�v�̐擪�A�h���X(GPU)���L�^
	D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandleStart = basicDescHeap->GetGPUDescriptorHandleForHeapStart();
	//�f�X�N���v�^1���̃o�b�t�@�T�C�Y���擾
	UINT descHandleIncrementSize = dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	//CBV1��
	gpuDescHandleCBV = gpuDescHandleStart;
	gpuDescHandleCBV.ptr += descHandleIncrementSize * 0;
	//SRV
	gpuDescHandleSRV = gpuDescHandleStart;
	gpuDescHandleSRV.ptr += descHandleIncrementSize * 1;

}

void Create3DObject::DrawPyramid( XMFLOAT4 color,ID3D12PipelineState * pipelinestate, ID3D12RootSignature * rootsignature)
{
	ID3D12GraphicsCommandList * cmdList = DirectXLib::GetInstance()->GetCommandList();
	HRESULT result;
	baseColor = color;
	if (isDamege)
	{
		damegeCounter--;
		if (damegeCounter <= 0)
		{
			damegeCounter = 20;
			isDamege = false;
		}
		constMap->color = XMFLOAT4(1.0f, 0.0f, 0.0f, color.w);
	}
	else
	{
		constMap->color = color;
	}

	result = vertBuff->Map(0, nullptr, (void**)&vertMap);

	//�S���_�ɑ΂���
	for (int i = 0; i < _countof(vertices); i++)
	{
		vertMap[i] = vertices[i];
	}
	//�}�b�v��������
	vertBuff->Unmap(0, nullptr);
	ID3D12DescriptorHeap* ppHeaps[] = { basicDescHeap.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	cmdList->IASetVertexBuffers(0, 1, &vbView);

	cmdList->SetPipelineState(pipelinestate);
	cmdList->SetGraphicsRootSignature(rootsignature);
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	cmdList->IASetIndexBuffer(&ibView);

	//�萔�o�b�t�@�r���[�R�}���h���Z�b�g
	cmdList->SetGraphicsRootDescriptorTable(0, gpuDescHandleCBV);
	//�V�F�[�_���\�[�X�r���[���Z�b�g
	cmdList->SetGraphicsRootDescriptorTable(1, gpuDescHandleSRV);

	cmdList->DrawIndexedInstanced((vertex - 1) * 6, 1, 0, 0, 0);

}

void Create3DObject::DrawBox(XMFLOAT4 color,ID3D12PipelineState * pipelinestate, ID3D12RootSignature * rootsignature)
{
	ID3D12GraphicsCommandList * cmdList = DirectXLib::GetInstance()->GetCommandList();
	baseColor = color;
	HRESULT result;
	if (isDamege)
	{
		damegeCounter--;
		if (damegeCounter <= 0)
		{
			damegeCounter = 20;
			isDamege = false;
		}
		constMap->color = XMFLOAT4(1.0f, 0.0f, 0.0f, color.w);
	}
	else
	{
		constMap->color = color;
	}

	result = vertBuff->Map(0, nullptr, (void**)&vertMap);

	//�S���_�ɑ΂���
	for (int i = 0; i < _countof(vertices); i++)
	{
		vertMap[i] = vertices[i];
	}
	//�}�b�v��������
	vertBuff->Unmap(0, nullptr);
	ID3D12DescriptorHeap* ppHeaps[] = { basicDescHeap.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	cmdList->IASetVertexBuffers(0, 1, &vbView);

	cmdList->SetPipelineState(pipelinestate);
	cmdList->SetGraphicsRootSignature(rootsignature);
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	cmdList->IASetIndexBuffer(&ibView);

	//�萔�o�b�t�@�r���[�R�}���h���Z�b�g
	cmdList->SetGraphicsRootDescriptorTable(0, gpuDescHandleCBV);
	//�V�F�[�_���\�[�X�r���[���Z�b�g
	cmdList->SetGraphicsRootDescriptorTable(1, gpuDescHandleSRV);

	cmdList->DrawIndexedInstanced(vertex  * 12, 1, 0, 0, 0);

}

void Create3DObject::Move()
{
	//if (input->DownKey(DIK_UP))
	//{
	//	position.y += 1.0f;
	//}
	//else if (input->DownKey(DIK_DOWN))
	//{
	//	position.y -= 1.0f;
	//}
	////���W���ړ����鏈��(X���W)
	//if (input->DownKey(DIK_RIGHT))
	//{
	//	position.x += 1.0f;
	//}
	//else if (input->DownKey(DIK_LEFT))

	//{
	//	position.x -= 1.0f;
	//}

	//if (input->DownKey(DIK_W))
	//{
	//	rotation.x += 2;
	//}
	//else if (input->DownKey(DIK_S))
	//{
	//	rotation.x -= 2;
	//}
	//if (input->DownKey(DIK_D))
	//{
	//	rotation.y += 2;
	//}
	////if (input->DownPad(JOY_BUTTON1))
	////{
	////	rotation.y += 2;
	////}

	//else if (input->DownKey(DIK_A))
	//{
	//	rotation.y -= 2;
	//}
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));//z������ɉ�]
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));//x������ɉ�]
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));//y������ɉ�]

	matTrans = XMMatrixTranslation(position.x, position.y, position.z);//���s�ړ��s������W����Čv�Z
	matWorld = XMMatrixIdentity();//���[���h�s��͖��t���[�����Z�b�g
	matWorld *= matRot;
	matWorld *= matTrans;
	constMap->mat = matWorld * camera->GetMatView() * matProjection;
}

void Create3DObject::Update()
{
}

void Create3DObject::SetDirection()
{
}



XMFLOAT3 Create3DObject::GetPosition()
{
	return position;
}

//void Create3DObject::Hit(Create3DObject * object)
//{
//	float length = sqrt(((position.x - object->position.x)*(position.x - object->position.x)) + ((position.y - r - object->position.y - object->r)*(position.y - r - object->position.y - object->r)));
//	if (length <= r + object->r)
//	{
//		object->Damege();
//		Damege();
//}

void Create3DObject::Damege()
{
	hp -= 10;
	if (hp <= 0)
	{
		isDead = true;
	}
	isDamege = true;
}

bool Create3DObject::GetIsDead()
{
	return isDead;
}

void Create3DObject::Die()
{
	isDead = true;
}

float Create3DObject::GetR()
{
	return r;
}

int Create3DObject::GetWidth()
{
	return width;
}

