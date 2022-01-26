#include "Fluorescent.h"

#include "OBJLoader.h"

Fluorescent::Fluorescent(const Vector3& pos)
{
	Create(OBJLoader::GetModel("Fluorescent"));
	position = pos;
	scale = 3.5f;
	rotation = { 0,90,0 };
}
