#pragma once
#include "Object.h"

class Sky:public Object
{
public:
	Sky();
	void Draw()override;
	void DrawReady() override;
};

