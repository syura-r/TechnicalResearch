#pragma once
#include"ParticleParameter.h"
#include<d3d12.h>
class Particle
{
protected:
public:
	ParticleParameter parameter;
	OutputData vsParam;
public://�����o�֐�
	Particle(){};
	virtual ~Particle(){};
	void Dead() { vsParam.isDead = true; }
	bool GetisDead(){ return vsParam.isDead; }


};

