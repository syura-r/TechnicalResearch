#pragma once
#define DIRECTINPUT_VERSION 0x0800 //DirectInput�̃o�[�W����
#include <dinput.h>
#include <Windows.h>
#include <wrl.h>
#include<DirectXMath.h>
#include <Xinput.h>

#pragma comment(lib, "xinput.lib")
//https://docs.microsoft.com/en-us/windows/win32/api/xinput/ns-xinput-xinput_gamepad
static struct MouseMove
{
	LONG lX;
	LONG lY;
	LONG lZ;
};
class Input
{
	struct float2
	{
		float x;
		float y;
	};
private:
template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	static enum MOUSEBOTTAN
	{
		LEFT,
		RIGHT,
		MIDDLE
	};
	
	static void Update();
	static bool TriggerKey(BYTE keyCode);//�L�[�{�[�h�̃g���K�[����
	static bool DownKey(BYTE keyCode);//�L�[�{�[�h�̉����Ă���Ԕ���
	static bool UpKey(BYTE keyCode);//�L�[�{�[�h�̗������Ƃ�����
	static bool TriggerMouseKey(MOUSEBOTTAN bottan);//�}�E�X�̃g���K�[����
	static bool DownMouseKey(MOUSEBOTTAN bottan);//�}�E�X�̉����Ă���Ԕ���
	static bool UpMouseKey(MOUSEBOTTAN bottan);//�}�E�X�̗������Ƃ�����
	static MouseMove GetMouseMove();
	static void Initialize(HWND hwnd);
	static DirectX::XMFLOAT2 GetMousePos() { 
		return mousePos;
	};

	//�Q�[���p�b�h
	static bool CheckPadButton(int button);
	static bool TriggerPadButton(int button);
	static bool UpPadButton(int button);

	static bool CheckPadRightTrigger(); //�E�g���K�[
	static bool CheckPadLeftTrigger();  //���g���K�[

	static bool TriggerPadRightTrigger(); //�E�g���K�[�̃g���K�[����
	static bool TriggerPadLeftTrigger(); //���g���K�[�̃g���K�[����
	//�E�X�e�B�b�N
	static bool CheckPadRStickRight();
	static bool CheckPadRStickLeft();
	static bool CheckPadRStickUp();
	static bool CheckPadRStickDown();

	static bool TriggerPadRStickRight();
	static bool TriggerPadRStickLeft();
	static bool TriggerPadRStickUp();
	static bool TriggerPadRStickDown();

	//���X�e�B�b�N
	static bool CheckPadLStickRight();
	static bool CheckPadLStickLeft();
	static bool CheckPadLStickUp();
	static bool CheckPadLStickDown();

	static bool TriggerPadLStickRight();
	static bool TriggerPadLStickLeft();
	static bool TriggerPadLStickUp();
	static bool TriggerPadLStickDown();

	static float2 GetLStickDirection();
	static float2 GetRStickDirection();

	//�Q�[���p�b�h�U��
	static void LeftVibration(unsigned int power, int setTime);
	static void RightVibration(unsigned int power, int setTime);
	static void Vibration(unsigned int power, int setTime);

	//�����̃{�^���������ꂽ�牟���ꂽ�{�^����Ԃ�
	static int CheckAnyPadButton();
	
	static void ChangeMouseMove()
	{
		if (mouseMove == true)
		{
			while (1)
			{
				if (ShowCursor(false) < 0)
					break;
			}
			mouseMove = false;
		}
		else
		{
			while (1)
			{
				if (ShowCursor(true) >= 0)
					break;
			}
			mouseMove = true;
		}
	}
private:
	static ComPtr<IDirectInputDevice8> devkeyboard;
	static ComPtr<IDirectInputDevice8> devPad;
	static ComPtr<IDirectInputDevice8> devMouse;
	static BYTE keys[256];
	static BYTE prevKeys[256];
	static DIMOUSESTATE2 mouseState ;
	static DIMOUSESTATE2 mouseStatePre;
	static int num;//�p�b�h�̏������Ɏg�p
	static int pad_LStickX, pad_LStickY;
	static int prev_LStickX, prev_LStickY;
	static DirectX::XMFLOAT2 mousePos;
	
	static XINPUT_STATE padState;
	static XINPUT_STATE prevPadState;
	static XINPUT_VIBRATION vibration;
	static int time;

	static float screenWidth;
	static float screenHeight;

	static bool mouseMove;
};

