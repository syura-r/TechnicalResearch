#include"SmartPtr.h"

//template<class T>
//void SmartPtr<T>::Release()
//{
//	if (--(pRefCount) == 0)
//	{
//		delete* ppPtr;
//		delete ppPtr;
//		delete pRefCount;
//	}
//}

//template<class T>
//SmartPtr<T>::SmartPtr(T* src, int add)
//{
//	pRefCount = new unsigned int;
//	*pRefCount = add;
//	ppPtr = new T*;
//	NullPtr = nullptr;
//	if (src)
//		*ppPtr = src;
//	else
//		*ppPtr = NullPtr;
//	AddRef();
//}
//
//template<class T>
//SmartPtr<T>::SmartPtr(const SmartPtr<T>& src)
//{
//	// ����̃|�C���^�����ׂăR�s�[
//	pRefCount = src.pRefCount;
//	ppPtr = src.ppPtr;
//
//	// �������g�̎Q�ƃJ�E���^�𑝉�
//	AddRef();
//
//}

//template<class T>
//template<class T2> SmartPtr<T>::SmartPtr(SmartPtr<T2>& src)
//{
//	// ����̃_�u���|�C���^���R�s�[
//	pRefCount = src.GetRefPtr();
//	ppPtr = (T**)src.GetPtrPtr();
//
//	// �^�`�F�b�N�R�s�[
//	*ppPtr = src.GetPtr();
//
//	// �������g�̎Q�ƃJ�E���^�𑝉�
//	AddRef();
//}
//
//template<class T>
//SmartPtr<T>::SmartPtr(const int nullval)
//{
//	pRefCount = new UINT;
//	*pRefCount = 0;
//	ppPtr = new T*;
//	*ppPtr = NullPtr;
//	AddRef();       // �Q�ƃJ�E���^����
//}
//
//template<class T>
//void SmartPtr<T>::SetPtr(T* src, int add)
//{
//	// �Q�ƃJ�E���^�����炵����ɍď�����
//	Release();
//	pRefCount = new unsigned int;
//	*pRefCount = add;
//	ppPtr = new T*;
//	if (src)
//		*ppPtr = src;
//	else
//		*ppPtr = NullPtr;
//	AddRef();
//}

//template<class T>
//inline void SmartPtr<T>::SwapPtr(SmartPtr<T>& src)
//{
//	T* pTmp = src.GetPtr();
//	*src.ppPtr = *ppPtr;   // �|�C���^�̌���
//	*ppPtr = pTmp;
//}


//template<class T>
//template<class T2>
//inline bool SmartPtr<T>::DownCast(SmartPtr<T2>& src)
//{
//	{
//		// �����̃X�}�[�g�|�C���^�̎��|�C���^���A
//		// �����̓o�^���Ă���|�C���^��
//		// �_�E���L���X�g�\�ȏꍇ�̓_�E���L���X�g�R�s�[�����s
//		T* castPtr = dynamic_cast<T*>(src.GetPtr());
//		if (castPtr) {
//			// �_�E���L���X�g����
//			// �����͈Ⴄ�l�ɂȂ�̂�
//			// �����̎Q�ƃJ�E���^��1����
//			Release();
//			// �V�����|�C���^�ƎQ�ƃJ�E���^�����L
//			ppPtr = (T**)src.GetPtrPtr();
//			*ppPtr = castPtr;
//			pRefCount = src.GetRefPtr();
//			// �Q�ƃJ�E���^����
//			AddRef();
//			return true;
//		}
//
//		return false;
//	}
//}
//template<class T>
//SmartPtr<T>& SmartPtr<T>::operator =(const SmartPtr<T>& src)
//{
//	// �������g�ւ̑���͕s���ňӖ��������̂�
//	// �s��Ȃ��B
//	if (*src.ppPtr == *ppPtr)
//		return (*this);
//
//	// �����͑��l�ɂȂ��Ă��܂��̂�
//   // �Q�ƃJ�E���^��1����
//	Release();
//
//	// ����̃|�C���^���R�s�[
//	pRefCount = src.pRefCount;
//	ppPtr = src.ppPtr;
//
//	// �V�����������g�̎Q�ƃJ�E���^�𑝉�
//	AddRef();
//
//	return (*this);
//}

//template<class T>
//template<class T2> SmartPtr<T>& SmartPtr<T>::operator =(SmartPtr<T2>& src)
//{
//	// �������g�ւ̑���͕s���ňӖ��������̂�
//	// �s��Ȃ��B
//	if (src.GetPtr() == *ppPtr)
//		return (*this);
//
//	// �����͑��l�ɂȂ��Ă��܂��̂�
//   // �Q�ƃJ�E���^��1����
//	Release();
//
//	// ����̃|�C���^���R�s�[
//	pRefCount = src.GetRefPtr();
//	ppPtr = (T**)src.GetPtrPtr();
//
//	// �^�`�F�b�N�R�s�[
//	*ppPtr = src.GetPtr();
//
//	// �V�����������g�̎Q�ƃJ�E���^�𑝉�
//	AddRef();
//
//	return (*this);
//}


//template<class T>
//SmartPtr<T>& SmartPtr<T>::operator=(const int nullval)
//{
//	// �����͋�Ȑl�ɂȂ��Ă��܂��̂�
//	// �Q�ƃJ�E���^��1����
//	Release();
//
//	// �V�K�Ɏ������g�����
//	pRefCount = new unsigned int(1);
//	ppPtr = new T*;
//	*ppPtr = NullPtr;
//
//	return (*this);
//}

//template<class T>
//bool SmartPtr<T>::operator==(T* val)
//{
//	if (*ppPtr == val)
//		return true;
//	return false;
//}

//template<class T>
//bool SmartPtr<T>::operator!=(T* val)
//{
//	if (*ppPtr != val)
//		return true;
//	return false;
//}
