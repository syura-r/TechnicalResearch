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
	void AddCollider(const float* offset, const float* colScale);
private:
	std::vector<std::unique_ptr<BoxCollider>> colliders;

	bool hit = false;


};


