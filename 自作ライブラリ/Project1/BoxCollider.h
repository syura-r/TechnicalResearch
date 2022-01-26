#pragma once
#include "BaseCollider.h"
#include "CollisionPrimitive.h"

class BoxCollider : public BaseCollider ,public Box
{
public:
	BoxCollider(XMVECTOR offset = { 0,0,0,0 },Vector3 scale ={1,1,1}) :
		offset(offset),scale(scale)
	{
		shapeType = COLLISIONSHAPE_BOX;
	}

	void Update()override;
	inline const Vector3& GetMax() override
	{
		return maxPosition;
	}
	inline const Vector3& GetMin() override
	{
		return minPosition;
	}
	

	inline void SetScale(const Vector3& scale) { this->scale = scale; }

	inline void SetRotation(const Vector3& rot) { this->colliderRot = rot; }
	
	inline const XMVECTOR& GetOffset() { return offset; }

	inline void SetOffset(const XMVECTOR& offset) { this->offset = offset; }

	inline const Vector3& GetScale() { return scale; }

	inline const Vector3& GetRotation() { return colliderRot; }

	inline const Vector3& GetObjCenter() { return objCenter; }

	inline const Vector3& GetCenter() { return center; }

	//�I�u�W�F�N�g��Collider�ǂ��炩�ł���]���Ă�����true��Ԃ�
	bool CheckRotation()
	{
		if (rotation != 0 || colliderRot != 0)
			return true;
		return false;
	}

private:
	void ClucCenter(const XMMATRIX& matRot);
	void ClucMinMaxPoint(const XMMATRIX& colMatRot, const XMMATRIX& matRot);
	void ClucDirect(const XMMATRIX& colMatRot, const XMMATRIX& matRot);
	XMVECTOR offset;
	Vector3 scale;
	Vector3 colliderRot = {};
	Vector3 objCenter;
};

