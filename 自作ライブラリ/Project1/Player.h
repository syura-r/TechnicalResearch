#pragma once
#include <array>

#include "BoxCollider.h"
#include "DebugCamera.h"
#include "FBXModel.h"
#include "Object.h"
class Player :
	public Object
{
public:
	Player();
	void Initialize()override;
	void Update()override;
	void Draw() override;
	void DrawReady() override;
	static void SetDebugCamera(DebugCamera* cameraPtr) { camera = cameraPtr; }
	void Reset();
	bool GetReset() { return reset; }
private:
	struct ConstBuffData
	{
		float _Destruction; //����x����
		float _ScaleFactor; //�X�P�[���̕ω���
		float _PositionFactor; //�|�W�V�����̕ω���
		float _RotationFactor; //��]�̕ω���
		int _Tessellation;//�|���S�������x
		int _OnEasing;//�C�[�W���O�ŕ������邩
		XMFLOAT2 pad;
	};
	struct ConstLightCameraBuff
	{
		XMMATRIX viewProjection;
		XMFLOAT3 cameraPos;
	};

	std::array<ComPtr<ID3D12Resource>, 3> constBuff; // �萔�o�b�t�@
	ConstBuffData sendData;
	ComPtr<ID3D12Resource> constCameraBuff; // �萔�o�b�t�@

	const unsigned short WALK = 0b1 << 0;
	const unsigned short RUN = 0b1 << 1;
	const unsigned short JUMP = 0b1 << 2;
	const unsigned short SECONDJUMP = 0b1 << 3;
	const unsigned short AIRSLIDE = 0b1 << 4;

	unsigned short attribute = 0b0;

	const float val = 0.4f;
	
	const float secondJumpVYFist = 0.4f*val;//��i�W�����v�����������
	const float jumpVYFist = 0.5f * val;//�W�����v�����������
	const float wallJumpVYFist = 0.43f * val;//�ǃW�����v�����������

	//����������
	const float fallAcc = -0.02f * val;
	const float fallVYMin = -0.5f;

	int walkDustCounter = 0;
	
	//�����ʒu
	const Vector3 StartPos = { 23,2,-20 };

	//�ړ�����
	void Move();
	//�����蔻��
	void CheckHit();
	//�ǃW�����v����
	void WallJump();
	//�G�A�X���C�h����
	void AirSlide();
	//�J�����̐���
	void MoveCamera();
	//�o�����Ŏ��̉��o����
	void ResetPerform();
	//�ǃW�����v�t���O
	bool wallJump = false;
	int wallJumpCounter;
	//�ǃW�����v�p
	Vector3 wallRejectVec = {};
	Vector3 wallJumpVel;
	Vector3 wallJumAccel;
	//�G�A�X���C�h�t���O
	bool airSlide = false;
	int airSlideCounter;
	Vector3 airSlideVel;
	Vector3 airSlideAccel;
	//�ڒn�t���O
	bool onGround = true;
	//�W�����v�t���O
	bool jump = false;
	//��i�W�����v�t���O
	bool secondJump = false;
	//�����x�N�g��
	XMVECTOR fallV;
	//���݌����Ă����
	Vector3 direction;
	//�ړ����x
	float speed = 0.09f;
	const float walkSpeed = 0.09f;
	const float runSpeed = 0.125f;
	//����t���O
	bool run = false;
	//��]���x
	float rotateSpeed = 7.5f;
	Vector3 prePos;
	//�J������]��
	bool rotCamera;
	//�J�����̉�]�x����
	float radY;
	int cameraRotCount;
	const int RotTime = 10;

	bool reset = true;
	bool appear;
	bool resetMove;
	int appearCounter;//�o�����̉��o�p�J�E���^�[
	int disappearCounter;//���Ŏ��̉��o�p�J�E���^�[
	int resetMoveCounter;
	Vector3 resetStartPos;
	float resetPhi;
	
	//BoxCollider* boxCollider;

	FBXModel* myModel = nullptr;
private://�ÓI�����o�ϐ�
	static DebugCamera* camera;

};

