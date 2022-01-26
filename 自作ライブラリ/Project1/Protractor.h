#pragma once
#include "BoxCollider.h"
#include "GSShaderTestObj.h"
//分度器のクラスだけど使ってない
class Protractor :
    public GSShaderTestObj
{
public:
	Protractor();
	~Protractor();
	void Initialize()override;
	void Update()override;
	void Draw()override;
	void OnCollision(const CollisionInfo& info) override;
private:
	std::vector<BoxCollider*> colliders;
	float colliderOffset[8][3] =
	{
		-0.77f , 0.96f , 0.788f,
		-0.007f, 1.96f , 0.048f,
		-0.02f , 3.525f, -0.828f,
		0.772f , 0.946f, 0.815f,
		0.866f , 1.00f , -0.842f,
		0.812f , 2.623f, -0.845f,
		-0.799f, 2.623f, -0.828f,
		-0.866f, 0.984f, -0.860f,
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

