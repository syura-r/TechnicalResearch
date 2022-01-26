#include "FBXModel.h"
#include"DirectXLib.h"
#include "FbxLoader.h"
#include "Object3D.h"
#include "Texture.h"
#include"PtrDelete.h"
#include "Quaternion.h"

void FBXModel::CreateBuffers()
{
	auto dev = DirectXLib::GetInstance()->GetDevice();
	HRESULT result;
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// �A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataSkin) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffSkin));
	assert(SUCCEEDED(result));
	//���_�f�[�^�S�̂��T�C�Y
	UINT sizeVB =
		static_cast<UINT>(sizeof(VertexPosNormalUvSkin) *
			vertices.size());

	//���_�o�b�t�@�̐���
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));
	//���_�o�b�t�@�ւ̃f�[�^�]��
	VertexPosNormalUvSkin* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result))
	{
		std::copy(vertices.begin(), vertices.end(), vertMap);
		vertBuff->Unmap(0, nullptr);
	}

	//���_�o�b�t�@�r���[(VBV)�̍쐬
	vbView.BufferLocation =
		vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(vertices[0]);

	//���_�C���f�b�N�X�S�̂̃T�C�Y
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * indices.size());
	//�C���f�b�N�X�o�b�t�@�쐬
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeIB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));
	assert(SUCCEEDED(result));
	//�C���f�b�N�X�o�b�t�@�ւ̃f�[�^�]��
	unsigned short* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	if (SUCCEEDED(result))
	{
		std::copy(indices.begin(), indices.end(), indexMap);
		indexBuff->Unmap(0, nullptr);
	}
	//�C���f�b�N�X�o�b�t�@�r���[(IBV)�̍쐬
	ibView.BufferLocation =
		indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;

	ConstBufferDataSkin* constMapSkin = nullptr;
	result = constBuffSkin->Map(0, nullptr, (void**)&constMapSkin);
	assert(SUCCEEDED(result));
	for (int i = 0; i < MAX_BONES; i++)
	{
		constMapSkin->bones[i] = XMMatrixIdentity();
	}
	constBuffSkin->Unmap(0, nullptr);

	//�A�j���[�V�����̏�����
	frameTime.SetTime(0, 0, 0, 1, 0, FbxTime::EMode::eFrames60);
	isPlay = true;
}

FBXModel::~FBXModel()
{
}

void FBXModel::Draw()
{

	auto cmdList = DirectXLib::GetInstance()->GetCommandList();
	//���_�o�b�t�@���Z�b�g(VBV)
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	//�C���f�b�N�X�o�b�t�@���Z�b�g
	cmdList->IASetIndexBuffer(&ibView);

	//�f�X�N���v�^�q�[�v�̃Z�b�g
	ID3D12DescriptorHeap* ppHeaps[] = { Texture::GetBasicDescHeap().Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	if (!Object3D::GetDrawShadow())
	{
		//�V�F�[�_�[���\�[�X�r���[���Z�b�g
		cmdList->SetGraphicsRootDescriptorTable(2, Texture::GetGpuDescHandleSRV(texName));
		cmdList->SetGraphicsRootConstantBufferView(4, constBuffSkin->GetGPUVirtualAddress());
	}
	else
		cmdList->SetGraphicsRootConstantBufferView(1, constBuffSkin->GetGPUVirtualAddress());
	//�`��R�}���h
	cmdList->DrawIndexedInstanced((UINT)indices.size(), 1, 0, 0, 0);


}

void FBXModel::AddAnimation(const std::string& animationName, const int startFrame, const int endFrame)
{
	AnimationTime time;
	time.startTime.SetTime(0, 0, 0, startFrame, 0, FbxTime::EMode::eFrames60);
	time.endTime.SetTime(0, 0, 0, endFrame, 0, FbxTime::EMode::eFrames60);
	animations[animationName] = time;

}

const FBXModel::AnimationTime& FBXModel::GetAnimation(const std::string& animationName)
{
	return animations[animationName];
}

void FBXModel::AnimationInit()
{
	//0�Ԃ̃A�j���[�V�����擾
	FbxAnimStack* animstack = fbxScene->GetSrcObject<FbxAnimStack>(0);
	//�A�j���[�V�����̖��O�擾
	const char* animstackname = animstack->GetName();
	//�A�j���V�����̎��Ԏ擾
	FbxTakeInfo* takeinfo = fbxScene->GetTakeInfo(animstackname);

	//�J�n���Ԏ擾
	startTime = takeinfo->mLocalTimeSpan.GetStart();
	//�I�����Ԏ擾
	endTime = takeinfo->mLocalTimeSpan.GetStop();
	//�J�n���Ԃɍ��킹��
	currentTime = startTime;
	//�Đ�����Ԃɂ���
	isPlay = true;
}

void FBXModel::SetAnimationFrame(const int startFrame, const int endFrame, const int FrameTime)
{
	startTime.SetTime(0, 0, 0, startFrame, 0, FbxTime::EMode::eFrames60);
	currentTime = startTime;
	endTime.SetTime(0, 0, 0, endFrame, 0, FbxTime::EMode::eFrames60);
	if (startFrame > endFrame)
		frameTime.SetTime(0, 0, 0, -FrameTime, 0, FbxTime::EMode::eFrames60);
	else
		frameTime.SetTime(0, 0, 0, FrameTime, 0, FbxTime::EMode::eFrames60);
	isPlay = true;
}

void FBXModel::SetAnimation(const std::string& animationName, const int FrameTime)
{
	isPlay = true;

	//�o�^����Ă��Ȃ��A�j���[�V�������̏ꍇ��0�t���[���ڂ�ݒ�
	if(animations.count(animationName) == 0)
	{
		startTime.SetTime(0, 0, 0, 0, 0, FbxTime::EMode::eFrames60);
		endTime.SetTime(0, 0, 0, 0, 0, FbxTime::EMode::eFrames60);
		frameTime.SetTime(0, 0, 0, 0, 0, FbxTime::EMode::eFrames60);
		return;
	}
	nowAnimationName = animationName;
	startTime = animations[animationName].startTime;
	endTime = animations[animationName].endTime;
	currentTime = startTime;

	if (startTime > endTime)
		frameTime.SetTime(0, 0, 0, -FrameTime, 0, FbxTime::EMode::eFrames60);
	else
		frameTime.SetTime(0, 0, 0, FrameTime, 0, FbxTime::EMode::eFrames60);

}

bool FBXModel::PlayAnimation(bool endless)
{
	if (!isPlay)
		return false;

	currentTime += frameTime;
	if ((currentTime >= endTime && frameTime > 0) || (currentTime <= endTime && frameTime < 0))
	{
		if (!endless)
		{
			currentTime = endTime;
			isPlay = false;
			return false;
		}
		currentTime = startTime;
	}
	//�萔�o�b�t�@�̃f�[�^�]��
	ConstBufferDataSkin* constMapSkin = nullptr;
	HRESULT result = constBuffSkin->Map(0, nullptr, (void**)&constMapSkin);
	assert(SUCCEEDED(result));
	for (int i = 0; i < bones.size(); i++)
	{
		//���̎p��
		XMMATRIX matCurrentPose;
		//���̎p���s����擾
		FbxAMatrix fbxCurrentPose =
			bones[i].fbxCluster->GetLink()->EvaluateGlobalTransform(currentTime);
		//XMMATRIX�ɕϊ�
		FbxLoader::ConvertMatrixFromFbx(&matCurrentPose, fbxCurrentPose);
		//�������ăX�L�j���O���čs���
		constMapSkin->bones[i] = bones[i].invInitialPose * matCurrentPose;
	}
	constBuffSkin->Unmap(0, nullptr);


	return true;
}

bool FBXModel::PlayAnimation(const std::string& animationName, bool endless, const int frameTime,bool blend)
{
	if (nowAnimationName == animationName)
	{
		return PlayAnimation(endless);
	}
	if (!blend)
	{
		nowAnimationName = animationName;
		lerpAnimeName = animationName;
		SetAnimation(animationName, frameTime);
		lerpTime = 0.0f;
		return PlayAnimation(endless);
	}

	const auto nextAnimationTime = animations[animationName].startTime;
	if(lerpAnimeName != animationName)
	{
		lerpAnimeName = animationName;
		motionBlendTime = ClucLerpTime(currentTime, nextAnimationTime);
		
	}
	lerpTime += 1.0f / motionBlendTime;
	//�萔�o�b�t�@�̃f�[�^�]��
	ConstBufferDataSkin* constMapSkin = nullptr;
	HRESULT result = constBuffSkin->Map(0, nullptr, (void**)&constMapSkin);
	assert(SUCCEEDED(result));
	for (int i = 0; i < bones.size(); i++)
	{

		XMMATRIX matCurrentPose, matStartPose, matEndPose;
		////���̎p���s����擾
		//FbxAMatrix fbxStartPose =
		//	bones[i].fbxCluster->GetLink()->EvaluateGlobalTransform(currentTime);
		////���̎p���s����擾
		//FbxAMatrix fbxEndPose =
		//	bones[i].fbxCluster->GetLink()->EvaluateGlobalTransform(nextAnimationTime);

		//XMMATRIX�ɕϊ�
		FbxLoader::ConvertMatrixFromFbx(&matStartPose, blendStartBorn[i]);
		FbxLoader::ConvertMatrixFromFbx(&matEndPose, blendEndBorn[i]);
		matCurrentPose = SlerpMatrix(matStartPose, matEndPose, lerpTime);
		//�������ăX�L�j���O���čs���
		constMapSkin->bones[i] = bones[i].invInitialPose * matCurrentPose;
	}
	constBuffSkin->Unmap(0, nullptr);

	if (lerpTime >= 1.0f)
	{
		nowAnimationName = animationName;
		SetAnimation(animationName, frameTime);
		lerpTime = 0.0f;
	}

	return true;
}

XMMATRIX FBXModel::SlerpMatrix(const Matrix4& start, const Matrix4& end, float slerpAmount)
{
	if (start == end)
		return  Matrix4::convertToXMMATRIX(start);
	Quaternion qStart, qEnd, qResult;
	Vector3 curTrans, nextTrans, lerpedTrans;
	Vector3 curScale, nextScale, lerpedScale;
	Matrix4 startRotation, endRotation;
	Matrix4 returnMatrix;

	//��]�s���Quaternion.Slerp�ł͂Ȃ�Quaternion.Lerp
	qStart = quaternion(start);
	qEnd = quaternion(end);
	qResult = lerp(qStart, qEnd, slerpAmount);

	//���s�ړ��s���Vector3.Lerp���g���Ă���
	curTrans.x = start.m[3][0];
	curTrans.y = start.m[3][1];
	curTrans.z = start.m[3][2];
	nextTrans.x = end.m[3][0];
	nextTrans.y = end.m[3][1];
	nextTrans.z = end.m[3][2];

	lerpedTrans = Vector3::Lerp(curTrans, nextTrans, slerpAmount);

	//�g��k���s��ɂ�Vector3.Lerp
	startRotation = rotate(qStart);
	endRotation = rotate(qEnd);
	curScale.x = start.m[0][0] - startRotation.m[0][0];
	curScale.y = start.m[1][1] - startRotation.m[1][1];
	curScale.z = start.m[2][2] - startRotation.m[2][2];
	nextScale.x = end.m[0][0] - endRotation.m[0][0];
	nextScale.y = end.m[1][1] - endRotation.m[1][1];
	nextScale.z = end.m[2][2] - endRotation.m[2][2];

	lerpedScale = Vector3::Lerp(curScale, nextScale, slerpAmount);

	//srt�s����쐬����Matrix.CreateScale(S*R*T)�Ɠ������Ƃ����Ă���i�������̂��f�R�y���j
	returnMatrix = rotate(qResult);
	returnMatrix.m[3][0] = lerpedTrans.x;
	returnMatrix.m[3][1] = lerpedTrans.y;
	returnMatrix.m[3][2] = lerpedTrans.z;
	returnMatrix.m[0][0] += lerpedScale.x;
	returnMatrix.m[1][1] += lerpedScale.y;
	returnMatrix.m[2][2] += lerpedScale.z;
	return Matrix4::convertToXMMATRIX(returnMatrix);
}

float FBXModel::ClucLerpTime(const FbxTime& current, const FbxTime& next)
{
	float result = 0.0f;
	for (int i = 0; i < bones.size(); i++)
	{
		XMMATRIX matCurrentPose, matStartPose, matEndPose;
		//���̎p���s����擾
		blendStartBorn[i] =
			bones[i].fbxCluster->GetLink()->EvaluateGlobalTransform(current);
		//���̎p���s����擾
		blendEndBorn[i] =
			bones[i].fbxCluster->GetLink()->EvaluateGlobalTransform(next);
		//XMMATRIX�ɕϊ�
		FbxLoader::ConvertMatrixFromFbx(&matStartPose, blendStartBorn[i]);
		FbxLoader::ConvertMatrixFromFbx(&matEndPose, blendEndBorn[i]);

		matCurrentPose = matStartPose - matEndPose;

		Vector3 x = matCurrentPose.r[0];
		Vector3 y = matCurrentPose.r[1];
		Vector3 z = matCurrentPose.r[2];

		const float thisBoneSub = x.Length() + y.Length() + z.Length();

		if (result < thisBoneSub)
			result = thisBoneSub;
	}

	if (result == 0.0f)
		return 1.0f;
	
	return result * 2.5f;
}

