#include "Chair.h"

#include "BaseCollider.h"
#include "BoxCollider.h"
#include "CollisionAttribute.h"
#include "CollisionManager.h"
#include "OBJLoader.h"
#include "Texture.h"

Chair::Chair()
{
	Create(OBJLoader::GetModel("chair"));
	position = { 0,-5,-3.7f };
	scale = 9;
	Object::Update();
	for (int i = 0; i < 8; i++)
	{
		auto adjustScale = scale / 5;//scaleが5の時を基準に調節した値だからscaleを5で割ってる
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


void Chair::Initialize()
{
}

void Chair::Update()
{
	if(hit)
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
