#pragma once
#include"BaseCollider.h"
#include<DirectXMath.h>

class Object;

struct RaycastHit
{
	//�Փˑ���̃I�u�W�F�N�g
	Object* object = nullptr;
	//�Փˑ���̃R���C�_�[
	BaseCollider* collider = nullptr;
	//�Փ˓_
	DirectX::XMVECTOR inter;
	//�Փ˂܂ł̋���
	float distance = 0.0f;
};