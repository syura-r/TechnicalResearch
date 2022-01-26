#include "Input.h"
#pragma comment(lib,"dinput8.lib")
#include<cassert>
using namespace Microsoft::WRL;

ComPtr<IDirectInputDevice8> Input::devkeyboard = nullptr;
ComPtr<IDirectInputDevice8> Input::devPad = nullptr;
ComPtr<IDirectInputDevice8> Input::devMouse = nullptr;

BYTE Input::keys[256] = {};
BYTE Input::prevKeys[256] = {};
DIMOUSESTATE2 Input::mouseState = {};
DIMOUSESTATE2 Input::mouseStatePre = {};
int Input::num = 0;//�p�b�h�̏������Ɏg�p
int Input::pad_LStickX = 0;
int Input::pad_LStickY = 0;
int Input::prev_LStickX = 0;
int Input::prev_LStickY = 0;
DirectX::XMFLOAT2 Input::mousePos = { 0,0 };

XINPUT_STATE Input::padState = {};
XINPUT_STATE Input::prevPadState = {};
XINPUT_VIBRATION Input::vibration = {};
int Input::time = 0;

float Input::screenHeight = 0;
float Input::screenWidth = 0;

bool Input::mouseMove = true;
#define STICKMAX 32767


void Input::Initialize(HWND hwnd)
{
	HRESULT result;
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);
	//while (1)
	//{
	//	if (ShowCursor(false) < 0)
	//		break;
	//}
	num = 1;
	pad_LStickX = 0;
	pad_LStickY = 0;
	//DirectInput�I�u�W�F�N�g�̐���
	IDirectInput8* dinput = nullptr;
	result = DirectInput8Create(
		GetModuleHandle(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, nullptr);
	assert(SUCCEEDED(result));
	IDirectInput8* dinput2 = nullptr;
	result = DirectInput8Create(
		GetModuleHandle(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput2, nullptr);
	assert(SUCCEEDED(result));

	//�L�[�{�[�h�f�o�C�X�̐���
	devkeyboard = nullptr;
	result = dinput->CreateDevice(GUID_SysKeyboard, &devkeyboard, NULL);
	assert(SUCCEEDED(result));

	//���̓f�[�^�`���̃Z�b�g
	result = devkeyboard->SetDataFormat(&c_dfDIKeyboard);//�W���`��
	assert(SUCCEEDED(result));
	//
	//�r�����䃌�x���̃Z�b�g
	result = devkeyboard->SetCooperativeLevel(
		hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));

	//SetCursorPos(0,0);
	// �}�E�X�f�o�C�X�̐���	
	result = dinput->CreateDevice(GUID_SysMouse, &devMouse, NULL);
	assert(SUCCEEDED(result));
	// ���̓f�[�^�`���̃Z�b�g
	result = devMouse->SetDataFormat(&c_dfDIMouse2); // �W���`��
	assert(SUCCEEDED(result));
	// �r�����䃌�x���̃Z�b�g
	result = devMouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));

	ZeroMemory(&padState, sizeof(XINPUT_STATE));
	ZeroMemory(&prevPadState, sizeof(XINPUT_STATE));
	ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
	//�p�b�h�f�o�C�X�̐���
	//devPad = nullptr;
	//result = dinput2->CreateDevice(GUID_Joystick, &devPad, NULL);
	////���̓f�[�^�`���̃Z�b�g
	//result = devPad->SetDataFormat(&c_dfDIJoystick);//�W���`��
	////�r�����䃌�x���̃Z�b�g
	//result = devPad->SetCooperativeLevel(
	//	hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);

}

bool Input::CheckPadButton(int button)
{
	return padState.Gamepad.wButtons & button;
}

bool Input::TriggerPadButton(int button)
{
	return (padState.Gamepad.wButtons & button) && !(prevPadState.Gamepad.wButtons & button);
}

bool Input::UpPadButton(int button)
{
	return !(padState.Gamepad.wButtons & button) && prevPadState.Gamepad.wButtons & button;
}

bool Input::CheckPadRightTrigger()
{
	return padState.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
}

bool Input::CheckPadLeftTrigger()
{
	return padState.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
}

bool Input::TriggerPadRightTrigger()
{
	return (padState.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD &&
		!(prevPadState.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD));
}

bool Input::TriggerPadLeftTrigger()
{
	return (padState.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD &&
		!(prevPadState.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD));
}

bool Input::CheckPadRStickRight()
{
	return padState.Gamepad.sThumbRX > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
}

bool Input::CheckPadRStickLeft()
{
	return padState.Gamepad.sThumbRX < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
}

bool Input::CheckPadRStickUp()
{
	return padState.Gamepad.sThumbRY > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
}

bool Input::CheckPadRStickDown()
{
	return padState.Gamepad.sThumbRY < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
}

bool Input::TriggerPadRStickRight()
{
	return (padState.Gamepad.sThumbRX > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
		!(prevPadState.Gamepad.sThumbRX > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE));
}

bool Input::TriggerPadRStickLeft()
{
	return (padState.Gamepad.sThumbRX < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
		!(prevPadState.Gamepad.sThumbRX < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE));
}

bool Input::TriggerPadRStickUp()
{
	return (padState.Gamepad.sThumbRY > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
		!(prevPadState.Gamepad.sThumbRY > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE));
}

bool Input::TriggerPadRStickDown()
{
	return (padState.Gamepad.sThumbRY < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
		!(prevPadState.Gamepad.sThumbRY < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE));
}

bool Input::CheckPadLStickRight()
{
	return padState.Gamepad.sThumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
}

bool Input::CheckPadLStickLeft()
{
	return padState.Gamepad.sThumbLX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
}

bool Input::CheckPadLStickUp()
{
	return padState.Gamepad.sThumbLY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
}

bool Input::CheckPadLStickDown()
{
	return padState.Gamepad.sThumbLY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
}

void Input::Update()
{
	HRESULT result;
	//�L�[�{�[�h���̎擾�J�n
	result = devkeyboard->Acquire();
	//assert(SUCCEEDED(result));

	memcpy(prevKeys, keys, sizeof(keys));
	//for (int i = 0; i < 256; i++)
	//{
	//	prevKeys[i] = keys[i];
	//}
	//�S�L�[�̓��͏�Ԃ��擾����
	result = devkeyboard->GetDeviceState(sizeof(keys), keys);
	//assert(SUCCEEDED(result));

	//�}�E�X
	result = devMouse->Acquire();	// �}�E�X����J�n
	//assert(SUCCEEDED(result));
	
	// �O��̓��͂�ۑ�
	mouseStatePre = mouseState;
	// �}�E�X�̓���
	result = devMouse->GetDeviceState(sizeof(mouseState), &mouseState);
	mousePos.x += mouseState.lX;
	mousePos.y += mouseState.lY;
	//if(DownKey(DIK_LSHIFT))
	//{
	//	if (mousePos.x < 0)
	//		mousePos.x = 0;
	//	if (mousePos.y < 0)
	//		mousePos.y = 0;
	//	if (mousePos.x > x)
	//		mousePos.x = (float)x;
	//	if (mousePos.y > y)
	//		mousePos.y = (float)y;
	//}
	if(!mouseMove)
	{
		SetCursorPos(screenWidth/2,screenHeight/2);
	}

	//�p�b�h���̎擾�J�n
	//result = devPad->Acquire();
	//for (int i = 0; i < 256; i++)
	//{
	//	prevPads[i] = pads[i];
	//}
	////�S�L�[�̓��͏�Ԃ��擾����
	//result = devkeyboard->GetDeviceState(sizeof(pads), pads);

	prevPadState = padState;
	XInputGetState(0, &padState);
	if (time > -1) { time--; };

	if (time <= 0)
	{
		vibration.wLeftMotorSpeed = 0;
		vibration.wRightMotorSpeed = 0;
	}
	XInputSetState(0, &vibration);
}

bool Input::TriggerKey(BYTE keyCode)
{
	if (keyCode > 255 || keyCode < 0)
		return false;
	if (keys[keyCode] != 0 && prevKeys[keyCode] == 0)
	{
		return true;
	}
	return false;
}

bool Input::DownKey(BYTE keyCode)
{
	if (keyCode > 255 || keyCode < 0)
		return false;
	if (keys[keyCode] != 0)
	{
		return true;
	}
	return false;
}

bool Input::UpKey(BYTE keyCode)
{
	if (keyCode > 255 || keyCode < 0)
		return false;
	if (keys[keyCode] == 0 && prevKeys[keyCode] != 0)
	{
		return true;
	}
	return false;
}

bool Input::TriggerMouseKey(MOUSEBOTTAN bottan)
{
	if (!mouseStatePre.rgbButtons[(int)bottan] && mouseState.rgbButtons[(int)bottan]) {
		return true;
	}
	return false;
}

bool Input::DownMouseKey(MOUSEBOTTAN bottan)
{
	if (mouseState.rgbButtons[(int)bottan]) {
		return true;
	}
	return false;
}

bool Input::UpMouseKey(MOUSEBOTTAN bottan)
{
	if (mouseStatePre.rgbButtons[(int)bottan] && !mouseState.rgbButtons[(int)bottan]) {
		return true;
	}
	return false;
}

MouseMove Input::GetMouseMove()
{
	MouseMove tmp;
	tmp.lX = mouseState.lX;
	tmp.lY = mouseState.lY;
	tmp.lZ = mouseState.lZ;
	return tmp;
}


bool Input::TriggerPadLStickRight()
{
	return (padState.Gamepad.sThumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		!(prevPadState.Gamepad.sThumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE));
}

bool Input::TriggerPadLStickLeft()
{
	return (padState.Gamepad.sThumbLX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		!(prevPadState.Gamepad.sThumbLX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE));
}

bool Input::TriggerPadLStickUp()
{
	return (padState.Gamepad.sThumbLY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		!(prevPadState.Gamepad.sThumbLY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE));
}

bool Input::TriggerPadLStickDown()
{
	return (padState.Gamepad.sThumbLY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		!(prevPadState.Gamepad.sThumbLY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE));
}

Input::float2 Input::GetLStickDirection()
{
	float2 vec;
	if ((padState.Gamepad.sThumbLX <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		padState.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
		(padState.Gamepad.sThumbLY <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
			padState.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
	{
		vec.x = padState.Gamepad.sThumbLX = 0;
		vec.y = padState.Gamepad.sThumbLY = 0;
	}
	else
	{
		vec.x = padState.Gamepad.sThumbLX;
		vec.y = padState.Gamepad.sThumbLY;
	}
	vec.x /= STICKMAX;
	vec.y /= STICKMAX;
	return vec;
}

Input::float2 Input::GetRStickDirection()
{
	float2 vec;
	if ((padState.Gamepad.sThumbRX <  XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
		padState.Gamepad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) &&
		(padState.Gamepad.sThumbRY <  XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
			padState.Gamepad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE))
	{
		vec.x = padState.Gamepad.sThumbRX = 0;
		vec.y = padState.Gamepad.sThumbRY = 0;
	}
	else
	{
		vec.x = padState.Gamepad.sThumbRX;
		vec.y = padState.Gamepad.sThumbRY;
	}


	vec.x /= STICKMAX;
	vec.y /= STICKMAX;
	return vec;
}

void Input::LeftVibration(unsigned int power, int setTime)
{
	time = setTime;
	vibration.wLeftMotorSpeed = power;
}

void Input::RightVibration(unsigned int power, int setTime)
{
	time = setTime;
	vibration.wRightMotorSpeed = power;
}

void Input::Vibration(unsigned int power, int setTime)
{
	time = setTime;
	vibration.wLeftMotorSpeed = power;
	vibration.wRightMotorSpeed = power;
}

int Input::CheckAnyPadButton()
{
	int result = 0;
	if (padState.Gamepad.wButtons != prevPadState.Gamepad.wButtons && padState.Gamepad.wButtons != 0)
		result = padState.Gamepad.wButtons ^ prevPadState.Gamepad.wButtons;
	return result;
}
