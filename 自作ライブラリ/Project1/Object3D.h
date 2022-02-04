#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <string>
#include"PipelineState.h"
#include"Camera.h"
#include "LightCamera.h"
#include"Model.h"
#include"LightGroup.h"
#include"Vector.h"
using namespace Microsoft::WRL;
using namespace DirectX;

enum BILLBOARD_TYPE
{
	NONE,//�r���{�[�h�Ȃ�
	NORMAL,//�S����
	Y_AXIS,//Y������
};
class Object3D
{
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public://�ÓI�����o�֐�
	static Object3D* Create(Model* model,Vector3& pos,Vector3& scale,Vector3& rot,XMFLOAT4& color);
	static void SetCamera(Camera* camera);
	static void SetLightCamera(LightCamera* camera) { lightCamera = camera; }
	static void SetLightGroup(LightGroup* lightGroup) { Object3D::lightGroup = lightGroup; }
	static void SetDrawShadow(const bool flag) { drawShadow = flag; }
	inline static bool GetDrawShadow() { return drawShadow; }
	static LightGroup* GetLightGroup() { return Object3D::lightGroup; }

	inline static Camera* GetCamera() { return Object3D::camera; }
	inline static LightCamera* GetLightCamera() { return Object3D::lightCamera; }
	static void SetBbIndex(const int arg_index)
	{
		bbIndex = arg_index;
	}
	static void ClucLightViewProjection();
private://�ÓI�����o�ϐ�
	static Camera* camera;
	static LightCamera* lightCamera;
	//���C�g�J�������g���ĕ`����s����
	static bool drawShadow;
	static LightGroup* lightGroup;
	static int bbIndex;
	static XMMATRIX  lightViewProjection;
public:
//�\����
	struct ConstBufferData
	{
		XMMATRIX viewprojection;
		XMMATRIX world;
		XMFLOAT3 cameraPos;
		float pad;//�p�f�B���O
		XMFLOAT4 color;
		XMMATRIX lightViewProjection;

	};
	struct LightCameraCBData
	{
		XMMATRIX viewprojection;
		XMMATRIX world;
	};
	struct ConstLightCameraBuff
	{
		XMMATRIX viewProjection;
		XMFLOAT3 cameraPos;
	};

//�����o�֐�
	Object3D(Vector3& pos, Vector3& scale, Vector3& rot, XMFLOAT4& color);
	bool Initialize();
	void Update(BILLBOARD_TYPE billboardType = NONE);
	void Draw(const bool fbx = false , const bool shade = true, BLENDTYPE type = ALPHA, const bool customPipeline = false, const int lightRootParameterIndex = 3);
	void SetModel(Model* model) { this->model = model; }
	void WorldUpdate(const Vector3& rot, const BILLBOARD_TYPE billboardType);

	const XMMATRIX& GetMatWorld() { return matWorld; }

	const XMMATRIX& GetRotateMat();
	//���f�����擾
	inline Model* GetModel() { return model; }
	//�e�I�u�W�F�N�g���Z�b�g
	void SetParent(Object3D* parent) { this->parent = parent; }
private://�����o�ϐ�
	std::array<ComPtr<ID3D12Resource>, 3> constBuff; // �萔�o�b�t�@
	std::array<ComPtr<ID3D12Resource>,3> lCameraConstBuff; // �萔�o�b�t�@
	std::array<ComPtr<ID3D12Resource>,3> constCameraBuff;
	// ���[�J�����[���h�ϊ��s��
	XMMATRIX matWorld;
	//���[���h�s��ɑ΂����]�p�x
	XMMATRIX appendRot = XMMatrixIdentity();
	// ���f��
	Model* model = nullptr;
	//�e�I�u�W�F�N�g
	Object3D* parent = nullptr;

	Vector3& position;
	Vector3& scale;
	Vector3& rotation;
	XMFLOAT4& color;

	
};




