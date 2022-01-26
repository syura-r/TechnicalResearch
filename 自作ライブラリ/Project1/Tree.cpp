#include "Tree.h"
using namespace Tree;

//template<class T>
//inline bool TreeObject<T>::Remove()
//{
//	// ���łɈ�E���Ă��鎞�͏����I��
//	if (!pSpace)
//		return false;
//
//	// ������o�^���Ă����ԂɎ��g��ʒm
//	if (!pSpace->OnRemove(this))
//		return false;
//
//	// ��E����
//	// �O��̃I�u�W�F�N�g�����т���
//	if (spPre.GetPtr() != nullptr)
//	{
//		spPre->m_spNext = spNext;
//		spPre.SetPtr(nullptr);
//	}
//	if (spNext.GetPtr() != nullptr)
//	{
//		spNext->m_spPre = spPre;
//		spNext.SetPtr(nullptr);
//	}
//	pSpace = nullptr;
//	return true;
//}
//
//template<class T>
//inline CLiner8TreeManager<T>::CLiner8TreeManager()
//{
//	m_uiDim = 0;
//	ppCellAry = nullptr;
//	m_W.x = m_W.y = m_W.z = 1.0f;
//	m_RgnMin.x = m_RgnMin.y = m_RgnMin.z = 0.0f;
//	m_RgnMax.x = m_RgnMax.y = m_RgnMax.z = 1.0f;
//	m_Unit.x = m_Unit.y = m_Unit.z = 1.0f;
//	m_dwCellNum = 0;
//	m_uiLevel = 0;
//}
//
//template<class T>
//CLiner8TreeManager<T>::~CLiner8TreeManager()
//{
//	DWORD i;
//	for (i = 0; i < m_dwCellNum; i++) {
//		if (ppCellAry[i] != NULL)
//			delete ppCellAry[i];
//	}
//	delete[] ppCellAry;
//}

//template<class T>
//bool CLiner8TreeManager<T>::Initialize(const unsigned int Level, const Vector3& Min, const Vector3& Max)
//{
//	// �ݒ�ō����x���ȏ�̋�Ԃ͍��Ȃ�
//	if (Level >= CLINER8TREEMANAGER_MAXLEVEL)
//		return false;
//
//	// �e���x���ł̋�Ԑ����Z�o
//	int i;
//	m_iPow[0] = 1;
//	for (i = 1; i < CLINER8TREEMANAGER_MAXLEVEL + 1; i++)
//		m_iPow[i] = m_iPow[i - 1] * 8;
//
//	// Level���x���i0��_�j�̔z��쐬
//	m_dwCellNum = (m_iPow[Level + 1] - 1) / 7;
//	ppCellAry = new CCell<T>*[m_dwCellNum];
//	ZeroMemory(ppCellAry, sizeof(CCell<T>*) * m_dwCellNum);
//
//	// �̈��o�^
//	m_RgnMin = Min;
//	m_RgnMax = Max;
//	m_W = m_RgnMax - m_RgnMin;
//	m_Unit = m_W / ((float)(1 << Level));
//
//	m_uiLevel = Level;
//
//	return true;
//}

//template<class T>
//bool CLiner8TreeManager<T>::Regist(const Vector3& Min, const Vector3& Max, SmartPtr<TreeObject<T>>& spTO)
//{
//	// �I�u�W�F�N�g�̋��E�͈͂���o�^���[�g���ԍ����Z�o
//	DWORD Elem = GetMortonNumber(Min, Max);
//	if (Elem < m_dwCellNum) {
//		// ��Ԃ������ꍇ�͐V�K�쐬
//		if (!ppCellAry[Elem])
//			CreateNewCell(Elem);
//		return ppCellAry[Elem]->Push(spTO);
//	}
//	return false;	// �o�^���s
//}

//template<class T>
//DWORD CLiner8TreeManager<T>::GetAllCollisionList(std::vector<T*>& ColVect)
//{
//	// ���X�g�i�z��j�͕K�����������܂�
//	ColVect.clear();
//
//	// ���[�g��Ԃ̑��݂��`�F�b�N
//	if (ppCellAry[0] == nullptr)
//		return 0;	// ��Ԃ����݂��Ă��Ȃ�
//
//	// ���[�g��Ԃ�����
//	std::list<T*> ColStac;
//	GetCollisionList(0, ColVect, ColStac);
//
//	return (DWORD)ColVect.size();
//}

//template<class T>
//bool CLiner8TreeManager<T>::GetCollisionList(const DWORD& Elem, std::vector<T*>& ColVect, std::list<T*>& ColStac)
//{
//	std::list<T*>::iterator it;
//	// �@ ��ԓ��̃I�u�W�F�N�g���m�̏Փ˃��X�g�쐬
//	SmartPtr<TreeObject<T> > spOFT1 = ppCellAry[Elem]->GetFirstObj();
//	while (spOFT1.GetPtr() != nullptr)
//	{
//		SmartPtr<TreeObject<T> > spOFT2 = spOFT1->m_spNext;
//		while (spOFT2 != nullptr) {
//			// �Փ˃��X�g�쐬
//			ColVect.push_back(spOFT1->m_pObject);
//			ColVect.push_back(spOFT2->m_pObject);
//			spOFT2 = spOFT2->m_spNext;
//		}
//		// �A �Փ˃X�^�b�N�Ƃ̏Փ˃��X�g�쐬
//		for (it = ColStac.begin(); it != ColStac.end(); it++) {
//			ColVect.push_back(spOFT1->m_pObject);
//			ColVect.push_back(*it);
//		}
//		spOFT1 = spOFT1->m_spNext;
//	}
//
//	bool ChildFlag = false;
//	// �B �q��ԂɈړ�
//	DWORD ObjNum = 0;
//	DWORD i, NextElem;
//	for (i = 0; i < 8; i++) {
//		NextElem = Elem * 8 + 1 + i;
//		if (NextElem < m_dwCellNum && ppCellAry[Elem * 8 + 1 + i]) {
//			if (!ChildFlag) {
//				// �C �o�^�I�u�W�F�N�g���X�^�b�N�ɒǉ�
//				spOFT1 = ppCellAry[Elem]->GetFirstObj();
//				while (spOFT1.GetPtr()) {
//					ColStac.push_back(spOFT1->m_pObject);
//					ObjNum++;
//					spOFT1 = spOFT1->m_spNext;
//				}
//			}
//			ChildFlag = true;
//			GetCollisionList(Elem * 8 + 1 + i, ColVect, ColStac);	// �q��Ԃ�
//		}
//	}
//
//	// �D �X�^�b�N����I�u�W�F�N�g���O��
//	if (ChildFlag) {
//		for (i = 0; i < ObjNum; i++)
//			ColStac.pop_back();
//	}
//
//	return true;
//}
//
//template<class T>
//bool CLiner8TreeManager<T>::CreateNewCell(DWORD Elem)
//{
//	// �����̗v�f�ԍ�
//	while (!ppCellAry[Elem])
//	{
//		// �w��̗v�f�ԍ��ɋ�Ԃ�V�K�쐬
//		ppCellAry[Elem] = new CCell<T>;
//
//		// �e��ԂɃW�����v
//		Elem = (Elem - 1) >> 3;
//		if (Elem >= m_dwCellNum) break;
//	}
//	return true;
//}

//template<class T>
//DWORD CLiner8TreeManager<T>::GetMortonNumber(const Vector3& Min, const Vector3& Max)
//{
//	// �ŏ����x���ɂ�����e���ʒu���Z�o
//	DWORD LT = GetPointElem(Min);
//	DWORD RB = GetPointElem(Max);
//
//	// ��Ԕԍ��������Z����
//	// �ŏ�ʋ�؂肩�珊�����x�����Z�o
//	DWORD Def = RB ^ LT;
//	unsigned int HiLevel = 1;
//	unsigned int i;
//	for (i = 0; i < m_uiLevel; i++)
//	{
//		DWORD Check = (Def >> (i * 3)) & 0x7;
//		if (Check != 0)
//			HiLevel = i + 1;
//	}
//	DWORD SpaceNum = RB >> (HiLevel * 3);
//	DWORD AddNum = (m_iPow[m_uiLevel - HiLevel] - 1) / 7;
//	SpaceNum += AddNum;
//
//	if (SpaceNum > m_dwCellNum)
//		return 0xffffffff;
//
//	return SpaceNum;
//}

//template<class T>
//DWORD CLiner8TreeManager<T>::BitSeparateFor3D(const BYTE n)
//{
//	DWORD s = n;
//	s = (s | s << 8) & 0x0000f00f;
//	s = (s | s << 4) & 0x000c30c3;
//	s = (s | s << 2) & 0x00249249;
//	return s;
//}

//template<class T>
//DWORD CLiner8TreeManager<T>::GetPointElem(const Vector3& p)
//{
//	return Get3DMortonNumber(
//		(BYTE)((p.x - m_RgnMin.x) / m_Unit.x),
//		(BYTE)((p.y - m_RgnMin.y) / m_Unit.y),
//		(BYTE)((p.z - m_RgnMin.z) / m_Unit.z)
//	);
//}


//template<class T>
//inline void TreeSpace<T>::ResetLink(SmartPtr<TreeObject<T>>& spTO)
//{
//	if (spTO->spNext.GetPtr() != nullptr)
//		ResetLink(spTO->spNext);
//	spTO.SetPtr(nullptr);		// �X�}�[�g�|�C���^���Z�b�g		
//}

//template<class T>
//bool TreeSpace<T>::Push(SmartPtr<TreeObject<T>>& spTO)
//{
//	if (spTO.GetPtr() == nullptr) return false;	// �����I�u�W�F�N�g�͓o�^���Ȃ�
//	if (spTO->pSpacel == this) return false;	// 2�d�o�^�`�F�b�N
//	if (smartPtrLatest.GetPtr() == nullptr) {
//		smartPtrLatest = spTO;	// ��ԂɐV�K�o�^
//	}
//	else
//	{
//		// �ŐVOFT�I�u�W�F�N�g���X�V
//		spTO->spNext = smartPtrLatest;
//		smartPtrLatest->spPre = spTO;
//		smartPtrLatest = spTO;
//	}
//	spTO->RegistCell(this);	// ��Ԃ�o�^
//	return true;
//}

//template<class T>
//bool TreeSpace<T>::OnRemove(TreeObject<T>* pRemoveObj)
//{
//	if (smartPtrLatest.GetPtr() == pRemoveObj) {
//		// ���̃I�u�W�F�N�g�ɑ}���ւ�
//		if (smartPtrLatest.GetPtr() != nullptr)
//			smartPtrLatest = smartPtrLatest->GetNextObj();
//	}
//	return true;
//}
