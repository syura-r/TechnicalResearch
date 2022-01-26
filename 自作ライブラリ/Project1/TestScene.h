#pragma once
#include "Scene.h"
#include"LightGroup.h"
#include "Object.h"
#include "OBJLoader.h"
#include "SmartPtr.h"
#include "Vector.h"
#include"Tree.h"
#include<vector>
class SPHERE:public Object
{
public:
	DWORD ID;
	Vector3 p;			// ���̈ʒu
	Vector3 Pre_p;		// 1�O�̉~�̈ʒu
	Vector3 v;			// ���x�x�N�g��
	Vector3 a;			// �����x�x�N�g��
	float r;				// ���a
	float w;				// ����
	float scale;			// �X�P�[��

	SPHERE()
	{
		Create(OBJLoader::GetModel("sphere"));
		p.x = p.y = p.z = 0.0f;
		Pre_p.x = Pre_p.y = Pre_p.z = 0.0f;
		v.x = v.y = v.z = 0.0f;
		a.x = a.y = a.z = 0.0f;
		r = 1.0f;
		w = 1.0f;
		scale = 1.0f;
	}
};


class TestScene :
    public Scene
{
public:
	TestScene();
	~TestScene()override;
	void Initialize()override;
	void Update()override;
	void PreDraw()override;
	void PostDraw()override;
private:
	LightGroup* lightGroup = nullptr;
	float coloramb[3] = { 1,1,1 };
	float color0[3] = { 1,1,1 };
	float color1[3] = { 1,1,1 };
	float color2[3] = { 1,1,1 };
	float lightDir[3] = { 0,0,1 };

	std::vector<SPHERE*> ColVect;	// �ՓˑΏۃ��X�g

	
	DWORD collisionCount;
	static const int g_SphereNum = 1000;

	float g_Circle_Ref = 0.75f;	// �����m�̔����W��
	float Wall_Ref = 0.78f;		// ���ƕǂ̔����W��
	int g_PartitionLevel = 6;		// ��ԕ������x��
	float g_Gravity = 9.8f;			// �d��
	bool g_XLeft = true;	// X������
	bool g_XRight = true;	// X���E��
	bool g_ZFar = true;	// Z������
	bool g_ZNear = true;	// Z����O��
	bool g_WindowMode = false;	// �E�B���h�E���[�h/�t���X�N���[���؂�ւ�

	int cn;



	SPHERE SAry[g_SphereNum];
	SmartPtr<Tree::TreeObject<SPHERE> > spSOFTAry[g_SphereNum];	// ���I�u�W�F�N�g����OFT�I�u�W�F�N�g
	Tree::CLiner8TreeManager<SPHERE> L8Tree;

	Object* wall[5];
	bool drawWall = true;
//////////////////////////////////////////////////////////	
	void SphereColProc(SPHERE* s1, SPHERE* s2);

	// ���̋��̈ʒu���擾
	void GetNextSpherePos(SPHERE& s)
	{
		Vector3 RefV;	// ���ˌ�̑��x�x�N�g��
		Vector3 ColliPos;	// �Փˈʒu
		float Res_time = 0.0f;	// �Փˌ�̈ړ��\����

		// �d�͂��|���ė��Ƃ�
		s.v.y -= g_Gravity / 60;	// 1�t���[����9.8/60(m/s)����

		// ���̑��x�ňʒu���X�V
		s.Pre_p = s.p;		// �O�̈ʒu��ۑ�
		s.p += s.v;			// �ʒu�X�V

		// �ǂƂ̏Փ˂��`�F�b�N
		// X����
		if (s.p.x < s.r && g_XLeft) {
			// ���ˌ�̑��x�x�N�g�����擾
			GetRefrectVelo(&RefV, Vector3(1, 0, 0), s.v, Wall_Ref);
			// �c�莞�ԎZ�o
			Res_time = (s.p.x - s.r) / s.v.x;
			// ���ˌ�̈ʒu���Z�o
			GetRelectedPos(Res_time, s, RefV);
		}
		// X�E��
		else if (s.p.x > 640 - s.r && g_XRight) {
			GetRefrectVelo(&RefV, Vector3(-1, 0, 0), s.v, Wall_Ref);
			Res_time = (s.p.x - 640 + s.r) / s.v.x;
			GetRelectedPos(Res_time, s, RefV);
		}
		// Z��O��
		if (s.p.z < s.r && g_ZNear) {
			GetRefrectVelo(&RefV, Vector3(0, 0, 1), s.v, Wall_Ref);
			Res_time = (s.p.z - s.r) / s.v.z;
			GetRelectedPos(Res_time, s, RefV);
		}
		// Z����
		else if (s.p.z > 640 - s.r && g_ZFar) {
			GetRefrectVelo(&RefV, Vector3(0, 0, -1), s.v, Wall_Ref);
			Res_time = (s.p.z - 640 + s.r) / s.v.z;
			GetRelectedPos(Res_time, s, RefV);
		}
		// Y����
		else if (s.p.y < s.r) {
			GetRefrectVelo(&RefV, Vector3(0, 1, 0), s.v, Wall_Ref);
			Res_time = (s.p.y - s.r) / s.v.y;
			GetRelectedPos(Res_time, s, RefV);
		}
	}
	// �ǂƋ��̔��˃x�N�g�����v�Z
	void GetRefrectVelo(Vector3* pOut, Vector3& N, Vector3& V, float e)
	{
		N = N.Normalize();
		*pOut = V - (1 + e) * Vector3::Dot(N, V) * N;
	}
	// �ǂƂ̔��ˌ�̈ʒu���Z�o
	void GetRelectedPos(float Res_time, SPHERE& s, Vector3& RefV)
	{
		// �Փˈʒu
		// 0.99�͕ǂɂ߂荞�܂Ȃ����߂̕␳
		s.p = s.Pre_p + s.v * (1 - Res_time) * 0.99f;
		// ���˃x�N�g��
		s.v = RefV;
		// �ʒu��␳
		s.p += s.v * Res_time;
	}

	
	bool CheckParticleCollision(
		FLOAT rA, FLOAT rB,
		Vector3* pPre_pos_A, Vector3* pPos_A,
		Vector3* pPre_pos_B, Vector3* pPos_B,
		FLOAT* pOut_t,
		Vector3* pOut_colli_A,
		Vector3* pOut_colli_B
	)
	{
		// �O�ʒu�y�ѓ��B�ʒu�ɂ�����p�[�e�B�N���Ԃ̃x�N�g�����Z�o
		Vector3 C0 = *pPre_pos_B - *pPre_pos_A;
		Vector3 C1 = *pPos_B - *pPos_A;
		Vector3 D = C1 - C0;

		// �Փ˔���p��2���֐��W���̎Z�o
		FLOAT P = D.Length() * D.Length(); if (P <= 0) return false; // ���������Ɉړ�
		FLOAT Q = Vector3::Dot(C0, D); if (Q == 0) return false; // ���s
		FLOAT R = C0.Length() * C0.Length();

		// �p�[�e�B�N������
		FLOAT r = rA + rB;

		// �Փ˔��莮
		FLOAT Judge = Q * Q - P * (R - r * r);
		if (Judge < 0) {
			// �Փ˂��Ă��Ȃ�
			return false;
		}

		// �Փˎ��Ԃ̎Z�o
		FLOAT t_plus = (-Q + sqrt(Judge)) / P;
		FLOAT t_minus = (-Q - sqrt(Judge)) / P;

		// �Փˎ��Ԃ�0����1���傫���ꍇ�A�Փ˂��Ȃ�
	 //   if( (t_plus < 0 || t_plus > 1) && (t_minus < 0 || t_minus > 1)) return false;
		if (t_minus < 0 || t_minus > 1) return false;

		// �Փˎ��Ԃ̌���it_minus������ɍŏ��̏Փˁj
		*pOut_t = t_minus;

		// �Փˈʒu�̌���
		*pOut_colli_A = *pPre_pos_A + t_minus * (*pPos_A - *pPre_pos_A);
		*pOut_colli_B = *pPre_pos_B + t_minus * (*pPos_B - *pPre_pos_B);

		return true; // �Փ˕�
	}




	///////////////////////////////////////////////////
	// �p�[�e�B�N���Փˌ㑬�x�ʒu�Z�o�֐�
	//   pColliPos_A : �Փ˒��̃p�[�e�B�N��A�̒��S�ʒu
	//   pVelo_A     : �Փ˂̏u�Ԃ̃p�[�e�B�N��A�̑��x
	//   pColliPos_B : �Փ˒��̃p�[�e�B�N��B�̒��S�ʒu
	//   pVelo_B     : �Փ˂̏u�Ԃ̃p�[�e�B�N��B�̑��x
	//   weight_A    : �p�[�e�B�N��A�̎���
	//   weight_B    : �p�[�e�B�N��B�̎���
	//   res_A       : �p�[�e�B�N��A�̔�����
	//   res_B       : �p�[�e�B�N��B�̔�����
	//   time        : ���ˌ�̈ړ��\����
	//   pOut_pos_A  : �p�[�e�B�N��A�̔��ˌ�ʒu
	//   pOut_velo_A : �p�[�e�B�N��A�̔��ˌ㑬�x�x�N�g��
	//   pOut_pos_B  : �p�[�e�B�N��B�̔��ˌ�ʒu
	//   pOut_velo_B : �p�[�e�B�N��B�̔��ˌ㑬�x�x�N�g��
	bool CalcParticleColliAfterPos(
		Vector3* pColliPos_A, Vector3* pVelo_A,
		Vector3* pColliPos_B, Vector3* pVelo_B,
		FLOAT weight_A, FLOAT weight_B,
		FLOAT res_A, FLOAT res_B,
		FLOAT time,
		Vector3* pOut_pos_A, Vector3* pOut_velo_A,
		Vector3* pOut_pos_B, Vector3* pOut_velo_B
	)
	{
		FLOAT TotalWeight = weight_A + weight_B; // ���ʂ̍��v
		FLOAT RefRate = (1 + res_A * res_B); // ������
		Vector3 C = *pColliPos_B - *pColliPos_A; // �Փˎ��x�N�g��
		C.Normalize();
		FLOAT Dot = Vector3::Dot((*pVelo_A - *pVelo_B), C); // ���ώZ�o
		Vector3 ConstVec = RefRate * Dot / TotalWeight * C; // �萔�x�N�g��

		// �Փˌ㑬�x�x�N�g���̎Z�o
		*pOut_velo_A = -weight_B * ConstVec + *pVelo_A;
		*pOut_velo_B = weight_A * ConstVec + *pVelo_B;

		// �Փˌ�ʒu�̎Z�o
		*pOut_pos_A = *pColliPos_A + time * (*pOut_velo_A);
		*pOut_pos_B = *pColliPos_B + time * (*pOut_velo_B);

		return true;
	}


	///////////////////////////////////////////////////
	// ���ʃp�[�e�B�N���Փ˔���E�����E�ʒu�Z�o�֐�
	//   r          : �p�[�e�B�N���̔��a
	//   pPre_pos   : �p�[�e�B�N���̑O�̈ʒu
	//   pPos       : �p�[�e�B�N���̎��̓��B�ʒu
	//   pNormal    : ���ʂ̖@��
	//   pPlane_pos : ���ʏ��1�_
	//   pOut_t     : �Փˎ��Ԃ��i�[����FLOAT�^�ւ̃|�C���^
	//   pOut_colli : �p�[�e�B�N���̏Փˈʒu���i�[����D3DXVECTOR�^�ւ̃|�C���^

	bool CheckParticlePlaneCollision(
		FLOAT r,
		Vector3* pPre_pos, Vector3* pPos,
		Vector3* pNormal, Vector3* pPlane_pos,
		FLOAT* t,
		Vector3* pOut_colli
	)
	{
		Vector3 C0 = *pPre_pos - *pPlane_pos; // ���ʏ�̈�_���猻�݈ʒu�ւ̃x�N�g��
		Vector3 D = *pPos - *pPre_pos; // ���݈ʒu����\��ʒu�܂ł̃x�N�g��
		Vector3 N = Vector3::Normalize(*pNormal);

		// �Փ˔���
		FLOAT Dot = Vector3::Dot(D, N);
		if (Dot >= 0)
			return false; // �������Ȃ�

		 // �������Ԃ̎Z�o
		FLOAT Dot_C0 = Vector3::Dot(C0, N);
		*t = (r - Dot_C0) / Dot;

		// �Փˈʒu�̎Z�o
		*pOut_colli = *pPre_pos + (*t) * D;

		// �Փ˔���
		// �������Ԃ�0�`1�̊ԂȂ�ΏՓ�
		if (*t >= 0 && *t <= 1)
			return true; // �Փ˕�

		 // �ǂɂ߂荞��ł����Ԃł����
		 // �ǂ̑O�ɉ����߂�
		if (-Dot < r) {
			*pOut_colli = *pPre_pos - Dot * N;
		}

		return false;
	}


	///////////////////////////////////////////////////
	// ���ʂƋ��̏Փˌ㑬�x�Z�o�֐�
	// pColliPos : �Փ˒��̃p�[�e�B�N���̒��S�ʒu
	// pVelo : �Փ˂̏u�Ԃ̃p�[�e�B�N���̑��x
	// res : �p�[�e�B�N���̕ǂɑ΂��锽����
	// time : ���ˌ�̈ړ��\����
	// pNormal : ���ʂ̖@��
	// pOut_pos : �p�[�e�B�N���̔��ˌ�ʒu
	// pOut_velo : �p�[�e�B�N���̔��ˌ㑬�x�x�N�g��

	bool CalcParticlePlaneAfterPos(
		Vector3* pColliPos,
		Vector3* pVelo,
		FLOAT res,
		FLOAT time,
		Vector3* pNormal,
		Vector3* pOut_pos,
		Vector3* pOut_velo
	)
	{
		// ���ˌ㑬�x���Z�o
		Vector3 N = Vector3::Normalize(*pNormal);
		*pOut_velo = *pVelo - (1 + res) * Vector3::Dot(N, *pVelo) * N;

		// �ړ��ʒu���v�Z
		*pOut_pos = *pColliPos + *pOut_velo * time;

		return true;
	}


	
};

