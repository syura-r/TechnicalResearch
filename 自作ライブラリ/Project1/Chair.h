#pragma once
#include "BoxCollider.h"
#include "GSShaderTestObj.h"
class Chair :
    public GSShaderTestObj
{
public:
	Chair();
	void Initialize()override;
	void Update()override;
private:
	std::vector<std::unique_ptr<BoxCollider>> colliders;

	float colliderOffset[8][3] = 
	{
		-0.8656f , 0.96f , 0.84675f,
		-0.1026f, 1.96f , 0.10675f,
		-0.1156f, 3.525f, -0.76925f,
		0.6764f , 0.946f, 0.87375f,
		0.7704f , 1.00f , -0.78325f,
		0.7164f , 2.623f, -0.78625f,
		-0.8946f, 2.623f, -0.76925f,
		-0.9616f, 0.984f, -0.80125f,
	};
	float colliderScale[8][3]
	{
		0.133f , 0.933f , 0.10f,
		0.856f , 0.090f , 0.766f,
		0.900f , 0.443f , 0.153f,
		0.090f , 0.900f , 0.090f,
		0.090f , 0.964f , 0.090f,
		0.148f , 0.566f , 0.090f,
		0.148f , 0.566f , 0.090f,
		0.090f , 0.971f , 0.090f,
	};
	float colliderRotation[8][3]
	{
		-4.4f , 0 , 0,
		0 , 0 , 0,
		-3 , 0 , 0,
		-3 , 0 , 0,
		4 , 0 , 0,
		-4.5f , 0 , 0,
		-3 , 0 , 0,
		4 , 0 , 0,
	};

	bool hit = false;
};

