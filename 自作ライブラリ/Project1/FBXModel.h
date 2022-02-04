#pragma once
#include <array>
#include<string>
#include<DirectXMath.h>
#include"Model.h"
#include<vector>
#include<DirectXTex.h>
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <fbxsdk.h>
#include <unordered_map>

#include "DirectXLib.h"
#include "Matrix4.h"

struct Node
{
	//���O
	std::string name;
	//���[�J���X�P�[��
	DirectX::XMVECTOR scaling = { 1,1,1,0 };
	//���[�J����]�p
	DirectX::XMVECTOR rotation = { 0,0,0,0 };
	//���[�J���ړ�
	DirectX::XMVECTOR translation = { 0,0,0,1 };
	//���[�J���ό`�s��
	DirectX::XMMATRIX transform;
	//�O���[�o���ό`�s��
	DirectX::XMMATRIX globalTransform;
	//�e�m�[�h
	Node* parent = nullptr;
};
class FBXModel:public Model
{
private://�G�C���A�X
	//Microsoft::WRL::�ȗ�
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using TexMetadata = DirectX::TexMetadata;
	using ScratchImage = DirectX::ScratchImage;
	//std::���ȗ�
	using string = std::string;
	template <class T>using vector = std::vector<T>;
public:
	//�t�����h�N���X
	friend class FbxLoader;
	struct Bone
	{
		std::string name;
		DirectX::XMMATRIX invInitialPose;
		FbxCluster* fbxCluster;
		Bone(const std::string& name)
		{
			this->name = name;
		}
	};

private:
	void CreateBuffers();
	XMMATRIX SlerpMatrix(const Matrix4& start, const Matrix4& end, float slerpAmount);
	float ClucLerpTime(const FbxTime& current,const FbxTime& next);
	struct AnimationTime
	{
		FbxTime startTime;
		FbxTime endTime;
	};
	std::unordered_map<std::string, AnimationTime > animations;
	//���f����
	string name;
	//�m�[�h�z��
	vector<Node> nodes;
	//�A���r�G���g�W��
	XMFLOAT3 ambient = { 1,1,1 };
	//�f�B�t���[�Y�W��
	XMFLOAT3 diffuse = { 1,1,1 };
	//�e�N�X�`�����^�f�[�^
	//TexMetadata metadata = {};
	//�X�N���b�`�C���[�W
	ScratchImage scratchImg = {};
	//���_�o�b�t�@
	ComPtr<ID3D12Resource>vertBuff;
	//�C���f�b�N�X�o�b�t�@
	ComPtr<ID3D12Resource> indexBuff;
	//�e�N�X�`���o�b�t�@
	//ComPtr<ID3D12Resource>texBuff;
	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView = {};
	//�C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView = {};
	//SRV�p�f�X�N���v�^�q�[�v
	//ComPtr<ID3D12DescriptorHeap> descHeapSRV;
	//�e�N�X�`����
	string texName;
	//�{�[���̔z��
	std::vector<Bone> bones;
//FBX�V�[��
	FbxScene* fbxScene = nullptr;
	//static int bbIndex;
public://�����o�֐�
	//static void SetBbIndex()
	//{
	//	bbIndex = DirectXLib::GetInstance()->GetBbIndex();
	//}
	~FBXModel();
	//�`��
	void Draw()override;
	//getter
	std::vector <Bone> &GetBones() { return bones; }
	//�{�[���C���f�b�N�X�̍ő吔
	static const int MAX_BONE_INDICES = 4;
	//���f���̕ό`�s��擾
	const XMMATRIX& GetModelTransform() { return meshNode->globalTransform; }
	//getter
	FbxScene* GetFbxScene() { return fbxScene; }
		//�{�[���̍ő吔
	static const int MAX_BONES = 32;
	struct ConstBufferDataSkin
	{
		std::array<XMMATRIX,MAX_BONES> bones;
	};
	void AddAnimation(const std::string& animationName, const int startFrame, const int endFrame);
	const AnimationTime& GetAnimation(const std::string& animationName);

	
	/// <summary>
	/// �A�j���[�V�����J�n
	/// </summary>
	void AnimationInit();

	void SetAnimationFrame(const int startFrame, const int endFrame, const int frameTime = 1);
	void SetAnimation(const std::string&animationName, const int frameTime = 1);

	bool PlayAnimation(bool endless = false);
	bool PlayAnimation(const std::string& animationName,bool endless = false, const int frameTime = 1,bool blend = true);


	inline const FbxTime& GetCurrentAnimationTime() { return currentTime; }

private:
	ComPtr<ID3D12Resource> constBuffSkin; // �萔�o�b�t�@
		//1�t���[���̎���
	FbxTime frameTime;
	//�A�j���[�V�����J�n����
	FbxTime startTime;
	//�A�j���[�V�����I������
	FbxTime endTime;
	//���ݎ���(�A�j���[�V����)
	FbxTime currentTime;
	//���݂̃A�j���[�V������
	std::string nowAnimationName;

	std::string lerpAnimeName;
	//�A�j���[�V�����Đ���
	bool isPlay = false;
	float motionBlendTime = 7.0f;
	float lerpTime = 0.0f;
	//EvaluateGlobalTransform�֐����x�����烂�[�V�����u�����h�p�Ƀ{�[���z����p��
	std::array<FbxAMatrix, MAX_BONES> blendStartBorn;
	std::array<FbxAMatrix, MAX_BONES> blendEndBorn;


public://�T�u�N���X
	//���_�f�[�^�\����
	struct VertexPosNormalUvSkin
	{
		XMFLOAT3 pos;	 //�ʒu
		XMFLOAT3 normal;//�@���x�N�g��
		XMFLOAT2 uv;    //uv���W
		std::array<UINT, MAX_BONE_INDICES> boneIndex;//�ԍ�
		std::array<float, MAX_BONE_INDICES> boneWeight;//�d��
	};

	//���b�V�������m�[�h
	Node* meshNode = nullptr;
	//���_�f�[�^�z��
	vector<VertexPosNormalUvSkin> vertices;
	//���_�C���f�b�N�X�z��
	vector<unsigned short>indices;

};

