#pragma once
#include <intsafe.h>

#include"Vector.h"
#include<string>
struct ParticleParameter
{
	// �����x
	Vector3 accel = {0,0,0};
	float s_alpha = 1;
	// �X�P�[��
	Vector2 scaleAce = { 0,0 };
	Vector2 s_scale = { 0,0 };
	Vector2 e_scale = { 0,0 };
	float e_alpha = 1;
	// �I���t���[��
	int num_frame = 60;

	Vector3 s_color = {0,0,0};
	float pad;
	Vector3 s_rotation = {0,0,0};
	float pad2;
	// �ŏI�l
	Vector3 e_color = {1,1,1};
	float pad3;
	Vector3 e_rotation = {0,0,0};
	float pad4;
};

struct OutputData
{
	Vector3 position;
	//���S�t���O
	UINT isDead;

	Vector3 rotation;
	//�r���{�[�h���邩�ǂ���
	UINT billboardActive;

	DirectX::XMFLOAT4 color;
	Vector2 scale;
	Vector2 scaleVel;

	Vector3 velocity;
	UINT frame;

};
