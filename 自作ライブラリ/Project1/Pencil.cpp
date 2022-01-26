#include "Pencil.h"

#include "BoxCollider.h"
#include "CollisionAttribute.h"
#include "CollisionManager.h"
#include "Easing.h"
#include "OBJLoader.h"
#include "ParticleEmitter.h"
#include "PtrDelete.h"

Pencil::Pencil(const Vector3& pos)
{
	//�G�t�F�N�g�����̍쐬
	Create(OBJLoader::GetModel("Area"));

	position = pos;
	scale = { 0.5f,0.6f,0.5f };
	color = { 0.3f,0.9f,0.1f,1 };
	//���M�̍쐬
	pencil = new Object();
	pencil->Create(OBJLoader::GetModel("pencil"));
	pencilPosition = pos + Vector3{ 0,0.7f,0 };
	BasePencilPosY = pos.y + 0.8f;
	pencilMoveCounter = 30;
	pencilRotation = { 0,0,20 };
	pencil->SetPosition(pencilPosition);
	pencil->SetRotation(pencilRotation);
	pencil->SetScale({ 3,2.3f,3 });
	HRESULT result;
	for (int i = 0; i < 3; i++)
	{
		result = DirectXLib::GetInstance()->GetDevice()->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// �A�b�v���[�h�\
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBuffDeta) + 0xff) & ~0xff),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constBuff[i]));
		assert(SUCCEEDED(result));
	}
	offsetTime = 0;
	object->Update();
	for (int i = 0; i < 3; i++)
	{
		colliders[i] = new BoxCollider({ 0, 0.65f, 0,0 }, Vector3{ 0.25f,0.65f,0.25f });
		colliders[i]->SetRotation({ 0,30.0f * i,0 });
		colliders[i]->SetObject(this);
		colliders[i]->SetAttribute(COLLISION_ATTR_ITEM);
		colliders[i]->Update();
		CollisionManager::GetInstance()->AddCollider(colliders[i]);

	}

}

Pencil::~Pencil()
{
	PtrDelete(pencil);
	Object::~Object();
}
void Pencil::Initialize()
{
	pencilMoveCounter = 30;
	get = false;
	getFlame = false;
}

void Pencil::Update()
{
	//�l����ԂȂ瑁�����^�[��
	if (get)
	{
		getFlame = false;
		return;
	}
	//�G�t�F�N�g��UV�X�N���[���̍X�V
	offsetTime += 0.01f;
	if (offsetTime > 1.0f)
		offsetTime = 0;

	//���M�̉�]
	pencilRotation.y += 0.3f;
	//���M�㏸����
	if (pencilMoveCounter <= PencilEasingTime)
		pencilPosition.y = Easing::EaseInOutQuad(BasePencilPosY - 0.2f, BasePencilPosY + 0.2f, PencilEasingTime, pencilMoveCounter);
	//���M�~������
	else if (pencilMoveCounter <= PencilEasingTime*2)
		pencilPosition.y = Easing::EaseInOutQuad(BasePencilPosY + 0.2f, BasePencilPosY - 0.2f, PencilEasingTime, pencilMoveCounter - PencilEasingTime);
	pencilMoveCounter++;
	if (pencilMoveCounter > PencilEasingTime*2)
		pencilMoveCounter = 0;

	//���M�̍X�V
	pencil->SetPosition(pencilPosition);
	pencil->SetRotation(pencilRotation);
	pencil->Update();
	Object::Update();
	//�v���C���[�Ƃ̃q�b�g�`�F�b�N
	for (int i = 0; i < 3; i++)
	{
		colliders[i]->Update();
		//�������Ă�����
		if (CollisionManager::GetInstance()->CheckHitBox(*colliders[i], COLLISION_ATTR_ALLIES))
		{
			//�l���G�t�F�N�g���o��������
			ParticleEmitter::CreateGetEffect(pencilPosition);
			//�l���t���O�A�l���t���[���t���O�𗧂Ă�
			get = true;
			getFlame = true;
		}
	}

}

void Pencil::Draw(const bool pause)
{
	//�l����͕`�悵�Ȃ�
	if (get)
		return;
	//�e�͂Ȃ�
	if (Object3D::GetDrawShadow())
		return;

	//�p�C�v���C����ݒ�
	PipelineState::SetPipeline("DrawShadowOBJ");
	//���M�̕`��
	pencil->Draw();

	//�|�[�Y��ԂȂ�G�t�F�N�g���o���Ȃ�
	if(pause) return;

	//�G�t�F�N�g��UV�X�N���[���p�萔�o�b�t�@�̍X�V
	ConstBuffDeta* constMap = nullptr;
	auto result = constBuff[bbIndex]->Map(0, nullptr, (void**)&constMap);
	assert(SUCCEEDED(result));
	constMap->offsetTime = offsetTime;
	constBuff[bbIndex]->Unmap(0, nullptr);

	//�p�C�v���C����ݒ�
	PipelineState::SetPipeline("AreaEffect", ADD);
	//�萔�o�b�t�@���Z�b�g
	DirectXLib::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(3, constBuff[bbIndex]->GetGPUVirtualAddress());
	//���C�e�B���O�������Z�u�����h�ŕ`��
	CustomDraw(false, false, ADD, true);
}
