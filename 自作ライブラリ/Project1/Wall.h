#pragma once
#include "Object.h"
#include "BoxCollider.h"
#include "GSShaderTestObj.h"

class Wall :
    public GSShaderTestObj
{
public:
	Wall(const std::string& modelName);
	~Wall();
	void Update()override;
	void AddCollider(const float offset[3], const float colScale[3]);
private:
	std::vector<std::unique_ptr<BoxCollider>> colliders;

	bool hit = false;


};


