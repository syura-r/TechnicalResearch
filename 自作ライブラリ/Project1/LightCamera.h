#pragma once
#include"Camera.h"
class LightCamera : public Camera
{
public:
	LightCamera();
	void SetLightDir(const Vector3& dir);
	void SetDistance(const float dis)
	{
		//������0�Ȃ炵�Ȃ��Ń��^�[��
		if(dis == 0)
			return;
		distance = dis;
	};
private:
	float distance;
	float phi;
	float theta;

};

