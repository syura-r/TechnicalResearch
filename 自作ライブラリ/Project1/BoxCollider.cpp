#include "BoxCollider.h"

void BoxCollider::Update()
{
	//�t�����Ă���I�u�W�F�N�g�̎p���s����擾
	const XMMATRIX& matWorld = object->GetMatWorld();
	//�I�u�W�F�N�g�̒��S�̎擾
	objCenter = matWorld.r[3];
	//�I�u�W�F�N�g�̉�]�x����
	auto matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
	//�R���C�_�[�̉�]�x����
	auto colMatRot = XMMatrixIdentity();
	colMatRot *= XMMatrixRotationZ(XMConvertToRadians(colliderRot.z));
	colMatRot *= XMMatrixRotationX(XMConvertToRadians(colliderRot.x));
	colMatRot *= XMMatrixRotationY(XMConvertToRadians(colliderRot.y));

	Box::scale = scale;
	//�R���C�_�[�̒��S�ʒu�̌v�Z
	ClucCenter(matRot);
	//�R���C�_�[��MAX��MIN�̌v�Z
	ClucMinMaxPoint(colMatRot,matRot);
	//�ʕ����̌v�Z
	ClucDirect(colMatRot,matRot);
}



void BoxCollider::ClucCenter(const XMMATRIX& matRot)
{
	const XMMATRIX& matWorld = object->GetMatWorld();

	auto rotOffset = offset;
	//�I�u�W�F�N�g�̉�]�x�����ɍ��킹�ĉ�]
	rotOffset = XMVector3TransformNormal(rotOffset, matRot);

	Box::center = matWorld.r[3] + rotOffset;
}

void BoxCollider::ClucMinMaxPoint(const XMMATRIX&colMatRot,const XMMATRIX& matRot)
{
	//�X�P�[����񂩂��]�A���s�ړ��O�̃��[�J�����W�n�ł�MAX��MIN���Z�o
	XMVECTOR minOffset = -XMVECTOR{ scale.x, scale.y, scale.z, 0 };
	XMVECTOR maxOffset = XMVECTOR{ scale.x, scale.y, scale.z, 0 };

	Box::rotation = object->GetRotation();
	//�R���C�_�[�̉�]�x�����ɍ��킹�ĉ�]
	minOffset = XMVector3TransformNormal(minOffset, colMatRot);
	maxOffset = XMVector3TransformNormal(maxOffset, colMatRot);

	//�I�u�W�F�N�g�̒��S�ʒu����̃I�t�Z�b�g�����Z
	minOffset += offset;
	maxOffset += offset;
	
	//�I�u�W�F�N�g�̉�]�x�����ɍ��킹�ĉ�]
	minOffset = XMVector3TransformNormal(minOffset, matRot);
	maxOffset = XMVector3TransformNormal(maxOffset, matRot);

	//���[���h���W�n�ɕϊ�
	Box::minPosition = objCenter + minOffset;
	Box::maxPosition = objCenter + maxOffset;

}

void BoxCollider::ClucDirect(const XMMATRIX& colMatRot, const XMMATRIX& matRot)
{
	//��]�O�̊e������
	XMVECTOR x = { 1,0,0,0 };
	XMVECTOR y = { 0,1,0,0 };
	XMVECTOR z = { 0,0,1,0 };

	//�R���C�_�[�̉�]�x�����ɍ��킹�ĉ�]
	x = XMVector3TransformNormal(x, colMatRot);
	y = XMVector3TransformNormal(y, colMatRot);
	z = XMVector3TransformNormal(z, colMatRot);
	//�I�u�W�F�N�g�̉�]�x�����ɍ��킹�ĉ�]
	x = XMVector3TransformNormal(x, matRot);
	y = XMVector3TransformNormal(y, matRot);
	z = XMVector3TransformNormal(z, matRot);
	//���K�����Ċi�[
	normaDirect[0] = Vector3(x).Normalize().ConvertXMVECTOR();
	normaDirect[1] = Vector3(y).Normalize().ConvertXMVECTOR();
	normaDirect[2] = Vector3(z).Normalize().ConvertXMVECTOR();
}
