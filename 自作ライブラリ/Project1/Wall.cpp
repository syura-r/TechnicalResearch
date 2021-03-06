#include "Wall.h"
#include "BoxCollider.h"
#include "CollisionAttribute.h"
#include "CollisionManager.h"
#include "OBJLoader.h"
#include "Texture.h"

Wall::Wall(const std::string& modelName)
{
	Create(OBJLoader::GetModel(modelName));
	position = { 0,-5.3f,0 };
	scale = { 3.5f,3.5f,-3.5f };
	rotation = { 0,90,0 };
	Object::Update();
}

Wall::~Wall()
{
}

void Wall::Update()
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

void Wall::AddCollider(const float* offset, const float* colScale)
{
	auto adjustScale = scale / 3.5f;//scaleが3.5の時を基準に調節した値だからscaleを3.5で割ってる
	adjustScale.z *= -1;
	const auto colOffset = XMVECTOR{ offset[0] * adjustScale.x,offset[1] * adjustScale.y,offset[2] * adjustScale.z,0 };
	const auto colliderScale = Vector3{ colScale[2], colScale[1], colScale[0] } *adjustScale;
	std::unique_ptr<BoxCollider> col(new BoxCollider(colOffset, colliderScale));
	col->SetObject(this);
	col->SetRotation({ 0,-90,0 });
	col->SetMove(true);
	col->SetAttribute(COLLISION_ATTR_LANDSHAPE);
	col->Update();
	CollisionManager::GetInstance()->AddCollider(col.get());
	colliders.push_back(std::move(col));

}
