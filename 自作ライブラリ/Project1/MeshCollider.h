#pragma once

#include"BaseCollider.h"
#include"CollisionPrimitive.h"

#include<DirectXMath.h>
#include "OBJModel.h"

class MeshCollider:public BaseCollider
{
public:
	MeshCollider()
	{
		//���b�V���`����Z�b�g
		shapeType = COLLISIONSHAPE_MESH;
		maxPoint = {};
		minPoint = {};
	}

	//�O�p�`�̔z����\�z����
	void ConstructTriangles(OBJModel* model);

	void Update()override;
	inline const Vector3& GetMax() override
	{
		auto point = XMVector3Transform(maxPoint, object->GetMatWorld());
		return Vector3(point.m128_f32[0], point.m128_f32[1], point.m128_f32[2]);
	}
	inline const Vector3& GetMin() override
	{
		auto point = XMVector3Transform(minPoint, object->GetMatWorld());
		return Vector3(point.m128_f32[0], point.m128_f32[1], point.m128_f32[2]);
	}

	//���Ƃ̓����蔻��
	bool CheckCollisionSphere(const Sphere& sphere, DirectX::XMVECTOR* inter = nullptr,DirectX::XMVECTOR* reject = nullptr);

	//���C�Ƃ̓����蔻��
	bool CheckCollisionRay(const Ray& ray, float* distance = nullptr, DirectX::XMVECTOR* inter = nullptr);


private:

	void CheckMaxMinPoint(const XMVECTOR& point);
	std::vector<Triangle> triangles;
	//���[���h�s��̋t�s��
	DirectX::XMMATRIX invMatWorld;
	////�O�t���[���̃��[���h�s��̋t�s��
	//DirectX::XMMATRIX preInvMatWorld;
	//��Ԓl�̏������ʒu�ɂ��钸�_
	XMVECTOR minPoint;
	//��Ԓl�̑傫���ʒu�ɂ��钸�_
	XMVECTOR maxPoint;
};