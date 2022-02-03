#pragma once
#include "Object.h"
class GSShaderTestObj :
    public Object
{
public:
	//GSShaderTestObj(const Vector3& pos);
	virtual void Draw() override;
	void DrawReady() override;
	static void CreateConstBuff();
	static void SendConstBuff();
	static void ChangeSendData();
protected:
	struct ConstBuffData
	{
		//XMFLOAT4 status;
		float _Destruction; //����x����
		float _ScaleFactor; //�X�P�[���̕ω���
		float _PositionFactor;//�|�W�V�����̕ω���
		float _RotationFactor;//��]�̕ω���
		int _Tessellation;//�|���S�������x
		int _OnEasing;//�C�[�W���O���邩�ǂ���
		int _OnTargetBreak; //����̈ʒu�̂ݕ������邩�ǂ���
		int _OnCameraBreak;
		XMFLOAT3 BreakPoint; //���𒆐S�_
		float BreakRadius; //���𔼌a
		XMFLOAT3 TargetPos;//�J�����̒����_
		float CapsuleRadius;//�J�������璍���_�܂ł̃J�v�Z�����a
		int _On4Vertex;
	};
	struct ConstLightCameraBuff
	{
		XMMATRIX viewProjection;
		XMFLOAT3 cameraPos;
	};

	static ComPtr<ID3D12Resource> constBuff[3]; // �萔�o�b�t�@
	static ConstBuffData sendData;
	static ComPtr<ID3D12Resource> constCameraBuff[3]; // �萔�o�b�t�@
};

