#pragma once
class Scene
{
public:
	enum SCENE
	{
		Title,
		Play,
		Ending,
		Test,
	};
	Scene() {};
	virtual ~Scene() {};
	//������
	virtual void Initialize() = 0;
	//�X�V
	virtual void Update() = 0;

	//�w�ʕ`��
	virtual void PreDraw() = 0;
	//�O�ʕ`��
	virtual void PostDraw() = 0;//����PostDraw�ɃX�v���C�g�`�������ƃI�u�W�F�N�g�̏�ɃX�v���C�g���`�悳���


	//�I���t���O���擾
	inline bool GetIsEnd()
	{
		return isEnd;
	}
	//���̃V�[���ɐi��
	inline SCENE NextScene()
	{
		return next;
	}

protected:
	//�I���t���O��True�ɂ���
    inline void ShutDown()
	{
		isEnd = true;
	}
	//�I���t���O
	bool isEnd;
	//�V�[���J�ڊJ�n�t���O
	bool nextFeed;
	//���̃V�[��
	SCENE next;
};

