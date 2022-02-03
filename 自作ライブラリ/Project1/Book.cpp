#include "Book.h"
#include "BaseCollider.h"
#include "BoxCollider.h"
#include "CollisionAttribute.h"
#include "CollisionManager.h"
#include "OBJLoader.h"
#include"PtrDelete.h"
#include "Texture.h"

Book::Book()
{
	Create(OBJLoader::GetModel("book1"));
	position = { 0,-5,-3.7f };
	scale = {-3,3,3};
	Object::Update();
	auto adjustScale = scale / 3;
	adjustScale.x *= -1;
	const auto colOffset = XMVECTOR{ colliderOffset[0] * adjustScale.x,colliderOffset[1] * adjustScale.y,colliderOffset[2] * adjustScale.z,0 };
	const Vector3 colScale = Vector3{ colliderScale[0], colliderScale[1], colliderScale[2] } *adjustScale;
	BoxCollider* col = new BoxCollider(colOffset, colScale);
	col->SetObject(this);
	col->SetMove(true);
	col->SetRotation({ colliderRotation[0],colliderRotation[1],colliderRotation[2] });
	col->SetAttribute(COLLISION_ATTR_LANDSHAPE);
	col->Update();
	CollisionManager::GetInstance()->AddCollider(col);
	colliders.push_back(col);
}

Book::~Book()
{
	for (auto& it : colliders)
	{
		CollisionManager::GetInstance()->RemoveCollider(it);
		PtrDelete(it);
	}
}

void Book::Update()
{
	if (hit)
	{
		color = { 1,1,1,1 };
		hit = false;
	}
	Object::Update();
	for (auto& it : colliders)
	{
		it->Update();
	}
}

