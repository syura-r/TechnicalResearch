#pragma once

class SettingParam
{
public:
	inline static const int GetPadSensitive() { return padSensitive; }
	inline static const int GetReverseX() { return reverseX; }
	inline static const int GetReverseY() { return reverseY; }

	inline static const int GetJumpButton() { return jumpButton; }
	inline static const int GetAirSlideButton() { return airSlideButton; }
	inline static const int GetRunButton() { return runButton; }
	inline static const int GetResetButton() { return resetButton; }

	inline static const bool GetViewCollision() { return viewCollision; }
	inline static const bool GetOnSSAO() { return onSSAO; }


private:
	//Pad���_���x
	static int padSensitive;
	//���_���씽�]�p(1��-1,-1�Ŕ��])
	static int reverseX;
	static int reverseY;

	//����ύX�p�ϐ�
	static int jumpButton;
	static int airSlideButton;
	static int runButton;
	static int resetButton;

	static bool viewCollision;

	static bool onSSAO;
	
	friend class Menu;
};

