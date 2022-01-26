#pragma once
#include "Camera.h"
class Window;
class DebugCamera :
    public Camera
{
public:
	DebugCamera();
	void Update();
	void SetDistance(float distance)
	{
		this->distance = distance;
		viewDirty = true;
	}
	void AddPhi(const float angle)
	{
		phi += angle;
		if (phi > 360 * XM_PI / 180.0f)
			phi -= 360 * XM_PI / 180.0f;
		else if (phi < 0)
			phi += 360 * XM_PI / 180.0f;
		viewDirty = true;
	}
	const float GetPhi()
	{
		return phi;
	}
	void SetPhi(const float arg_phi)
	{
		phi = arg_phi;
		if (phi > 360 * XM_PI / 180.0f)
			phi -= 360 * XM_PI / 180.0f;
		else if (phi < 0)
			phi += 360 * XM_PI / 180.0f;
		viewDirty = true;
	}
private:
	//�����_�܂ŋ���
	float distance = 10;
};

