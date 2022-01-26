#include "Menu.h"

#include "Easing.h"
#include "Input.h"
#include "PtrDelete.h"
#include "Texture.h"
#include"DirectXLib.h"
#include "GameSettingParam.h"
#include "Input.h"
#include "PipelineState.h"
int SettingParam::padSensitive = 3;
int SettingParam::reverseX = 1;
int SettingParam::reverseY = 1;

int SettingParam::jumpButton = XINPUT_GAMEPAD_A;
int SettingParam::airSlideButton = XINPUT_GAMEPAD_B;
int SettingParam::runButton = XINPUT_GAMEPAD_LEFT_SHOULDER;
int SettingParam::resetButton = XINPUT_GAMEPAD_RIGHT_SHOULDER;
bool SettingParam::viewCollision = false;

bool SettingParam::onSSAO = false;

Menu::Menu()
{
//�X�v���C�g�̐���
	menuUI = new Sprite();
	backTex = new Sprite();
	restart = new Sprite();
	end = new Sprite();
	setting = new Sprite();
	yes = new Sprite();
	no = new Sprite();
	endCheck = new Sprite();
	restartCheck = new Sprite();
	sensitiveSetting = new Sprite();
	operateSetting = new Sprite();
	sensitiveSettingBar = new Sprite();
	dot = new Sprite();
	check = new Sprite();
	check2 = new Sprite();
	jump = new Sprite();
	cameraReset = new Sprite();
	action = new Sprite();
	upsideDown = new Sprite();
	changeRun = new Sprite();
	flipLeftRight = new Sprite();
	jumpOperate = new Sprite();
	cameraResetOperate = new Sprite();
	actionOperate = new Sprite();
	changeRunOperate = new Sprite();
	selectFrame = new Sprite();
	viewCollision = new Sprite();
	screenSetting = new Sprite();
	sSAO = new Sprite();
	
	//number = new Sprite();
//����ύX�p�̃{�^���Ɖ摜����ݒ�
	operateTexNames[XINPUT_GAMEPAD_LEFT_THUMB] = "LT";
	operateTexNames[XINPUT_GAMEPAD_RIGHT_THUMB] = "RT";
	operateTexNames[XINPUT_GAMEPAD_LEFT_SHOULDER] = "LB";
	operateTexNames[XINPUT_GAMEPAD_RIGHT_SHOULDER] = "RB";
	operateTexNames[XINPUT_GAMEPAD_A] = "a";
	operateTexNames[XINPUT_GAMEPAD_B] = "b";
	operateTexNames[XINPUT_GAMEPAD_X] = "x";
	operateTexNames[XINPUT_GAMEPAD_Y] = "y";
//���j���[��ʑS�̂̉摜��SRV�̐���
	resource = new TextureResource("Menu", { 960,540 });
}
Menu::~Menu()
{
	PtrDelete(menuUI);
	PtrDelete(backTex);
	PtrDelete(resource);
	PtrDelete(restart);
	PtrDelete(end);
	PtrDelete(yes);
	PtrDelete(no);
	PtrDelete(endCheck);
	PtrDelete(restartCheck);
	PtrDelete(sensitiveSetting);
	PtrDelete(setting);
	PtrDelete(operateSetting);
	PtrDelete(sensitiveSettingBar);
	PtrDelete(dot);
	PtrDelete(selectFrame);
	PtrDelete(check);
	PtrDelete(check2);
	PtrDelete(jump);
	PtrDelete(cameraReset);
	PtrDelete(action);
	PtrDelete(upsideDown);
	PtrDelete(changeRun);
	PtrDelete(flipLeftRight);
	PtrDelete(jumpOperate);
	PtrDelete(cameraResetOperate);
	PtrDelete(actionOperate);
	PtrDelete(changeRunOperate);
	PtrDelete(viewCollision);
	PtrDelete(screenSetting);
	PtrDelete(sSAO);
	//PtrDelete(number);

}

void Menu::Initialize()
{
	easingScale = 0;
	openCounter = 0;
	menuUIScale = { 0,0 };
	backTexScale = { 1,1 };
	restartScale = { 1,1 };
	endScale = { 1,1 };
	settingScale = { 1.5f,1.5f };
	sensitiveSettingScale = { 1.5f,1.5f };
	operateSettingScale = { 1,1 };
	yesScale = { 1.5f,1.5f };
	noScale = { 1,1 };
	screenSettingScale = { 1,1 };
	select = 0;
	close = false;
	startClose = false;
	closeCounter = 0;
	menuState = Base;
	dotPos = { 480 - 228*1.5f,300 - 20*1.5f };
	selectFramePos = {};
	operateChangeFlag = false;
	onSSAO = SettingParam::onSSAO;
	SettingParam::onSSAO = false;
	restartFlag = false;
	returnTitle = false;
	texAlpha = 1;
	migrateCounter = 0;
	migrate = false;
}

bool Menu::Update()
{
//�\�����
	if (openCounter > 30)
	{
		//�N���[�Y���̏k������
		if (startClose)
		{
			if (closeCounter > 30)
			{
				close = true;
				SettingParam::onSSAO = onSSAO;
			}
			easingScale = Easing::EaseInOutQuart(1, 0, 30, closeCounter);
			menuUIScale = { easingScale,easingScale };
			closeCounter++;
		}
		else
		{
			//�J�ڎ��͑J�ڏ����݂̂ő�����󂯕t���Ȃ�
			if (migrate)
			{
				Migrate();
				return false;
			}
			//���j���[�I����ʂ̏����i���j���[�{�́j
			switch (menuState)
			{
			case Base:
				BaseMenu();
				break;
				
			case SettingSelect:
				Setting();
				break;
				
			case PadOperateSetting:
				OperateSetting();
				break;
				
			case PadSensitiveSetting:
				SensitiveSetting();
				break;
				
			case ScreenSetting:
				ScreenSettingProcess();
				break;
				
			case RestartCheck:
				RestartCheckProcess();
				break;
				
			case EndCheck:
				EndCheckProcess();
				break;
				
			default:
				break;
			}
			//�ǂ̉�ʂł����j���[�{�^���������ꂽ�����
			if (Input::TriggerKey(DIK_ESCAPE)||Input::TriggerPadButton(XINPUT_GAMEPAD_START))
				startClose = true;
		}
	}
//���j���[�J�����Ƃ��̊g�又��
	else
	{
		easingScale = Easing::EaseInOutQuart(0, 1, 30, openCounter);
		menuUIScale = { easingScale,easingScale };
		openCounter++;
	}
	return close;
}
void Menu::Draw()
{
	//���\�[�X�o���A�̕ύX�ƃ����_�[�^�[�Q�b�g�����j���[�p�̃e�N�X�`���ɕύX
	resource->PreDraw();
	backTex->DrawSprite("menuUI", { 480,270 }, 0, backTexScale);
	resource->DepthClear();
	//�e��ʏ�Ԃɍ��킹�ĕ`�悪���e��ύX
	switch (menuState)
	{
	case Base:
		setting->DrawSprite("setting", { 480,104 }, 0, settingScale, { 1,1,1,texAlpha });
		restart->DrawSprite("restart", { 480,270 }, 0, restartScale, { 1,1,1,texAlpha });
		end->DrawSprite("end", { 480,436 }, 0, endScale, { 1,1,1,texAlpha });
		break;

	case SettingSelect:
		sensitiveSetting->DrawSprite("sensitiveSetting", { 480,104 }, 0, sensitiveSettingScale, { 1,1,1,texAlpha });
		operateSetting->DrawSprite("operateSetting", { 480,270 }, 0, operateSettingScale, { 1,1,1,texAlpha });
		screenSetting->DrawSprite("screenSetting", { 480,436 }, 0, screenSettingScale, { 1,1,1,texAlpha });
		break;

	case PadOperateSetting:
		operateSetting->DrawSprite("operateSetting", { 480,105 }, 0, { 1,1 }, { 1,1,1,texAlpha });
		
		upsideDown->DrawSprite("upsideDown", { 295,200 }, 0, { 1,1 }, { 1,1,1,texAlpha });
		//ON�̎��̂݃`�F�b�N�}�[�N��`��
		if (SettingParam::reverseY == -1)
			check->DrawSprite("check", { 403,196 }, 0, { 1,1 }, { 1,1,1,texAlpha });
		
		flipLeftRight->DrawSprite("flipLeftRight", { 655,200 }, 0, { 1,1 }, { 1,1,1,texAlpha });
		
		//ON�̎��̂݃`�F�b�N�}�[�N��`��
		if (SettingParam::reverseX == -1)
			check2->DrawSprite("check", { 763,196 }, 0, { 1,1 }, { 1,1,1,texAlpha });

		jump->DrawSprite("jump", { 240,300 }, 0, { 1,1 }, { 1,1,1,texAlpha });
		jumpOperate->DrawSprite(operateTexNames[SettingParam::jumpButton], { 400,300 }, 0, { 1,1 }, { 1,1,1,texAlpha });
		cameraReset->DrawSprite("cameraReset", { 620,300 }, 0, { 1,1 }, { 1,1,1,texAlpha });
		cameraResetOperate->DrawSprite(operateTexNames[SettingParam::resetButton], { 830,300 }, 0, { 1,1 }, { 1,1,1,texAlpha });
		action->DrawSprite("action", { 240,400 }, 0, { 1,1 }, { 1,1,1,texAlpha });
		actionOperate->DrawSprite(operateTexNames[SettingParam::airSlideButton], { 400,400 }, 0, { 1,1 }, { 1,1,1,texAlpha });
		changeRun->DrawSprite("changeRun", { 620,400 }, 0, { 1,1 }, { 1,1,1,texAlpha });
		changeRunOperate->DrawSprite(operateTexNames[SettingParam::runButton], { 830,400 }, 0, { 1,1 }, { 1,1,1,texAlpha });
		selectFrame->DrawSprite("selectFrame", selectFramePos, 0, { 1,1 }, { 1,1,1,texAlpha });
		break;

	case PadSensitiveSetting:
		sensitiveSetting->DrawSprite("sensitiveSetting", { 480,130 }, 0, { 1.2f,1.2f }, { 1,1,1,texAlpha });
		dot->DrawSprite("dot", dotPos, 0, { 1,1 }, { 1,1,1,texAlpha });
		sensitiveSettingBar->DrawSprite("sensitiveSettingBar", { 480,300 }, 0, { 1.5f,1.5f }, { 1,1,1,texAlpha });

		break;
		
	case ScreenSetting:
		screenSetting->DrawSprite("screenSetting", { 480,105 }, 0, { 1.2f,1.2f }, { 1,1,1,texAlpha });
		viewCollision->DrawSprite("viewCollision", { 480,240 }, 0, { 1,1 }, { 1,1,1,texAlpha });
		sSAO->DrawSprite("SSAO", { 614,390 }, 0, { 1,1 }, { 1,1,1,texAlpha });

		//ON�̎��̂݃`�F�b�N�}�[�N��`��
		if (SettingParam::viewCollision)
			check->DrawSprite("check", { 730,240 }, 0, { 1.2f,1.2f }, { 1,1,1,texAlpha });
		//ON�̎��̂݃`�F�b�N�}�[�N��`��
		if (onSSAO)
			check2->DrawSprite("check", { 730,390 }, 0, { 1.2f,1.2f }, { 1,1,1,texAlpha });
		selectFrame->DrawSprite("selectFrame", selectFramePos, 0, { 1.2f,1.2f }, { 1,1,1,texAlpha });
		break;
		
	case RestartCheck:
		restartCheck->DrawSprite("restartCheck", { 480,150 }, 0, { 1,1 }, { 1,1,1,texAlpha });
		yes->DrawSprite("yes", { 320,380 }, 0, yesScale, { 1,1,1,texAlpha });
		no->DrawSprite("no", { 640,380 }, 0, noScale, { 1,1,1,texAlpha });
		break;

	case EndCheck:
		endCheck->DrawSprite("endCheck", { 480,150 }, 0, { 1,1 }, { 1,1,1,texAlpha });
		yes->DrawSprite("yes", { 320,380 }, 0, yesScale, { 1,1,1,texAlpha });
		no->DrawSprite("no", { 640,380 }, 0, noScale, { 1,1,1,texAlpha });
		break;

	default:
		break;
	}
	//���\�[�X�o���A��߂�
	resource->PostDraw();
	//�����_�����O���ꂽ���j���[�摜��`��
	menuUI->DrawSprite("Menu" + std::to_string(DirectXLib::GetInstance()->GetBbIndex()), { 960,540 }, 0, menuUIScale, { 1,1,1,1 }, { 0.5f,0.5f },"NoAlphaToCoverageSprite");
}

void Menu::BaseMenu()
{
	if (Input::TriggerPadLStickDown() || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_DOWN)
		|| Input::TriggerPadLStickUp() || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_UP)
		|| Input::TriggerKey(DIK_W) || Input::TriggerKey(DIK_S))
	{
		if (Input::TriggerPadLStickDown() || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_DOWN) || Input::TriggerKey(DIK_S))
		{
			if (select < 2)
				select++;
		}
		if (Input::TriggerPadLStickUp() || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_UP) || Input::TriggerKey(DIK_W))
		{
			if (select > 0)
				select--;
		}
		SetTexParam();
	}

	if (Input::TriggerPadButton(XINPUT_GAMEPAD_B) || Input::TriggerKey(DIK_SPACE))
	{
		switch (select)
		{
		case 0:
			nextMenuState = SettingSelect;
			break;
		case 1:
			nextMenuState = RestartCheck;
			break;
		case 2:
			nextMenuState = EndCheck;
			break;
		default:
			return;
		}
		migrate = true;
		SetTexParam();
	}

	if(Input::TriggerPadButton(XINPUT_GAMEPAD_A) || Input::TriggerKey(DIK_Z))
	{
		startClose = true;
	}
}

void Menu::Setting()
{
	if (Input::TriggerPadLStickDown() || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_DOWN)
		|| Input::TriggerPadLStickUp() || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_UP)
		|| Input::TriggerKey(DIK_W) || Input::TriggerKey(DIK_S))
	{
		if (Input::TriggerPadLStickDown() || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_DOWN) || Input::TriggerKey(DIK_S))
		{
			if (select < 2)
				select++;
		}
		if (Input::TriggerPadLStickUp() || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_UP) || Input::TriggerKey(DIK_W))
		{
			if (select > 0)
				select--;
		}
		SetTexParam();
	}
	if (Input::TriggerPadButton(XINPUT_GAMEPAD_B) || Input::TriggerKey(DIK_SPACE))
	{
		switch (select)
		{
		case 0:
			nextMenuState = PadSensitiveSetting;
			break;
		case 1:
			nextMenuState = PadOperateSetting;
			break;
		case 2:
			nextMenuState = ScreenSetting;
			break;
		default:
			return;
		}
		migrate = true;
		SetTexParam();
	}
	if (Input::TriggerPadButton(XINPUT_GAMEPAD_A) || Input::TriggerKey(DIK_Z))
	{
		migrate = true;
		nextMenuState = Base;
		SetTexParam();
	}
}

void Menu::SensitiveSetting()
{
	if (Input::TriggerPadLStickLeft() || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_LEFT)
		|| Input::TriggerPadLStickRight() || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_RIGHT)
		|| Input::TriggerKey(DIK_A) || Input::TriggerKey(DIK_D))

	{
		if (Input::TriggerPadLStickRight() || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_RIGHT) || Input::TriggerKey(DIK_D))
		{
			if (SettingParam::padSensitive < 10)
				SettingParam::padSensitive++;
		}
		if (Input::TriggerPadLStickLeft() || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_LEFT) || Input::TriggerKey(DIK_A))
		{
			if (SettingParam::padSensitive > 1)
				SettingParam::padSensitive--;
		}
		SetTexParam();
	}
	if (Input::TriggerPadButton(XINPUT_GAMEPAD_A) || Input::TriggerKey(DIK_Z))
	{
		nextMenuState = SettingSelect;
		migrate = true;
		SetTexParam();
	}
}

void Menu::OperateSetting()
{
	if (operateChangeFlag)
	{
		//���������PAD�̃{�^���������ꂽ���������
		int button = Input::CheckAnyPadButton();

		//�����ꂽ�{�^�������蓖�ĉ\�͈͓��̃{�^����������
		if (button == XINPUT_GAMEPAD_LEFT_THUMB||
			button == XINPUT_GAMEPAD_RIGHT_THUMB||
			button == XINPUT_GAMEPAD_LEFT_SHOULDER||
			button == XINPUT_GAMEPAD_RIGHT_SHOULDER||
			button == XINPUT_GAMEPAD_A||
			button == XINPUT_GAMEPAD_B||
			button == XINPUT_GAMEPAD_X||
			button == XINPUT_GAMEPAD_Y)
		{
			//�ύX�Ώۂɂ���ďꍇ�����{�{�^���̕ύX�{�ύX��̃{�^�������ɑ��̑���Ɋ��蓖�čς݂̏ꍇ�{�^�������ւ���
			switch (select)
			{
			case 2:
				if (SettingParam::resetButton == button) SettingParam::resetButton = SettingParam::jumpButton;

				else if (SettingParam::airSlideButton == button) SettingParam::airSlideButton = SettingParam::jumpButton;

				else if (SettingParam::runButton == button) SettingParam::runButton = SettingParam::jumpButton;

				SettingParam::jumpButton = button;
				break;
			case 3:
				if (SettingParam::jumpButton == button) SettingParam::jumpButton = SettingParam::resetButton;

				else if (SettingParam::airSlideButton == button) SettingParam::airSlideButton = SettingParam::resetButton;

				else if (SettingParam::runButton == button) SettingParam::runButton = SettingParam::resetButton;

				SettingParam::resetButton = button;
				break;
			case 4:
				if (SettingParam::jumpButton == button) SettingParam::jumpButton = SettingParam::airSlideButton;

				else if (SettingParam::resetButton == button) SettingParam::resetButton = SettingParam::airSlideButton;

				else if (SettingParam::runButton == button) SettingParam::runButton = SettingParam::airSlideButton;

				SettingParam::airSlideButton = button;
				break;
			case 5:
				if (SettingParam::jumpButton == button) SettingParam::jumpButton = SettingParam::runButton;

				else if (SettingParam::resetButton == button) SettingParam::resetButton = SettingParam::runButton;

				else if (SettingParam::airSlideButton == button) SettingParam::airSlideButton = SettingParam::runButton;

				SettingParam::runButton = button;
				break;
			default:
				return;
			}
			operateChangeFlag = false;
		}
		return;
	}
	if (Input::TriggerPadLStickDown() || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_DOWN)
		|| Input::TriggerPadLStickUp() || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_UP)||
		Input::TriggerPadLStickLeft() || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_LEFT)
		|| Input::TriggerPadLStickRight() || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_RIGHT)
		|| Input::TriggerKey(DIK_A) || Input::TriggerKey(DIK_D) || Input::TriggerKey(DIK_W) || Input::TriggerKey(DIK_S))
	{
		if (Input::TriggerPadLStickRight() || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_RIGHT) || Input::TriggerKey(DIK_D))
		{
			if (select < 5)
				select++;
		}
		if (Input::TriggerPadLStickLeft() || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_LEFT) || Input::TriggerKey(DIK_A))
		{
			if (select > 0)
				select--;
		}
		if (Input::TriggerPadLStickDown() || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_DOWN) || Input::TriggerKey(DIK_S))
		{
			if (select < 5)
			{
				select += 2;
				if (select > 5)
					select = 5;
			}
		}
		if (Input::TriggerPadLStickUp() || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_UP) || Input::TriggerKey(DIK_W))
		{
			if (select > 0)
			{
				select -= 2;
				if (select < 0)
					select = 0;
			}
		}
		SetTexParam();
	}
	if (Input::TriggerPadButton(XINPUT_GAMEPAD_B) || Input::TriggerKey(DIK_SPACE))
	{
		switch (select)
		{
		case 0:
			SettingParam::reverseY = SettingParam::reverseY == 1 ? -1 : 1;
			break;
		case 1:
			SettingParam::reverseX = SettingParam::reverseX == 1 ? -1 : 1;
			break;
		case 2:
		case 3:
		case 4:
		case 5:
			operateChangeFlag = true;
			break;
		default:
			return;
		}
		SetTexParam();
	}
	if (Input::TriggerPadButton(XINPUT_GAMEPAD_A) || Input::TriggerKey(DIK_Z))
	{
		nextMenuState = SettingSelect;
		migrate = true;
		SetTexParam();
	}

}

void Menu::ScreenSettingProcess()
{
	if (Input::TriggerPadLStickDown() || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_DOWN)
		|| Input::TriggerPadLStickUp() || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_UP)
		|| Input::TriggerKey(DIK_W) || Input::TriggerKey(DIK_S))
	{
		if (Input::TriggerPadLStickDown() || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_DOWN) || Input::TriggerKey(DIK_S))
		{
			if (select <1)
				select++;
		}
		if (Input::TriggerPadLStickUp() || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_UP) || Input::TriggerKey(DIK_W))
		{
			if (select > 0)
				select--;
		}
		SetTexParam();
	}
	if (Input::TriggerPadButton(XINPUT_GAMEPAD_B) || Input::TriggerKey(DIK_SPACE))
	{
		switch (select)
		{
		case 0:
			SettingParam::viewCollision = SettingParam::viewCollision == true ? false : true;
			break;
		case 1: 
			onSSAO = onSSAO == true ? false : true;
			break;
		default:
			return;
		}
		SetTexParam();
	}
	if (Input::TriggerPadButton(XINPUT_GAMEPAD_A) || Input::TriggerKey(DIK_Z))
	{
		migrate = true;
		nextMenuState = SettingSelect;
		SetTexParam();
	}

}

void Menu::RestartCheckProcess()
{
	if (Input::TriggerPadLStickLeft() || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_LEFT)
		|| Input::TriggerPadLStickRight() || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_RIGHT)
		|| Input::TriggerKey(DIK_A) || Input::TriggerKey(DIK_D))
	{
		if (Input::TriggerPadLStickRight() || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_RIGHT) || Input::TriggerKey(DIK_D))
		{
			if (select < 1)
				select++;
		}
		if (Input::TriggerPadLStickLeft() || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_LEFT) || Input::TriggerKey(DIK_A))
		{
			if (select > 0)
				select--;
		}
		SetTexParam();
	}
	if (Input::TriggerPadButton(XINPUT_GAMEPAD_B) || Input::TriggerKey(DIK_SPACE))
	{
		switch (select)
		{
		case 0:
			restartFlag = true;
			startClose = true;
			break;
		case 1:
			migrate = true;
			nextMenuState = Base;
			break;
		default:
			return;
		}
		SetTexParam();
	}
	if (Input::TriggerPadButton(XINPUT_GAMEPAD_A) || Input::TriggerKey(DIK_Z))
	{
		migrate = true;
		nextMenuState = Base;
		SetTexParam();
	}
}

void Menu::EndCheckProcess()
{
	if (Input::TriggerPadLStickLeft() || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_LEFT)
		|| Input::TriggerPadLStickRight() || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_RIGHT)
		|| Input::TriggerKey(DIK_A) || Input::TriggerKey(DIK_D))
	{
		if (Input::TriggerPadLStickRight() || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_RIGHT) || Input::TriggerKey(DIK_D))
		{
			if (select < 1)
				select++;
		}
		if (Input::TriggerPadLStickLeft() || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_LEFT) || Input::TriggerKey(DIK_A))
		{
			if (select > 0)
				select--;
		}
		SetTexParam();
	}

	if (Input::TriggerPadButton(XINPUT_GAMEPAD_B) || Input::TriggerKey(DIK_SPACE))
	{
		switch (select)
		{
		case 0:
			returnTitle = true;
			startClose = true;
			break;
		case 1:
			migrate = true;
			nextMenuState = Base;
			break;
		default:
			return;
		}
		SetTexParam();
	}
	if (Input::TriggerPadButton(XINPUT_GAMEPAD_A) || Input::TriggerKey(DIK_Z))
	{
		migrate = true;
		nextMenuState = Base;
		SetTexParam();
	}
}

void Menu::SetTexParam()
{
	switch (menuState)
	{
	case Base:
		switch (select)
		{
		case 0:
			restartScale = { 1,1 };
			endScale = { 1,1 };
			settingScale = { 1.5f,1.5f };
			break;
		case 1:
			restartScale = { 1.5f,1.5f };
			endScale = { 1,1 };
			settingScale = { 1,1 };
			break;
		case 2:
			restartScale = { 1,1 };
			endScale = { 1.5f,1.5f };
			settingScale = { 1,1 };
			break;
		default:
			break;
		}

		break;

	case SettingSelect:
		switch (select)
		{
		case 0:
			sensitiveSettingScale = { 1.5f,1.5f };
			operateSettingScale = { 1,1 };
			screenSettingScale = { 1,1 };
			break;
		case 1:
			operateSettingScale = { 1.5f,1.5f };
			sensitiveSettingScale = { 1,1 };
			screenSettingScale = { 1,1 };
			break;
		case 2:
			operateSettingScale = { 1,1 };
			sensitiveSettingScale = { 1,1 };
			screenSettingScale = { 1.5f,1.5f };
			break;
		default:
			break;
		}
		break;

	case PadOperateSetting:
		switch (select)
		{
		case 0:
			selectFramePos = { 403,200 };
			break;
		case 1:
			selectFramePos = { 763,200 };
			break;
		case 2:
			selectFramePos = { 400,300 };
			break;
		case 3:
			selectFramePos = { 830,300 };
			break;
		case 4:
			selectFramePos = { 400,400 };
			break;
		case 5:
			selectFramePos = { 830,400 };
			break;

		default:
			break;
		}

		break;

	case PadSensitiveSetting:
		dotPos = { (480 - 228 * 1.5f) + 49 * 1.5f * (SettingParam::padSensitive - 1),300 - 20 * 1.5f };
		break;
	case ScreenSetting:
		switch (select)
		{
		case 0:
			selectFramePos = { 718,240 };
			break;
		case 1:
			selectFramePos = { 718,390 };
			break;
		default:
			break;
		}

		break;
	case RestartCheck:
		switch (select)
		{
		case 0:
			yesScale = { 1.5f,1.5f };
			noScale = { 1,1 };
			break;
		case 1:
			noScale = { 1.5f,1.5f };
			yesScale = { 1,1 };
			break;
		default:
			break;
		}
		break;

	case EndCheck:
		switch (select)
		{
		case 0:
			yesScale = { 1.5f,1.5f };
			noScale = { 1,1 };
			break;
		case 1:
			noScale = { 1.5f,1.5f };
			yesScale = { 1,1 };
			break;
		default:
			break;
		}

		break;

	default:
		break;
	}

}

void Menu::Migrate()
{
	if(migrateCounter<=30)
	{
		texAlpha = Easing::EaseInOutQuart(1, 0, 30, migrateCounter);
	}
	else if(migrateCounter<=60)
	{
		texAlpha = Easing::EaseInOutQuart(0, 1, 30, migrateCounter-30);
	}
	else
	{
		migrate = false;
		migrateCounter = 0;
		return;
	}
	if(migrateCounter == 30)
	{
		menuState = nextMenuState;
		select = 0;
		SetTexParam();
	}
	migrateCounter++;
}
