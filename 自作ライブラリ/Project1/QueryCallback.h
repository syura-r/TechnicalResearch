#pragma once

#include<DirectXMath.h>

class Object;
class BaseCollider;

//�N�G���[�ɂ����𓾂�ׂ̍\����
struct QueryHit
{
	//�Փˑ���̃I�u�W�F�N�g
	Object* object = nullptr;
	//�Փˑ���̃R���C�_�[
	BaseCollider* collider = nullptr;
	//�Փ˓_
	DirectX::XMVECTOR inter;
	//�r�˃x�N�g��
	DirectX::XMVECTOR reject;
};

//�N�G���Ō��������o�������̓�����K�肷��N���X
class QueryCallback
{
public:
	QueryCallback() = default;
	virtual ~QueryCallback() = default;

	//�������R�[���o�b�N
	virtual bool OnQueryHit(const QueryHit& info) = 0;
};

