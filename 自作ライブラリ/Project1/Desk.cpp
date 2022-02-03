#include "Desk.h"
#include "BaseCollider.h"
#include "BoxCollider.h"
#include "CollisionAttribute.h"
#include "CollisionManager.h"
#include "OBJLoader.h"
#include"PtrDelete.h"
#include "Texture.h"

Desk::Desk()
{
	Create(OBJLoader::GetModel("desk"));
	position = { 0,-5,0 };
	scale = { 9,9,9 };
	Object::Update();
	for (int i = 0; i < 10; i++)
	{
		auto adjustScale = scale / 5;//scale‚ª5‚ÌŽž‚ðŠî€‚É’²ß‚µ‚½’l‚¾‚©‚çscale‚ð5‚ÅŠ„‚Á‚Ä‚é
		const auto colOffset = XMVECTOR{ colliderOffset[i][0] * adjustScale.x,colliderOffset[i][1] * adjustScale.y,colliderOffset[i][2] * adjustScale.z,0 };
		const Vector3 colScale = Vector3{ colliderScale[i][0], colliderScale[i][1], colliderScale[i][2] } *adjustScale;
		BoxCollider* col = new BoxCollider(colOffset, colScale);
		col->SetObject(this);
		col->SetMove(true);
		col->SetRotation({ colliderRotation[i][0],colliderRotation[i][1],colliderRotation[i][2] });
		col->SetAttribute(COLLISION_ATTR_LANDSHAPE);
		col->Update();
		CollisionManager::GetInstance()->AddCollider(col);
		colliders.push_back(col);
	}
}

Desk::~Desk()
{
	for (auto& it : colliders)
	{
		CollisionManager::GetInstance()->RemoveCollider(it);
		PtrDelete(it);
	}
}
void Desk::Update()
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

