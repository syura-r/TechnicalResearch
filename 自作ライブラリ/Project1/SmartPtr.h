#pragma once
template<class T>
class SmartPtr
{
private://�����o�ϐ�
	unsigned int* pRefCount;
	T** ppPtr;
	static T* NullPtr;
private://�����o�֐�
	void AddRef()
	{
		(*pRefCount)++;
	}
	//�������
	void Release()
	{
		if (--(*pRefCount) == 0)
		{
			delete* ppPtr;
			delete ppPtr;
			delete pRefCount;
		}
	}

public://�����o�֐�

	//�f�t�H���g�R���X�g���N�^
	explicit SmartPtr(T* src = nullptr, int add = 0)
	{
		pRefCount = new unsigned int;
		*pRefCount = add;
		ppPtr = new T*;
		NullPtr = nullptr;
		if (src)
			*ppPtr = src;
		else
			*ppPtr = NullPtr;
		AddRef();
	}

	// �R�s�[�R���X�g���N�^�i���^�R�s�[�j
	SmartPtr(const SmartPtr<T>& src)
	{
		// ����̃|�C���^�����ׂăR�s�[
		pRefCount = src.pRefCount;
		ppPtr = src.ppPtr;

		// �������g�̎Q�ƃJ�E���^�𑝉�
		AddRef();

	}

	 // �R�s�[�R���X�g���N�^�i�ÖٓI�A�b�v�L���X�g�j
	template<class T2> SmartPtr(SmartPtr<T2>& src)
	{
		// ����̃_�u���|�C���^���R�s�[
		pRefCount = src.GetRefPtr();
		ppPtr = (T**)src.GetPtrPtr();

		// �^�`�F�b�N�R�s�[
		*ppPtr = src.GetPtr();

		// �������g�̎Q�ƃJ�E���^�𑝉�
		AddRef();
	}

	// �R�s�[�R���X�g���N�^�iNULL�����p�j
	SmartPtr(const int nullval)
	{
		pRefCount = new UINT;
		*pRefCount = 0;
		ppPtr = new T*;
		*ppPtr = NullPtr;
		AddRef();       // �Q�ƃJ�E���^����
	}

	//�f�X�g���N�^
	virtual ~SmartPtr()
	{
		Release();
	}

	//�|�C���^�̓o�^
	void SetPtr(T* src = nullptr, int add = 0)
	{
		// �Q�ƃJ�E���^�����炵����ɍď�����
		Release();
		pRefCount = new unsigned int;
		*pRefCount = add;
		ppPtr = new T*;
		if (src)
			*ppPtr = src;
		else
			*ppPtr = NullPtr;
		AddRef();
	}

	//getter
	T* GetPtr() { return *ppPtr; }
	T** GetPtrPtr() { return ppPtr; }
	unsigned int* GetRefPtr() { return pRefCount; }

	template <class T2> bool DownCast(SmartPtr<T2>& src)
	{
		{
			// �����̃X�}�[�g�|�C���^�̎��|�C���^���A
			// �����̓o�^���Ă���|�C���^��
			// �_�E���L���X�g�\�ȏꍇ�̓_�E���L���X�g�R�s�[�����s
			T* castPtr = dynamic_cast<T*>(src.GetPtr());
			if (castPtr) {
				// �_�E���L���X�g����
				// �����͈Ⴄ�l�ɂȂ�̂�
				// �����̎Q�ƃJ�E���^��1����
				Release();
				// �V�����|�C���^�ƎQ�ƃJ�E���^�����L
				ppPtr = (T**)src.GetPtrPtr();
				*ppPtr = castPtr;
				pRefCount = src.GetRefPtr();
				// �Q�ƃJ�E���^����
				AddRef();
				return true;
			}

			return false;
		}
	}

	void SwapPtr(SmartPtr<T>& src)
	{
		T* pTmp = src.GetPtr();
		*src.ppPtr = *ppPtr;   // �|�C���^�̌���
		*ppPtr = pTmp;
	}

	
//////////////���Z�q�I�[�o�[���[�h////////////////////

	SmartPtr<T>& operator =(const SmartPtr<T>& src)
	{
		// �������g�ւ̑���͕s���ňӖ��������̂�
		// �s��Ȃ��B
		if (*src.ppPtr == *ppPtr)
			return (*this);

		// �����͑��l�ɂȂ��Ă��܂��̂�
	   // �Q�ƃJ�E���^��1����
		Release();

		// ����̃|�C���^���R�s�[
		pRefCount = src.pRefCount;
		ppPtr = src.ppPtr;

		// �V�����������g�̎Q�ƃJ�E���^�𑝉�
		AddRef();

		return (*this);
	}

	template<class T2> SmartPtr<T>& operator =(SmartPtr<T2>& src)
	{
		// �������g�ւ̑���͕s���ňӖ��������̂�
		// �s��Ȃ��B
		if (src.GetPtr() == *ppPtr)
			return (*this);

		// �����͑��l�ɂȂ��Ă��܂��̂�
	   // �Q�ƃJ�E���^��1����
		Release();

		// ����̃|�C���^���R�s�[
		pRefCount = src.GetRefPtr();
		ppPtr = (T**)src.GetPtrPtr();

		// �^�`�F�b�N�R�s�[
		*ppPtr = src.GetPtr();

		// �V�����������g�̎Q�ƃJ�E���^�𑝉�
		AddRef();

		return (*this);
	}

	SmartPtr<T>& operator =(const int nullval)
	{
		// �����͋�Ȑl�ɂȂ��Ă��܂��̂�
		// �Q�ƃJ�E���^��1����
		Release();

		// �V�K�Ɏ������g�����
		pRefCount = new unsigned int(1);
		ppPtr = new T*;
		*ppPtr = NullPtr;

		return (*this);
	}

	T& operator *() { return **ppPtr; }

	T* operator ->() { return *ppPtr; }

	bool operator ==(T* val)
	{
		if (*ppPtr == val)
			return true;
		return false;
	}

	bool operator !=(T* val)
	{
		if (*ppPtr != val)
			return true;
		return false;
	}

//////////////////////////////////////////////////////

	

};


template <class T>
T* SmartPtr<T>::NullPtr = nullptr;
