#pragma once
#include <array>

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

	std::array<float,3> colliderOffset =
	{
		0 , 1.5f , 0,
	};
	std::array<float, 3> colliderScale = 
	{
		1.179f , 1.5f , 0.534f,
	};
	std::array<float, 3> colliderRotation = 
	{
		0 , 0 , 0,
	};

	bool hit = false;

};

