#include "SetSquare45.h"

#include <fstream>

#include "CollisionAttribute.h"
#include "CollisionManager.h"
#include "DrawMode.h"
#include "OBJLoader.h"
#include "Texture.h"

SetSquare45::SetSquare45()
{
	Create(OBJLoader::GetModel("setSquare45"));
	position = { 0,-4.5f,5.0f };
	scale = { 5,5,5 };
	Object::Update();
	for (int i = 0; i < ColliderNum; i++)
	{
		auto adjustScale = scale / 8;//scale‚ª5‚ÌŽž‚ðŠî€‚É’²ß‚µ‚½’l‚¾‚©‚çscale‚ð5‚ÅŠ„‚Á‚Ä‚é
		const auto colOffset = XMVECTOR{ colliderOffset[i][0] * adjustScale.x,colliderOffset[i][1] * adjustScale.y,colliderOffset[i][2] * adjustScale.z,0 };
		const Vector3 colScale = Vector3{ colliderScale[i][0], colliderScale[i][1], colliderScale[i][2] } *adjustScale;
		std::unique_ptr<BoxCollider> col(new BoxCollider(colOffset, colScale));
		col->SetObject(this);
		col->SetMove(true);
		col->SetRotation({ colliderRotation[i][0],colliderRotation[i][1],colliderRotation[i][2] });
		col->SetAttribute(COLLISION_ATTR_LANDSHAPE);
		col->Update();
		CollisionManager::GetInstance()->AddCollider(col.get());
		colliders.push_back(std::move(col));

	}
}

SetSquare45::~SetSquare45()
{
	for (auto& it : colliders)
	{
		CollisionManager::GetInstance()->RemoveCollider(it.get());
	}

}
void SetSquare45::Update()
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
