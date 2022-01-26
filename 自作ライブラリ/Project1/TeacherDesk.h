#pragma once
#include "BoxCollider.h"
#include "GSShaderTestObj.h"

class TeacherDesk :
    public GSShaderTestObj
{
public:
	TeacherDesk();
	~TeacherDesk();
	void Update()override;
private:
	std::vector<BoxCollider*> colliders;
	float colliderOffset[9][3] =
	{
		-1.387f, 1.645f, 0.935f,
		0.000f , 3.355f, 0.000f,
		0.000f , 1.871f, 0.962f,
		1.387f , 1.645f, 0.935f,
		1.407f , 1.645f, -0.925f,
		1.388f , 1.863f, 0.000f,
		-1.388f, 1.863f, 0.000f,
		-1.388f, 1.635f,-0.932f,
		0.000f , 2.319f,-0.019f,

	};
	float colliderScale[9][3]
	{
		0.081f , 1.616f , 0.079f,
		1.742f , 0.097f , 1.277f,
		1.326f , 1.394f , 0.043f,
		0.087f , 1.645f , 0.065f,
		0.086f , 1.643f , 0.080f,
		0.057f , 1.409f , 0.861f,
		0.057f , 1.409f , 0.861f,
		0.074f , 1.625f , 0.074f,
		1.335f , 0.086f , 0.873f,
	};
	float colliderRotation[9][3]
	{
		0 , 0 , 0,
		0 , 0 , 0,
		0 , 0 , 0,
		0 , 0 , 0,
		0 , 0 , 0,
		0 , 0 , 0,
		0 , 0 , 0,
		0 , 0 , 0,
		0 , 0 , 0,
	};

	bool hit = false;

};


