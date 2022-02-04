#pragma once
#include <array>
#include<forward_list>
#include"CollisionPrimitive.h"
#include"RaycastHit.h"
#include"QueryCallback.h"
#include<d3d12.h>

#include "SphereCollider.h"
#include"Tree.h"

class BaseCollider;
class CollisionManager
{
public://�ÓI�����o�֐�
	static CollisionManager* GetInstance();

public://�����o�֐�
	void AddCollider(BaseCollider* collider);

	inline void RemoveCollider(BaseCollider* collider)
	{
		//auto end_it = spSOFTAry.end();
		for (auto it = spSOFTAry.begin(); it != spSOFTAry.end();)
		{
			if ((*it)->pObject == collider)
			{
				(*it)->Remove();
				it = spSOFTAry.erase(it);
			}
			else
				++it;
		}

		colliders.at(collider->GetObject3D()).remove(collider);
	}
	const std::forward_list<BaseCollider*>* GetColliders(Object* objPtr)
	{
		if (colliders.count(objPtr) == 0)
			return nullptr;
		return &colliders[objPtr];
	}
	void CheckAllCollisions();
	bool CheckHitBox(const Box& box, unsigned short attribute = (unsigned short)0xffffffff);
	/// <summary>
	/// 8���؂̗̈�����肷��
	/// </summary>
	/// <param name="minLine"></param>�̈�̍Œ�ʒu
	/// <param name="maxLine"></param>�̈�̍ō��ʒu
	void Initialize(const Vector3& minLine, const Vector3& maxLine);
	bool Raycast(const Ray& ray, BaseCollider* collider, RaycastHit* hitinfo = nullptr, float maxDistance = D3D12_FLOAT32_MAX);
	bool Raycast(const Ray& ray,BaseCollider* collider,unsigned short attribute, RaycastHit* hitinfo = nullptr, float maxDistance = D3D12_FLOAT32_MAX);
	//���ɂ��ՓˑS����
	void QuerySphere(const Sphere& sphere, QueryCallback* callback, SphereCollider* collider,unsigned short attribute = (unsigned short)0xffffffff);
	//�{�b�N�X�ɂ��ՓˑS����
	void QueryBox(const Box& box, QueryCallback* callback, unsigned short attribute1 = (unsigned short)0xffffffff, unsigned short attribute2 = (unsigned short)0xffffffff, BaseCollider* collider = nullptr);
	void DrawCollider();
private:
	void CreateVBV();
	void CreateConstBuff();
	void SendBoxBuffers(UINT& num);
	void SendSphereBuffers(UINT& num);
	void SendConstBuff();
	CollisionManager() = default;
	CollisionManager(const CollisionManager&) = default;
	~CollisionManager() = default;
	CollisionManager& operator=(const CollisionManager&) = default;
	std::unordered_map<Object*,std::forward_list<BaseCollider*>> colliders;
	Tree::CLiner8TreeManager<BaseCollider> L8Tree;
	std::vector<BaseCollider*> ColVect;	// �ՓˑΏۃ��X�g
	std::vector < SmartPtr<Tree::TreeObject<BaseCollider>>> spSOFTAry;


	bool viewCollider;
	const int vertexCount = 500;
	std::array<ComPtr<ID3D12Resource>,3> boxVertBuff;
	std::array<D3D12_VERTEX_BUFFER_VIEW, 3> boxVbView;
	std::array<ComPtr<ID3D12Resource>, 3> constBuff;
	std::array<ComPtr<ID3D12Resource>, 3> sphereVertBuff;
	std::array<D3D12_VERTEX_BUFFER_VIEW, 3> sphereVbView;
	struct BoxVBData
	{
		XMFLOAT3 center;
		XMFLOAT3 scale;
		XMFLOAT3 objCenter;
		XMFLOAT3 rotation;
		XMFLOAT3 colRotation;
	};
	struct SphereVBData
	{
		XMFLOAT3 center;
		float radius;
	};
	struct ConstBuffData
	{
		XMMATRIX mat;
	};
};

