#pragma once
#include "BoxCollider.h"
#include "GSShaderTestObj.h"
class Desk :
    public GSShaderTestObj
{
public:
	Desk();
	~Desk();
	void Update()override;
private:
	std::vector<std::unique_ptr<BoxCollider>> colliders;
	std::array<std::array<float, 3>, 10> colliderOffset =
	{
		-1.318f, 1.510f, 0.960f,
		0.000f , 3.140f, 0.000f,
		0.000f , 0.409f, 0.962f,
		1.322f , 1.514f, 0.947f,
		1.346f , 1.466f, -0.97f,
		1.322f , 0.409f, 0.012f,
		-1.33f , 0.397f, 0.018f,
		-1.33f , 1.526f,-0.968f,
		0.000f , 2.774f, 0.019f,
		0.000f , 2.358f, 0.906f,
	};
	std::array<std::array<float, 3>, 10> colliderScale =
	{
		0.081f , 1.500f , 0.079f,
		1.477f , 0.058f , 1.177f,
		1.302f , 0.047f , 0.043f,
		0.087f , 1.500f , 0.065f,
		0.058f , 1.450f , 0.058f,
		0.050f , 0.050f , 1.010f,
		0.053f , 0.053f , 1.011f,
		0.069f , 1.492f , 0.050f,
		1.274f , 0.340f , 0.985f,
		1.308f , 0.040f , 0.051f,

	};
	std::array<std::array<float, 3>, 10> colliderRotation =
	{
		-4.4f , 0 , 0,
		0     , 0 , 0,
		0     , 0 , 0,
		-4.4f , 0 , 0,
		4     , 0 , 0,
		0     , 0 , 0,
		0     , 0 , 0,
		4     , 0 , 0,
		0     , 0 , 0,
		0     , 0 , 0,
	};

	bool hit = false;

};

