#pragma once
#include "BoxCollider.h"
#include "TouchAbleObject.h"

class Floor :
    public Object
{
public:
	Floor();
	~Floor();
	void Update() override;
private:
	std::vector<BoxCollider*> colliders;
};

