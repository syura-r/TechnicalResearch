#pragma once
#include "Sprite.h"
#include "TextureResource.h"
#include "Vector.h"

class Menu
{
public:
	Menu();
	~Menu();
	void Initialize();
	bool Update();
	void Draw();
	bool GetRestart() { return restartFlag; }
	bool GetReturnTitle() { return returnTitle; }

private:
	//��ԑ�g�̃��j���[����
	void BaseMenu();
	//�ݒ�̍��ڑI��̏���
	void Setting();
	//���x�ݒ�̏���
	void SensitiveSetting();
	//����ݒ�̏���
	void OperateSetting();
	//��ʐݒ�̏���
	void ScreenSettingProcess();
	//���Ȃ����m�F�̏���
	void RestartCheckProcess();
	//�^�C�g���ɖ߂�m�F�̏���
	void EndCheckProcess();
	//�e��Ԃł̉摜�̈ʒu��X�P�[���̏���
	void SetTexParam();
	//�J�ڏ���
	void Migrate();
	enum MenuState
	{
		Base,//��ԑ�g
		SettingSelect,//�ݒ�̍��ڑI��
		PadSensitiveSetting,//pad���x�ݒ�
		PadOperateSetting,//����ݒ�
		ScreenSetting,//��ʐݒ�
		RestartCheck,//���X�^�[�g�m�F
		EndCheck,//�I���m�F
	};
	MenuState menuState;
	MenuState nextMenuState;
	
	int openCounter = 0;
	int closeCounter = 0;
	bool startClose = false;
	Sprite* menuUI = nullptr;
	Sprite* backTex = nullptr;
	Sprite* restart = nullptr;
	Sprite* end = nullptr;
	Sprite* setting = nullptr;
	Sprite* yes = nullptr;
	Sprite* no = nullptr;
	Sprite* endCheck = nullptr;
	Sprite* restartCheck = nullptr;
	Sprite* sensitiveSetting = nullptr;
	Sprite* operateSetting = nullptr;
	Sprite* sensitiveSettingBar = nullptr;
	Sprite* dot = nullptr;
	Sprite* check = nullptr;
	Sprite* check2 = nullptr;
	Sprite* jump = nullptr;
	Sprite* cameraReset = nullptr;
	Sprite* action = nullptr;
	Sprite* upsideDown = nullptr;
	Sprite* changeRun = nullptr;
	Sprite* flipLeftRight = nullptr;
	Sprite* jumpOperate = nullptr;
	Sprite* cameraResetOperate = nullptr;
	Sprite* actionOperate = nullptr;
	Sprite* changeRunOperate = nullptr;
	Sprite* selectFrame = nullptr; 
	Sprite* viewCollision = nullptr;
	Sprite* screenSetting = nullptr;
	Sprite* sSAO = nullptr;
	
	bool operateChangeFlag = false;
	
	//Sprite* number = nullptr;

	std::unordered_map<int, std::string> operateTexNames;
	
	int select = 0;
	float easingScale;
	Vector2 menuUIScale;
	Vector2 backTexScale;
	Vector2 restartScale;
	Vector2 endScale;
	Vector2 settingScale;
	Vector2 sensitiveSettingScale;
	Vector2	operateSettingScale;
	Vector2 yesScale;
	Vector2 noScale;
	Vector2 dotPos;
	Vector2 selectFramePos;
	Vector2 screenSettingScale;
	bool close = false;

	//�^�C�g���ɖ߂�t���O
	bool returnTitle = false;
	bool restartFlag = false;
	TextureResource* resource = nullptr;

	bool onSSAO;

	float texAlpha;
	//�J�ڗp�̃J�E���^�[
	int migrateCounter;
	bool migrate;
};

