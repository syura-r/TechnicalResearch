#pragma once
#include "BoxCollider.h"
#include "GSShaderTestObj.h"
class Book :
    public GSShaderTestObj
{
public:
	Book();
	~Book();
	void Update()override;
private:
	std::vector<std::unique_ptr<BoxCollider>> colliders;

	float colliderOffset[3] =
	{
		0 , 1.5f , 0,
	};
	float colliderScale[3]
	{
		1.179f , 1.5f , 0.534f,
	};
	float colliderRotation[3]
	{
		0 , 0 , 0,
	};

	bool hit = false;

};

