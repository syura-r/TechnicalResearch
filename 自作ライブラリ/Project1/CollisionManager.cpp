#include "CollisionManager.h"
#include"BaseCollider.h"
#include "BoxCollider.h"
#include"Collision.h"
#include "CollisionAttribute.h"
#include "DrawMode.h"
#include "GameSettingParam.h"
#include"MeshCollider.h"
#include "SphereCollider.h"
using namespace DirectX;
CollisionManager * CollisionManager::GetInstance()
{
	static CollisionManager instance;
	return &instance;
}

void CollisionManager::AddCollider(BaseCollider* collider)
{
	colliders[collider->GetObject3D()].push_front(collider);
	Tree::TreeObject<BaseCollider>* p = new Tree::TreeObject<BaseCollider>;
	p->pObject = collider;
	SmartPtr<Tree::TreeObject<BaseCollider>> s;
	s.SetPtr(p);
	const Vector3 min = collider->GetMin();
	const Vector3 max = collider->GetMax();
	spSOFTAry.push_back(s);
	L8Tree.Regist(min, max, s);
}

void CollisionManager::CheckAllCollisions()
{
	auto end_it = spSOFTAry.end();
	for(auto it = spSOFTAry.begin();it != end_it;++it)
	{
		if((*it)->pObject->move)
		{
			(*it)->Remove();
			const Vector3 min = (*it)->pObject->GetMin();
			const Vector3 max = (*it)->pObject->GetMax();
			L8Tree.Regist(min, max, *it);
		}
	}
	
	//const int collisionCount = L8Tree.GetAllCollisionList(ColVect);
	//long c = 0;
	//const auto colCount = collisionCount / 2;	// 2�Ŋ���̂̓y�A�ɂȂ��Ă���̂�
	//for (; c < colCount; c++) {
	//	BaseCollider* colA = ColVect[c * 2];
	//	BaseCollider* colB = ColVect[c * (long)2 + 1];
	//	if (colA->attribute == colB->attribute)
	//		continue;

	//	//�ǂ�������̏ꍇ
	//	if (colA->GetShapeType() == COLLISIONSHAPE_SPHERE &&
	//		colB->GetShapeType() == COLLISIONSHAPE_SPHERE)
	//	{
	//		Sphere* SphereA = dynamic_cast<Sphere*>(colA);
	//		Sphere* SphereB = dynamic_cast<Sphere*>(colB);
	//		DirectX::XMVECTOR inter;
	//		if (Collision::CheckSphere2Sphere(*SphereA, *SphereB, &inter))
	//		{
	//			colA->OnCollision(CollisionInfo(colB->GetObject3D(), colB, inter));
	//			colB->OnCollision(CollisionInfo(colA->GetObject3D(), colA, inter));
	//		}
	//	}
	//	//���b�V���Ƌ��̏ꍇ
	//	else if (colA->GetShapeType() == COLLISIONSHAPE_MESH &&
	//		colB->GetShapeType() == COLLISIONSHAPE_SPHERE)
	//	{
	//		MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colA);
	//		Sphere* sphere = dynamic_cast<Sphere*>(colB);
	//		DirectX::XMVECTOR inter;
	//		XMVECTOR tempReject;
	//		if (meshCollider->CheckCollisionSphere(*sphere, &inter,&tempReject)) {
	//			colA->OnCollision(CollisionInfo(colB->GetObject3D(), colB, inter));
	//			colB->OnCollision(CollisionInfo(colA->GetObject3D(), colA, inter, tempReject));
	//		}
	//	}
	//	//���b�V���Ƌ��̏ꍇ
	//	else if (colA->GetShapeType() == COLLISIONSHAPE_SPHERE &&
	//		colB->GetShapeType() == COLLISIONSHAPE_MESH)
	//	{
	//		MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colB);
	//		Sphere* sphere = dynamic_cast<Sphere*>(colA);
	//		DirectX::XMVECTOR inter;
	//		XMVECTOR tempReject;
	//		if (meshCollider->CheckCollisionSphere(*sphere, &inter,&tempReject)) 
	//		{
	//			colA->OnCollision(CollisionInfo(colB->GetObject3D(), colB, inter, tempReject));
	//			colB->OnCollision(CollisionInfo(colA->GetObject3D(), colA, inter));
	//		}
	//	}
	//	//�{�b�N�X���m�̏ꍇ
	//	else if (colA->GetShapeType() == COLLISIONSHAPE_BOX &&
	//		colB->GetShapeType() == COLLISIONSHAPE_BOX)
	//	{

	//		
	//		BoxCollider* BoxA = nullptr;
	//		BoxCollider* BoxB = nullptr;
	//		if (colA->GetAttribute() == COLLISION_ATTR_ALLIES)
	//		{
	//			BoxA = dynamic_cast<BoxCollider*>(colA);
	//			BoxB = dynamic_cast<BoxCollider*>(colB);
	//		}
	//		else
	//		{
	//			BoxA = dynamic_cast<BoxCollider*>(colB);
	//			BoxB = dynamic_cast<BoxCollider*>(colA);
	//		}
	//		DirectX::XMVECTOR inter;
	//		XMVECTOR tempReject;

	//		//if (BoxA->CheckRotation()|| BoxB->CheckRotation())
	//		//{
	//			if (Collision::CheckOBBOBB(*BoxA, *BoxB, &inter, &tempReject))
	//			{
	//				colA->OnCollision(CollisionInfo(colB->GetObject3D(), colB, inter, tempReject));
	//				colB->OnCollision(CollisionInfo(colA->GetObject3D(), colA, inter, tempReject));
	//			}
	//		//}
	//		//else if (Collision::CheckAABBAABB(*BoxA, *BoxB, &inter, &tempReject))
	//		//{
	//		//	colA->OnCollision(CollisionInfo(colB->GetObject3D(), colB, inter, tempReject));
	//		//	colB->OnCollision(CollisionInfo(colA->GetObject3D(), colA, inter, tempReject));
	//		//}
	//	}
	//	//�{�b�N�X�ƕ��ʂ̏ꍇ
	//	else if (colA->GetShapeType() == COLLISIONSHAPE_BOX &&
	//		colB->GetShapeType() == COLLISIONSHAPE_PLANE)
	//	{
	//		BoxCollider* BoxA = dynamic_cast<BoxCollider*>(colA);
	//		Plane* plane = dynamic_cast<Plane*>(colB);
	//		DirectX::XMVECTOR inter;
	//		XMVECTOR tempReject;

	//		if (Collision::CheckOBBPlane(*BoxA, *plane, &inter, &tempReject))
	//		{
	//			colA->OnCollision(CollisionInfo(colB->GetObject3D(), colB, inter,tempReject));
	//			colB->OnCollision(CollisionInfo(colA->GetObject3D(), colA, inter));
	//		}
	//	}
	//	//���ʂƃ{�b�N�X�̏ꍇ
	//	else if (colA->GetShapeType() == COLLISIONSHAPE_PLANE &&
	//		colB->GetShapeType() == COLLISIONSHAPE_BOX)
	//	{
	//		BoxCollider* Box = dynamic_cast<BoxCollider*>(colB);
	//		Plane* plane = dynamic_cast<Plane*>(colA);
	//		DirectX::XMVECTOR inter;
	//		XMVECTOR tempReject;

	//		if (Collision::CheckOBBPlane(*Box, *plane, &inter,&tempReject))
	//		{
	//			colA->OnCollision(CollisionInfo(colB->GetObject3D(), colB, inter));
	//			colB->OnCollision(CollisionInfo(colA->GetObject3D(), colA, inter, tempReject));
	//		}
	//	}

		
		//�{�b�N�X�Ƌ��̏ꍇ
		//else if (colA->GetShapeType() == COLLISIONSHAPE_SPHERE &&
		//	colB->GetShapeType() == COLLISIONSHAPE_BOX)
		//{
		//	Sphere* sphere = dynamic_cast<Sphere*>(colA);
		//	BoxCollider* BoxB = dynamic_cast<BoxCollider*>(colB);
		//	DirectX::XMVECTOR inter;
		//	if (Collision::CheckSphereBox(*sphere, *BoxB, &inter))
		//	{
		//		colA->OnCollision(CollisionInfo(colB->GetObject3D(), colB, inter));
		//		colB->OnCollision(CollisionInfo(colA->GetObject3D(), colA, inter));
		//	}
		//}
		////�{�b�N�X�Ƌ��̏ꍇ
		//else if (colB->GetShapeType() == COLLISIONSHAPE_SPHERE &&
		//	colA->GetShapeType() == COLLISIONSHAPE_BOX)
		//{
		//	Sphere* sphere = dynamic_cast<Sphere*>(colB);
		//	BoxCollider* BoxB = dynamic_cast<BoxCollider*>(colA);
		//	DirectX::XMVECTOR inter;
		//	if (Collision::CheckSphereBox(*sphere, *BoxB, &inter))
		//	{
		//		colA->OnCollision(CollisionInfo(colB->GetObject3D(), colB, inter));
		//		colB->OnCollision(CollisionInfo(colA->GetObject3D(), colA, inter));
		//	}
		//}
	//}

	//std::vector<BaseCollider*>::iterator itA;
	//std::vector<BaseCollider*>::iterator itB;

	//itA = colliders.begin();
	//for (; itA != colliders.end(); ++itA)
	//{
	//	itB = itA;
	//	++itB;
	//	for (; itB != colliders.end(); ++itB)
	//	{
	//		BaseCollider* colA = *itA;
	//		BaseCollider* colB = *itB;

	//		//�ǂ�������̏ꍇ
	//		if (colA->GetShapeType() == COLLISIONSHAPE_SPHERE &&
	//			colB->GetShapeType() == COLLISIONSHAPE_SPHERE)
	//		{
	//			Sphere* SphereA = dynamic_cast<Sphere*>(colA);
	//			Sphere* SphereB = dynamic_cast<Sphere*>(colB);
	//			DirectX::XMVECTOR inter;
	//			if (Collision::CheckSphere2Sphere(*SphereA, *SphereB, &inter))
	//			{
	//				colA->OnCollision(CollisionInfo(colB->GetObject3D(), colB, inter));
	//				colB->OnCollision(CollisionInfo(colA->GetObject3D(), colA, inter));
	//			}
	//		}
	//		//���b�V���Ƌ��̏ꍇ
	//		else if (colA->GetShapeType() == COLLISIONSHAPE_MESH &&
	//			colB->GetShapeType() == COLLISIONSHAPE_SPHERE) 
	//		{
	//			MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colA);
	//			Sphere* sphere = dynamic_cast<Sphere*>(colB);
	//			DirectX::XMVECTOR inter;
	//			if (meshCollider->CheckCollisionSphere(*sphere, &inter)) {
	//				colA->OnCollision(CollisionInfo(colB->GetObject3D(), colB, inter));
	//				colB->OnCollision(CollisionInfo(colA->GetObject3D(), colA, inter));
	//			}
	//		}
	//		//���b�V���Ƌ��̏ꍇ
	//		else if (colA->GetShapeType() == COLLISIONSHAPE_SPHERE &&
	//			colB->GetShapeType() == COLLISIONSHAPE_MESH)
	//		{
	//			MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colB);
	//			Sphere* sphere = dynamic_cast<Sphere*>(colA);
	//			DirectX::XMVECTOR inter;
	//			if (meshCollider->CheckCollisionSphere(*sphere, &inter))
	//			{
	//				colA->OnCollision(CollisionInfo(colB->GetObject3D(), colB, inter));
	//				colB->OnCollision(CollisionInfo(colA->GetObject3D(), colA, inter));
	//			}
	//		}
	//		//�{�b�N�X���m�̏ꍇ(AABB)
	//		else if (colA->GetShapeType() == COLLISIONSHAPE_BOX &&
	//			colB->GetShapeType() == COLLISIONSHAPE_BOX)
	//		{
	//			BoxCollider* BoxA = dynamic_cast<BoxCollider*>(colA);
	//			BoxCollider* BoxB = dynamic_cast<BoxCollider*>(colB);
	//			DirectX::XMVECTOR inter;
	//			if( Collision::CheckAABBAABB(*BoxA, *BoxB, &inter))
	//			{
	//				colA->OnCollision(CollisionInfo(colB->GetObject3D(), colB, inter));
	//				colB->OnCollision(CollisionInfo(colA->GetObject3D(), colA, inter));
	//			}
	//		}
	//		//�{�b�N�X�Ƌ��̏ꍇ
	//		else if (colA->GetShapeType() == COLLISIONSHAPE_SPHERE &&
	//			colB->GetShapeType() == COLLISIONSHAPE_BOX)
	//		{
	//			Sphere* sphere = dynamic_cast<Sphere*>(colA);
	//			BoxCollider* BoxB = dynamic_cast<BoxCollider*>(colB);
	//			DirectX::XMVECTOR inter;
	//			if (Collision::CheckSphereBox(*sphere, *BoxB, &inter))
	//			{
	//				colA->OnCollision(CollisionInfo(colB->GetObject3D(), colB, inter));
	//				colB->OnCollision(CollisionInfo(colA->GetObject3D(), colA, inter));
	//			}
	//		}
	//		//�{�b�N�X�Ƌ��̏ꍇ
	//		else if (colB->GetShapeType() == COLLISIONSHAPE_SPHERE &&
	//			colA->GetShapeType() == COLLISIONSHAPE_BOX)
	//		{
	//			Sphere* sphere = dynamic_cast<Sphere*>(colB);
	//			BoxCollider* BoxB = dynamic_cast<BoxCollider*>(colA);
	//			DirectX::XMVECTOR inter;
	//			if (Collision::CheckSphereBox(*sphere, *BoxB, &inter))
	//			{
	//				colA->OnCollision(CollisionInfo(colB->GetObject3D(), colB, inter));
	//				colB->OnCollision(CollisionInfo(colA->GetObject3D(), colA, inter));
	//			}
	//		}
	//	}
	//}
}

bool CollisionManager::CheckHitBox(const Box& box, unsigned short attribute)
{
	DWORD collisionCount = L8Tree.GetTargetCollisionList(ColVect, box.minPosition, box.maxPosition);
	DWORD c;
	for (c = 0; c < collisionCount; c++) 
	{
		BaseCollider* col = ColVect[c];
		if (col->attribute != attribute)
			continue;

		//�{�b�N�X
		if (col->GetShapeType() == COLLISIONSHAPE_BOX)
		{
			Box* boxB = dynamic_cast<Box*>(col);

			if (Collision::CheckOBBOBB(box, *boxB))
			{
				return true;
			}
		}
	}
	return false;
}

void CollisionManager::Initialize(const Vector3& minLine,const Vector3& maxLine)
{
	if (!L8Tree.Initialize(
		6,
		minLine,
		maxLine))
		assert(0);
	CreateConstBuff();
	CreateVBV();
	viewCollider = false;
}

bool CollisionManager::Raycast(const Ray & ray, BaseCollider* collider, RaycastHit * hitinfo, float maxDistance)
{
	return Raycast(ray,collider, 0xffff, hitinfo, maxDistance);
}

bool CollisionManager::Raycast(const Ray & ray, BaseCollider* collider, unsigned short attribute, RaycastHit * hitinfo, float maxDistance)
{
	bool result = false;
	//�����p�̃C�e���[�^
	//std::vector<BaseCollider*>::iterator it;
	//���܂łōł��߂��R���C�_�[���L�^����ׂ̃C�e���[�^
	///std::vector<BaseCollider*>::iterator it_hit;
	BaseCollider* hitCol;
	//���܂łōł��߂��R���C�_�[�̋������L�^����ϐ�
	float distance = maxDistance;
	//���܂łōł��߂��R���C�_�[�Ƃ̌�_���L�^����ϐ�
	XMVECTOR inter;

	DWORD collisionCount = L8Tree.GetTargetCollisionList(ColVect, collider->GetMin(), collider->GetMax());
	DWORD c;
	for (c = 0; c < collisionCount; c++) {
		BaseCollider* colA = ColVect[c];
		//����������Ȃ���΃X�L�b�v
		if (!(colA->attribute & attribute))
			continue;

		//���̏ꍇ
		if (colA->GetShapeType() == COLLISIONSHAPE_SPHERE)
		{
			Sphere* sphere = dynamic_cast<Sphere*>(colA);
			float tempDistance;
			XMVECTOR tempinter;
			//������Ȃ���Ώ��O
			if (!Collision::CheckRay2Sphere(ray, *sphere, &tempDistance, &tempinter))continue;
			//�������ŏ��łȂ���Ώ��O
			if (tempDistance >= distance)continue;
			//���܂łōł��߂��̂ł���΋L�^�����
			result = true;
			distance = tempDistance;
			inter = tempinter;
			hitCol = colA;
		}
		//���b�V���̏ꍇ
		else if (colA->GetShapeType() == COLLISIONSHAPE_MESH)
		{
			MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colA);

			float tempDistance;
			DirectX::XMVECTOR templnter;
			if (!meshCollider->CheckCollisionRay(ray, &tempDistance, &templnter))continue;
			if (tempDistance >= distance)continue;

			result = true;
			distance = tempDistance;
			inter = templnter;
			hitCol = colA;
		}
		//OBB�̏ꍇ
		else if (colA->GetShapeType() == COLLISIONSHAPE_BOX)
		{
			Box* box = dynamic_cast<Box*>(colA);

			float tempDistance;
			XMVECTOR tempinter;
			//������Ȃ���Ώ��O
			if (!Collision::CheckRay2OBB(ray, *box, &tempDistance, &tempinter))continue;
			//�������ŏ��łȂ���Ώ��O
			if (tempDistance >= distance)continue;
			//���܂łōł��߂��̂ł���΋L�^�����
			result = true;
			distance = tempDistance;
			inter = tempinter;
			hitCol = colA;
		}

	}
	//�ŏI�I�ɉ����ɓ������Ă����猋�ʂ���������
	if (result && hitinfo)
	{
		hitinfo->distance = distance;
		hitinfo->inter = inter;
		hitinfo->collider = hitCol;
		hitinfo->object = hitinfo->collider->GetObject3D();
	}
	return result;
}

void CollisionManager::QuerySphere(const Sphere & sphere, QueryCallback * callback, SphereCollider* collider, unsigned short attribute )
{
	assert(callback);

	std::vector<BaseCollider*>::iterator it;

	DWORD collisionCount = L8Tree.GetTargetCollisionList(ColVect,collider->GetMin(),collider->GetMax());
	DWORD c;
	for (c = 0; c < collisionCount; c++) {
		BaseCollider* col = ColVect[c];
		//����������Ȃ���΃X�L�b�v
		if (!(col->attribute & attribute))
			continue;
		
		//��
		if (col->GetShapeType() == COLLISIONSHAPE_SPHERE)
		{
			Sphere* sphereB = dynamic_cast<Sphere*>(col);

			XMVECTOR tempInter;
			XMVECTOR tempReject;
			if (!Collision::CheckSphere2Sphere(sphere, *sphereB, &tempInter, &tempReject))
				continue;

			//���������Z�b�g
			QueryHit info;
			info.collider = col;
			info.object = col->GetObject3D();
			info.inter = tempInter;
			info.reject = tempReject;

			//�N�G���[�R�[���o�b�N�Ăяo��
			if (!callback->OnQueryHit(info))
			{
				//�߂�l��false�̏ꍇ�A�p�������ɏI��
				return;
			}
		}
		//���b�V��
		if (col->GetShapeType() == COLLISIONSHAPE_MESH)
		{
			MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(col);

			XMVECTOR tempInter;
			XMVECTOR tempReject;
			if (!meshCollider->CheckCollisionSphere(sphere,&tempInter,&tempReject))
				continue;

			//���������Z�b�g
			QueryHit info;
			info.collider = col;
			info.object = col->GetObject3D();
			info.inter = tempInter;
			info.reject = tempReject;

			//�N�G���[�R�[���o�b�N�Ăяo��
			if (!callback->OnQueryHit(info))
			{
				//�߂�l��false�̏ꍇ�A�p�������ɏI��
				return;
			}
		}

	}

}

void CollisionManager::QueryBox(const Box& box, QueryCallback* callback, unsigned short attribute1, unsigned short attribute2, BaseCollider* collider)
{
	assert(callback);

	DWORD collisionCount = L8Tree.GetTargetCollisionList(ColVect, box.minPosition, box.maxPosition);
	DWORD c;
	for (c = 0; c < collisionCount; c++) {
		BaseCollider* col = ColVect[c];
		if (col->attribute != attribute1 && col->attribute != attribute2)
			continue;

		//��
		if (col->GetShapeType() == COLLISIONSHAPE_SPHERE)
		{
			Sphere* sphereB = dynamic_cast<Sphere*>(col);

			XMVECTOR tempInter;
			XMVECTOR tempReject;
			if (!Collision::CheckSphereBox(*sphereB, box, &tempInter, &tempReject))
				continue;

			//���������Z�b�g
			QueryHit info;
			info.collider = col;
			info.object = col->GetObject3D();
			info.inter = tempInter;
			info.reject = tempReject;

			//�N�G���[�R�[���o�b�N�Ăяo��
			if (!callback->OnQueryHit(info))
			{
				//�߂�l��false�̏ꍇ�A�p�������ɏI��
				return;
			}
		}
		//�{�b�N�X
		if (col->GetShapeType() == COLLISIONSHAPE_BOX)
		{
			Box* boxB = dynamic_cast<Box*>(col);

			XMVECTOR tempInter;
			XMVECTOR tempReject;

			if (!Collision::CheckOBBOBB(box, *boxB, &tempInter, &tempReject))
			{
				continue;
			}
			if (collider != nullptr)
				col->OnCollision(CollisionInfo(collider->GetObject3D(), collider, tempInter, tempReject));
			//���������Z�b�g
			QueryHit info;
			info.collider = col;
			info.object = col->GetObject3D();
			info.inter = tempInter;
			info.reject = tempReject;
			//if (tempReject.m128_f32[1] < 0)
			//{
			//	static  int a;
			//	a++;
			//}

			//�N�G���[�R�[���o�b�N�Ăяo��
			if (!callback->OnQueryHit(info))
			{
				//�߂�l��false�̏ꍇ�A�p�������ɏI��
				return;
			}
		}
		//����
		if (col->GetShapeType() == COLLISIONSHAPE_PLANE)
		{
			Plane* plane = dynamic_cast<Plane*>(col);

			XMVECTOR tempInter;
			XMVECTOR tempReject;

			if (!Collision::CheckOBBPlane(box, *plane, &tempInter, &tempReject))
			{
				continue;
			}
			if (collider != nullptr)
				col->OnCollision(CollisionInfo(collider->GetObject3D(), collider, tempInter, tempReject));
			//���������Z�b�g
			QueryHit info;
			info.collider = col;
			info.object = col->GetObject3D();
			info.inter = tempInter;
			info.reject = tempReject;
			//if (tempReject.m128_f32[1] < 0)
			//{
			//	static  int a;
			//	a++;
			//}

			//�N�G���[�R�[���o�b�N�Ăяo��
			if (!callback->OnQueryHit(info))
			{
				//�߂�l��false�̏ꍇ�A�p�������ɏI��
				return;
			}
		}
	}

}

void CollisionManager::DrawCollider()
{
	if(Object3D::GetDrawShadow())
		return;
//#ifdef _DEBUG
//	if (DrawMode::GetDrawImGui())
//	{
//		ImGui::Begin("Collider");
//		ImGui::Checkbox("viewCollider", &viewCollider);
//		ImGui::End();
//	}
//#endif
	
	if (!SettingParam::GetViewCollision())
		return;
	int bbIndex = DirectXLib::GetInstance()->GetBbIndex();

	auto cmdlist = DirectXLib::GetInstance()->GetCommandList();
	//�萔�o�b�t�@�̑��M
	SendConstBuff();
	
	UINT drawBoxNum,drawSphere;
//-----------------------------------BoxCollider�̕`��--------------------------------------
	SendBoxBuffers(drawBoxNum);
	// �p�C�v���C���X�e�[�g�̐ݒ�
	PipelineState::SetPipeline("CollisionBox");
	// �v���~�e�B�u�`���ݒ�
	//cmdlist->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

	// ���_�o�b�t�@�̐ݒ�
	cmdlist->IASetVertexBuffers(0, 1, &boxVbView[bbIndex]);

	// �萔�o�b�t�@�r���[���Z�b�g
	cmdlist->SetGraphicsRootConstantBufferView(0, constBuff[bbIndex]->GetGPUVirtualAddress());

	// �`��R�}���h
	cmdlist->DrawInstanced(drawBoxNum, 1, 0, 0);
//--------------------------------------------------------------------------------------------

//----------------------------------SphereCollider�̕`��--------------------------------------
	SendSphereBuffers(drawSphere);
	// �p�C�v���C���X�e�[�g�̐ݒ�
	PipelineState::SetPipeline("CollisionSphere");
	// �v���~�e�B�u�`���ݒ�
	//cmdlist->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

	// ���_�o�b�t�@�̐ݒ�
	cmdlist->IASetVertexBuffers(0, 1, &sphereVbView[bbIndex]);

	// �萔�o�b�t�@�r���[���Z�b�g
	cmdlist->SetGraphicsRootConstantBufferView(0, constBuff[bbIndex]->GetGPUVirtualAddress());

	// �`��R�}���h
	cmdlist->DrawInstanced(drawSphere, 1, 0, 0);
//----------------------------------------------------------------------------------------------
}

void CollisionManager::CreateVBV()
{
	HRESULT result = S_FALSE;
	auto dev = DirectXLib::GetInstance()->GetDevice();
	for (int i = 0; i < 3; i++)
	{
		//-----------------------------�{�b�N�X----------------------------------------

			// ���_�o�b�t�@����
		result = dev->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(sizeof(BoxVBData) * vertexCount),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&boxVertBuff[i]));
		if (FAILED(result)) {
			assert(0);
			return;
		}

		// ���_�o�b�t�@�r���[�̍쐬
		boxVbView[i].BufferLocation = boxVertBuff[i]->GetGPUVirtualAddress();
		boxVbView[i].SizeInBytes = sizeof(BoxVBData) * vertexCount;
		boxVbView[i].StrideInBytes = sizeof(BoxVBData);

		//-----------------------------------------------------------------------------
		//---------------------------------����----------------------------------------
			// ���_�o�b�t�@����
		result = dev->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(sizeof(SphereVBData) * vertexCount),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&sphereVertBuff[i]));
		if (FAILED(result)) {
			assert(0);
			return;
		}

		// ���_�o�b�t�@�r���[�̍쐬
		sphereVbView[i].BufferLocation = sphereVertBuff[i]->GetGPUVirtualAddress();
		sphereVbView[i].SizeInBytes = sizeof(SphereVBData) * vertexCount;
		sphereVbView[i].StrideInBytes = sizeof(SphereVBData);

		//------------------------------------------------------------------------------
	}
}
void CollisionManager::CreateConstBuff()
{
	HRESULT result;
	for (int i = 0; i < 3; i++)
	{
		result = DirectXLib::GetInstance()->GetDevice()->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// �A�b�v���[�h�\
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBuffData) + 0xff) & ~0xff),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constBuff[i]));
		if (FAILED(result)) {
			assert(0);
		}
	}
}

void CollisionManager::SendBoxBuffers(UINT& num)
{
	int bbIndex = DirectXLib::GetInstance()->GetBbIndex();
	// ���_�o�b�t�@�փf�[�^�]��
	int vertCount = 0;
	SphereVBData* vertMap2 = nullptr;
	//auto result = sphereVertBuff->Map(0, nullptr, (void**)&vertMap2);
	BoxVBData* vertMap = nullptr;
	auto result = boxVertBuff[bbIndex]->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		for (auto& it : colliders)
		{
			for (auto& colIt : it.second)
			{
				if (colIt->GetShapeType() != COLLISIONSHAPE_BOX)
					continue;
				BoxCollider* boxCollider = dynamic_cast<BoxCollider*>(colIt);
				vertMap->center = boxCollider->GetObjCenter() + boxCollider->GetOffset();
				vertMap->scale = boxCollider->GetScale();
				vertMap->objCenter = boxCollider->GetObjCenter();
				vertMap->rotation = boxCollider->GetObject3D()->GetRotation();
				vertMap->colRotation = boxCollider->GetRotation();

				// ���̒��_��
				vertMap++;
				//if(vertCount == 1)
				//{
				//	vertMap2->center = XMFLOAT3(boxCollider->center.m128_f32[0], boxCollider->center.m128_f32[1], boxCollider->center.m128_f32[2]);
				//	vertMap2->radius = 0.1f;
				//	vertMap2++;
				//	vertMap2->center = boxCollider->normaDirect[0] * boxCollider->GetScale().x + XMFLOAT3(boxCollider->center.m128_f32[0], boxCollider->center.m128_f32[1], boxCollider->center.m128_f32[2]);
				//	vertMap2->radius = 0.1f;
				//	vertMap2++;
				//	vertMap2->center = boxCollider->normaDirect[1] * boxCollider->GetScale().y + XMFLOAT3(boxCollider->center.m128_f32[0], boxCollider->center.m128_f32[1], boxCollider->center.m128_f32[2]);
				//	vertMap2->radius = 0.1f;
				//	vertMap2++;
				//	vertMap2->center = boxCollider->normaDirect[2] * boxCollider->GetScale().z + XMFLOAT3(boxCollider->center.m128_f32[0], boxCollider->center.m128_f32[1], boxCollider->center.m128_f32[2]);
				//	vertMap2->radius = 0.1f;
				//	vertMap2++;

				//}
				if (++vertCount >= vertexCount) {
					break;
				}
			}
		}
		boxVertBuff[bbIndex]->Unmap(0, nullptr);
		
	}
	num = vertCount;
}

void CollisionManager::SendSphereBuffers(UINT& num)
{
	int bbIndex = DirectXLib::GetInstance()->GetBbIndex();
	// ���_�o�b�t�@�փf�[�^�]��
	int vertCount = 0;
	SphereVBData* vertMap = nullptr;
	auto result = sphereVertBuff[bbIndex]->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		for (auto& it : colliders)
		{
			for (auto& colIt : it.second)
			{
				if (colIt->GetShapeType() != COLLISIONSHAPE_SPHERE)
					continue;
				SphereCollider* sphereCollider = dynamic_cast<SphereCollider*>(colIt);
				vertMap->center = XMFLOAT3(sphereCollider->center.m128_f32[0], sphereCollider->center.m128_f32[1], sphereCollider->center.m128_f32[2]);
				vertMap->radius = sphereCollider->GetRadius();
				// ���̒��_��
				vertMap++;
				if (++vertCount >= vertexCount) {
					break;
				}
			}
		}
		sphereVertBuff[bbIndex]->Unmap(0, nullptr);
	}
	num = vertCount;
}

void CollisionManager::SendConstBuff()
{
	int bbIndex = DirectXLib::GetInstance()->GetBbIndex();

	// �萔�o�b�t�@�փf�[�^�]��
	ConstBuffData* constMap = nullptr;
	auto result = constBuff[bbIndex]->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result))
	{
		constMap->mat = Object3D::GetCamera()->GetMatViewProjection();
		constBuff[bbIndex]->Unmap(0, nullptr);
	}
}
