#include "Object3D.h"
#include"DirectXLib.h"
#include "FBXModel.h"
#include "Texture.h"
Camera* Object3D::camera = nullptr;
LightCamera* Object3D::lightCamera = nullptr;
bool Object3D::drawShadow = false;
LightGroup* Object3D::lightGroup = nullptr;
int Object3D::bbIndex = 0;
XMMATRIX Object3D::lightViewProjection = {};
Object3D * Object3D::Create(Model* model,Vector3& pos, Vector3& scale, Vector3& rot, XMFLOAT4& color)
{
	Object3D* object = new Object3D(pos, scale, rot, color);
	if (object == nullptr) {
		return nullptr;
	}

	// ������
	if (!object->Initialize()) {
		delete object;
		assert(0);
	}

	if (model) {
		object->SetModel(model);
	}

	return object;
}

void Object3D::SetCamera(Camera * camera)
{
	Object3D::camera = camera;
}

void Object3D::ClucLightViewProjection()
{
	lightViewProjection = XMMatrixLookAtLH(Vector3(lightCamera->GetEye()).ConvertXMVECTOR(), Vector3(lightCamera->GetTarget()).ConvertXMVECTOR(), Vector3(lightCamera->GetUp()).ConvertXMVECTOR()) * XMMatrixOrthographicLH(100, 100, 1.0f, 1000.0f);
}

Object3D::Object3D(Vector3 & pos, Vector3 & scale, Vector3 & rot, XMFLOAT4 & color)
	:position(pos),scale(scale),rotation(rot),color(color)
{
}

bool Object3D::Initialize()
{
	HRESULT result;
	for (int i = 0; i < 3; i++)
	{
		// �萔�o�b�t�@�̐���
		result = DirectXLib::GetInstance()->GetDevice()->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// �A�b�v���[�h�\
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constBuff[i]));
		assert(SUCCEEDED(result));

		// �萔�o�b�t�@�̐���
		result = DirectXLib::GetInstance()->GetDevice()->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// �A�b�v���[�h�\
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer((sizeof(LightCameraCBData) + 0xff) & ~0xff),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&lCameraConstBuff[i]));
		assert(SUCCEEDED(result));

		result = DirectXLib::GetInstance()->GetDevice()->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// �A�b�v���[�h�\
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstLightCameraBuff) + 0xff) & ~0xff),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constCameraBuff[i]));
		assert(SUCCEEDED(result));
	}
	return true;
}

void Object3D::Update(const BILLBOARD_TYPE billboardType)
{
	XMMATRIX matScale, matRot, matTrans;
	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
	matTrans = XMMatrixTranslation(position.x, position.y, position.z);

	//���[���h�s��̍���
	matWorld = XMMatrixIdentity();
	matWorld *= matScale;
	matWorld *= matRot;
	switch (billboardType)
	{
	case NORMAL:
	{
		const XMMATRIX& matBillboard = camera->GetMatBillboard();
		matWorld *= matBillboard;
		break;
	}
	case Y_AXIS:
	{
		const XMMATRIX& matBillboardY = camera->GetMatBillboardY();
		matWorld *= matBillboardY;
		break;
	}
	default:
		break;
	}
	matWorld *= matTrans;
	// �e�I�u�W�F�N�g�������
	if (parent != nullptr) {
		// �e�I�u�W�F�N�g�̃��[���h�s����|����
		matWorld *= parent->matWorld;
	}	
}

void Object3D::WorldUpdate(const Vector3& rot,const BILLBOARD_TYPE billboardType)
{
	XMMATRIX matScale, matTrans, matRot;
	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	XMMATRIX camMatWorld = XMMatrixInverse(nullptr, camera->GetMatView());

	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));

	appendRot *= XMMatrixRotationAxis(camMatWorld.r[2], rot.z);
	appendRot *= XMMatrixRotationAxis(camMatWorld.r[0], rot.x);
	appendRot *= XMMatrixRotationAxis(camMatWorld.r[1], rot.y);

	matTrans = XMMatrixTranslation(position.x, position.y, position.z);

	//���[���h�s��̍���
	matWorld = XMMatrixIdentity();
	matWorld *= matScale;
	//XMMATRIX matRot2 = XMMatrixIdentity();
	//matRot2 *= XMMatrixRotationY(XMConvertToRadians(90));
	//matWorld *= matRot2;
	//matWorld *= matRot;
	switch (billboardType)
	{
	case NORMAL:
	{
		const XMMATRIX& matBillboard = camera->GetMatBillboard();
		matWorld *= matBillboard;
		break;
	}
	case Y_AXIS:
	{
		const XMMATRIX& matBillboardY = camera->GetMatBillboardY();
		matWorld *= matBillboardY;
		break;
	}
	default:
		break;
	}
	matWorld *= matTrans;
	// �e�I�u�W�F�N�g�������
	if (parent != nullptr) {
		// �e�I�u�W�F�N�g�̃��[���h�s����|����
		matWorld *= parent->matWorld;
	}
}

const XMMATRIX& Object3D::GetRotateMat()
{
	XMMATRIX matRot;
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(-rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(-rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(-rotation.y));
	return matRot;
}

void Object3D::Draw(const bool fbx, const bool shade, BLENDTYPE type, const bool customPipeline,const int lightRootParameterIndex)
{
	HRESULT result;
	const XMMATRIX& matViewProjection = camera->GetMatViewProjection();
	const XMFLOAT3& cameraPos = camera->GetEye();
	auto cmdList = DirectXLib::GetInstance()->GetCommandList();
	if (drawShadow)
	{
		// �萔�o�b�t�@�փf�[�^�]��
		LightCameraCBData* constMap = nullptr;
		result = lCameraConstBuff[bbIndex]->Map(0, nullptr, (void**)&constMap);
		assert(SUCCEEDED(result));
		constMap->viewprojection = lightViewProjection;
		constMap->world = matWorld;
		lCameraConstBuff[bbIndex]->Unmap(0, nullptr);

	}
	else
	{
		// �萔�o�b�t�@�փf�[�^�]��
		ConstBufferData* constMap = nullptr;
		result = constBuff[bbIndex]->Map(0, nullptr, (void**)&constMap);
		assert(SUCCEEDED(result));
		//constMap->mat = matWorld * matViewProjection;	// �s��̍���
		constMap->viewprojection = matViewProjection;
		constMap->lightViewProjection = lightCamera->GetMatViewProjection();
		constMap->cameraPos = cameraPos;
		constMap->world = matWorld;
		constMap->color = color;
		constBuff[bbIndex]->Unmap(0, nullptr);

		
		ConstLightCameraBuff* constMap2 = nullptr;
		auto result = constCameraBuff[bbIndex]->Map(0, nullptr, (void**)&constMap2);
		assert(SUCCEEDED(result));
		constMap2->cameraPos = lightCamera->GetEye();
		constMap2->viewProjection = lightViewProjection;

		constCameraBuff[bbIndex]->Unmap(0, nullptr);

	}
	if (!fbx && !customPipeline && !drawShadow && shade)
	{
		cmdList->SetGraphicsRootConstantBufferView(4, constCameraBuff[bbIndex]->GetGPUVirtualAddress());
		cmdList->SetGraphicsRootDescriptorTable(5, Texture::GetGpuDescHandleSRV("shadowMap" + std::to_string(bbIndex)));  //�q�[�v�̐擪���萔�o�b�t�@
	}

	if (drawShadow)
		// �萔�o�b�t�@�r���[���Z�b�g
		cmdList->SetGraphicsRootConstantBufferView(0, lCameraConstBuff[bbIndex]->GetGPUVirtualAddress());
	else
		cmdList->SetGraphicsRootConstantBufferView(0, constBuff[bbIndex]->GetGPUVirtualAddress());
	if (!drawShadow && shade)
		//���C�g�̕`��
		lightGroup->Draw(lightRootParameterIndex);
	//���f���̕`��
	model->Draw();
}

