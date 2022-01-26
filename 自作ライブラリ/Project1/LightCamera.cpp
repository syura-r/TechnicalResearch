#include "LightCamera.h"

LightCamera::LightCamera()
{
	Camera();
	distance = 10.0f;
}

void LightCamera::SetLightDir(const Vector3& dir)
{
	//�����x�N�g�����Ȃ��ꍇ�����^�[��
	if (dir.x == 0 && dir.y == 0 && dir.z == 0)
		return;
	eye = -1 * dir * distance + target;
	
	viewDirty = true;
	Update();
}
