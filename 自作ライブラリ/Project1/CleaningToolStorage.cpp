#include "CleaningToolStorage.h"
#include "BaseCollider.h"
#include "BoxCollider.h"
#include "CollisionAttribute.h"
#include "CollisionManager.h"
#include "FBXManager.h"
#include "OBJLoader.h"
#include "Texture.h"

CleaningToolStorage::CleaningToolStorage()
{
	Create(OBJLoader::GetModel("cleaningToolStorage"));
	position = { 22.9f,-4.95f,-33.3f };
	scale = { -2.122f,2,2 };
	Object::Update();
	collider = new BoxCollider({ 0,10.0f,0,0 }, Vector3{ -2,5,1.5f } * scale);
	collider->SetAttribute(COLLISION_ATTR_LANDSHAPE);
	collider->SetMove(true);
	SetCollider(collider);
}

void CleaningToolStorage::Update()
{
	Object::Update();
}

void CleaningToolStorage::Draw()
{
	float pos[3];
	memcpy(pos, &position, sizeof(float) * 3);
	float sc[3];
	memcpy(sc, &scale, sizeof(float) * 3);
#ifdef _Debug
	if (!Object3D::GetDrawShadow()&&DrawMode::GetDrawImGui())
	{

		ImGui::Begin("DeskState");
		ImGui::SliderFloat3("position", pos, -40, 40);
		ImGui::SliderFloat3("scale", sc, 0, 3);

		position = pos;
		scale = sc;
		ImGui::End();
	}
#endif
	CustomDraw(true, true, ALPHA, false);
}

