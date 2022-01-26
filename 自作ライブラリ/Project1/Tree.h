#pragma once
#include"SmartPtr.h"
#include "Vector.h"
#include<vector>
#include<list>
#include<windows.h>
namespace Tree
{

template <class T>
class TreeSpace;

//���ؓo�^�I�u�W�F�N�g
template <class  T>
class TreeObject
{
public:
	TreeSpace<T>* pSpace;			// �o�^���
	T* pObject;				// ����ΏۃI�u�W�F�N�g
	SmartPtr<TreeObject<T> > spPre;	// �O��OBJECT_FOR_TREE�\����
	SmartPtr<TreeObject<T> > spNext;	// ����OBJECT_FOR_TREE�\����

public://�����o�֐�
	//�R���X�g���N�^
	TreeObject()
	{
		pSpace = nullptr;
		pObject = nullptr;
	}
	//�f�X�g���N�^
	virtual ~TreeObject(){}

	// ���烊�X�g����O���
	bool Remove()
	{
		// ���łɈ�E���Ă��鎞�͏����I��
		if (!pSpace)
			return false;

		// ������o�^���Ă����ԂɎ��g��ʒm
		if (!pSpace->OnRemove(this))
			return false;

		// ��E����
		// �O��̃I�u�W�F�N�g�����т���
		if (spPre.GetPtr() != nullptr)
		{
			spPre->spNext = spNext;
			spPre.SetPtr(nullptr);
		}
		if (spNext.GetPtr() != nullptr)
		{
			spNext->spPre = spPre;
			spNext.SetPtr(nullptr);
		}
		pSpace = nullptr;
		return true;
	}

	// ��Ԃ�o�^
	void RegistCell(TreeSpace<T>* pSpace)
	{
		this->pSpace = pSpace;
	}

	// ���̃I�u�W�F�N�g�ւ̃X�}�[�g�|�C���^���擾
	SmartPtr<TreeObject<T> >& GetNextObj()
	{
		return spNext;
	}
};

// ���`8���؋�ԊǗ��N���X
#define CLINER8TREEMANAGER_MAXLEVEL		7//�ő��ԕ�����
template <class T>
class CLiner8TreeManager
{
protected:
	unsigned int m_uiDim;
	TreeSpace<T>** ppCellAry;	// ���`��ԃ|�C���^�z��
	unsigned int m_iPow[CLINER8TREEMANAGER_MAXLEVEL + 1];	// �ׂ��搔�l�z��
	Vector3 m_W;	// �̈�̕�
	Vector3 m_RgnMin;	// �̈�̍ŏ��l
	Vector3 m_RgnMax;	// �̈�̍ő�l
	Vector3 m_Unit;		// �ŏ��̈�̕ӂ̒���
	DWORD m_dwCellNum;		// ��Ԃ̐�
	unsigned int m_uiLevel;			// �ŉ��ʃ��x��

public:
	// �R���X�g���N�^
	CLiner8TreeManager()
	{
		m_uiDim = 0;
		ppCellAry = nullptr;
		m_W.x = m_W.y = m_W.z = 1.0f;
		m_RgnMin.x = m_RgnMin.y = m_RgnMin.z = 0.0f;
		m_RgnMax.x = m_RgnMax.y = m_RgnMax.z = 1.0f;
		m_Unit.x = m_Unit.y = m_Unit.z = 1.0f;
		m_dwCellNum = 0;
		m_uiLevel = 0;
	}

	// �f�X�g���N�^
	virtual ~CLiner8TreeManager()
	{
		DWORD i;
		for (i = 0; i < m_dwCellNum; i++) {
			if (ppCellAry[i] != nullptr)
				delete ppCellAry[i];
		}
		delete[] ppCellAry;
	}

	// ���`8���ؔz����\�z����
	bool Initialize(const unsigned int Level, const Vector3& Min, const Vector3& Max)
	{
		// �ݒ�ō����x���ȏ�̋�Ԃ͍��Ȃ�
		if (Level >= CLINER8TREEMANAGER_MAXLEVEL)
			return false;

		// �e���x���ł̋�Ԑ����Z�o
		int i;
		m_iPow[0] = 1;
		for (i = 1; i < CLINER8TREEMANAGER_MAXLEVEL + 1; i++)
			m_iPow[i] = m_iPow[i - 1] * 8;

		// Level���x���i0��_�j�̔z��쐬
		m_dwCellNum = (m_iPow[Level + 1] - 1) / 7;
		ppCellAry = new TreeSpace<T>*[m_dwCellNum];
		ZeroMemory(ppCellAry, sizeof(TreeSpace<T>*) * m_dwCellNum);

		// �̈��o�^
		m_RgnMin = Min;
		m_RgnMax = Max;
		m_W = m_RgnMax - m_RgnMin;
		m_Unit = m_W / ((float)(1 << Level));

		m_uiLevel = Level;

		return true;
	}

	// �I�u�W�F�N�g��o�^����
	bool Regist(const Vector3& Min, const Vector3& Max, SmartPtr<TreeObject<T> >& spTO)
	{
		// �I�u�W�F�N�g�̋��E�͈͂���o�^���[�g���ԍ����Z�o
		DWORD Elem = GetMortonNumber(Min, Max);
		if (Elem < m_dwCellNum) {
			// ��Ԃ������ꍇ�͐V�K�쐬
			if (!ppCellAry[Elem])
				CreateNewCell(Elem);
			return ppCellAry[Elem]->Push(spTO);
		}
		return false;	// �o�^���s
	}

	// �Փ˔��胊�X�g���쐬����
	inline DWORD GetAllCollisionList(std::vector<T*>& ColVect)
	{
		// ���X�g�i�z��j�͕K�����������܂�
		ColVect.clear();

		// ���[�g��Ԃ̑��݂��`�F�b�N
		if (ppCellAry[0] == nullptr)
			return 0;	// ��Ԃ����݂��Ă��Ȃ�

		// ���[�g��Ԃ�����
		std::list<T*> ColStac;
		GetCollisionList(0, ColVect, ColStac);

		return (DWORD)ColVect.size();
	}

	// ����̈ʒu����Փ˂̉\�������郊�X�g���쐬����
	DWORD GetTargetCollisionList(std::vector<T*>& ColVect, const Vector3& min,const Vector3& max)
	{
		// ���X�g�i�z��j�͕K�����������܂�
		ColVect.clear();
		auto Elem = GetMortonNumber(min,max);
		// ���[�g��Ԃ̑��݂��`�F�b�N
		if (ppCellAry[Elem] == nullptr)
			return 0;	// ��Ԃ����݂��Ă��Ȃ�

		// ���[�g��Ԃ�����
		GetCollisionList2(Elem, ColVect);

		if (Elem == 0)
			return (DWORD)ColVect.size();
		Elem = (Elem - 1) >> 3;
		if (ppCellAry[Elem] == nullptr)
			return (DWORD)ColVect.size();

		GetBackCollisionList(Elem, ColVect);

		return (DWORD)ColVect.size();
	}


protected:
	// ��ԓ��ŏՓ˃��X�g���쐬����
	inline bool GetCollisionList(const DWORD& Elem, std::vector<T*>& ColVect, std::list<T*>& ColStac)
	{
		std::list<T*>::iterator it;
		// �@ ��ԓ��̃I�u�W�F�N�g���m�̏Փ˃��X�g�쐬
		SmartPtr<TreeObject<T> > spOFT1 = ppCellAry[Elem]->GetFirstObj();
		while (spOFT1.GetPtr() != nullptr)
		{
			SmartPtr<TreeObject<T> > spOFT2 = spOFT1->spNext;
			while (spOFT2 != nullptr) {
				// �Փ˃��X�g�쐬
				ColVect.push_back(spOFT1->pObject);
				ColVect.push_back(spOFT2->pObject);
				spOFT2 = spOFT2->spNext;
			}
			// �A �Փ˃X�^�b�N�Ƃ̏Փ˃��X�g�쐬
			for (it = ColStac.begin(); it != ColStac.end(); ++it) {
				ColVect.push_back(spOFT1->pObject);
				ColVect.push_back(*it);
			}
			spOFT1 = spOFT1->spNext;
		}

		bool ChildFlag = false;
		// �B �q��ԂɈړ�
		DWORD ObjNum = 0;
		DWORD i, NextElem;
		for (i = 0; i < 8; i++) {
			NextElem = Elem * 8 + 1 + i;
			if (NextElem < m_dwCellNum && ppCellAry[Elem * 8 + 1 + i]) {
				if (!ChildFlag) {
					// �C �o�^�I�u�W�F�N�g���X�^�b�N�ɒǉ�
					spOFT1 = ppCellAry[Elem]->GetFirstObj();
					while (spOFT1.GetPtr()) {
						ColStac.push_back(spOFT1->pObject);
						ObjNum++;
						spOFT1 = spOFT1->spNext;
					}
				}
				ChildFlag = true;
				GetCollisionList(Elem * 8 + 1 + i, ColVect, ColStac);	// �q��Ԃ�
			}
		}

		// �D �X�^�b�N����I�u�W�F�N�g���O��
		if (ChildFlag) {
			for (i = 0; i < ObjNum; i++)
				ColStac.pop_back();
		}

		return true;
	}
	bool GetCollisionList2(DWORD Elem, std::vector<T*>& ColVect)
	{
		std::list<T*>::iterator it;
		// �@ ��ԓ��̃I�u�W�F�N�g���m�̏Փ˃��X�g�쐬
		SmartPtr<TreeObject<T> > spOFT1 = ppCellAry[Elem]->GetFirstObj();
		while (spOFT1.GetPtr() != NULL)
		{
			ColVect.push_back(spOFT1->pObject);
			spOFT1 = spOFT1->spNext;
		}

		// �B �q��ԂɈړ�
		DWORD ObjNum = 0;
		DWORD i, NextElem;
		for (i = 0; i < 8; i++) {
			NextElem = Elem * 8 + 1 + i;
			if (NextElem < m_dwCellNum && ppCellAry[Elem * 8 + 1 + i]) {
				GetCollisionList2(Elem * 8 + 1 + i, ColVect);	// �q��Ԃ�
			}
		}
		return true;
	}

	// �q��Ԃ���k�����Փ˃��X�g���쐬����
	bool GetBackCollisionList(DWORD Elem, std::vector<T*>& ColVect)
	{
		std::list<T*>::iterator it;
		// �@ ��ԓ��̃I�u�W�F�N�g���m�̏Փ˃��X�g�쐬
		SmartPtr<TreeObject<T> > spOFT1 = ppCellAry[Elem]->GetFirstObj();
		while (spOFT1.GetPtr() != NULL)
		{
			ColVect.push_back(spOFT1->pObject);
			spOFT1 = spOFT1->spNext;
		}

		// �B �e��ԂɈړ�
		DWORD  NextElem;

		if (Elem > 0)
		{
			NextElem = (Elem - 1) >> 3;
			if (ppCellAry[NextElem]) {
				GetBackCollisionList(NextElem, ColVect);	// �q��Ԃ�
			}
		}

		return true;
	}



	// ��Ԃ𐶐�
	bool CreateNewCell(DWORD Elem)
	{
		// �����̗v�f�ԍ�
		while (!ppCellAry[Elem])
		{
			// �w��̗v�f�ԍ��ɋ�Ԃ�V�K�쐬
			ppCellAry[Elem] = new TreeSpace<T>;

			// �e��ԂɃW�����v
			Elem = (Elem - 1) >> 3;
			if (Elem >= m_dwCellNum) break;
		}
		return true;
	}

	// ���W�����Ԕԍ����Z�o
	DWORD GetMortonNumber(const Vector3& Min, const Vector3& Max)
	{
		// �ŏ����x���ɂ�����e���ʒu���Z�o
		DWORD LT = GetPointElem(Min + 100);
		DWORD RB = GetPointElem(Max + 100);

		// ��Ԕԍ��������Z����
		// �ŏ�ʋ�؂肩�珊�����x�����Z�o
		DWORD Def = RB ^ LT;
		unsigned int HiLevel = 1;
		unsigned int i;
		for (i = 0; i < m_uiLevel; i++)
		{
			DWORD Check = (Def >> (i * 3)) & 0x7;
			if (Check != 0)
				HiLevel = i + 1;
		}
		DWORD SpaceNum = RB >> (HiLevel * 3);
		DWORD AddNum = (m_iPow[m_uiLevel - HiLevel] - 1) / 7;
		SpaceNum += AddNum;

		if (SpaceNum > m_dwCellNum)
			return 0xffffffff;

		return SpaceNum;
	}

	// �r�b�g�����֐�
	DWORD BitSeparateFor3D(const BYTE n)
	{
		DWORD s = n;
		s = (s | s << 8) & 0x0000f00f;
		s = (s | s << 4) & 0x000c30c3;
		s = (s | s << 2) & 0x00249249;
		return s;
	}

	// 3D���[�g����Ԕԍ��Z�o�֐�
	DWORD Get3DMortonNumber(const BYTE x,const BYTE y,const BYTE z)
	{
		return BitSeparateFor3D(x) | BitSeparateFor3D(y) << 1 | BitSeparateFor3D(z) << 2;
	}

	// ���W�����`8���ؗv�f�ԍ��ϊ��֐�
	DWORD GetPointElem(const Vector3& p)
	{
		return Get3DMortonNumber(
			(BYTE)((p.x - m_RgnMin.x) / m_Unit.x),
			(BYTE)((p.y - m_RgnMin.y) / m_Unit.y),
			(BYTE)((p.z - m_RgnMin.z) / m_Unit.z)
		);
	}

};

//���؂̋�ԃN���X
template <class T>
class TreeSpace
{
protected:
	SmartPtr<TreeObject< T >> smartPtrLatest;//�ŐV�o�^�I�u�W�F�N�g�ւ̃X�}�[�g�|�C���^
public:
	//�R���X�g���N�^
	TreeSpace() {};

	//�f�X�g���N�^
	virtual ~TreeSpace()
	{
		if (smartPtrLatest.GetPtr() != nullptr) ResetLink(smartPtrLatest);
	}

	// �����N��S�ă��Z�b�g����
	void ResetLink(SmartPtr<TreeObject<T> >& spTO)
	{
		if (spTO->spNext.GetPtr() != nullptr)
			ResetLink(spTO->spNext);
		spTO.SetPtr(nullptr);		// �X�}�[�g�|�C���^���Z�b�g		
	}

	//�I�u�W�F�N�g���v�b�V��
	bool Push(SmartPtr<TreeObject<T> >& spTO)
	{
		if (spTO.GetPtr() == nullptr) return false;	// �����I�u�W�F�N�g�͓o�^���Ȃ�
		if (spTO->pSpace == this) return false;	// 2�d�o�^�`�F�b�N
		if (smartPtrLatest.GetPtr() == nullptr) {
			smartPtrLatest = spTO;	// ��ԂɐV�K�o�^
		}
		else
		{
			// �ŐVOFT�I�u�W�F�N�g���X�V
			spTO->spNext = smartPtrLatest;
			smartPtrLatest->spPre = spTO;
			smartPtrLatest = spTO;
		}
		spTO->RegistCell(this);	// ��Ԃ�o�^
		return true;
	}

	inline SmartPtr<TreeObject<T> >& GetFirstObj()
	{
		return smartPtrLatest;
	}

	// �폜�����I�u�W�F�N�g���`�F�b�N
	bool OnRemove(TreeObject<T>* pRemoveObj)
	{
		if (smartPtrLatest.GetPtr() == pRemoveObj) {
			// ���̃I�u�W�F�N�g�ɑ}���ւ�
			if (smartPtrLatest.GetPtr() != nullptr)
				smartPtrLatest = smartPtrLatest->GetNextObj();
		}
		return true;
	}

};

}
