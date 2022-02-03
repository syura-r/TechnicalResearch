#include "FPS.h"

#include <chrono>
#include <time.h>
#include<synchapi.h>
#include<stdio.h>

float FPS::fps = 60;
//int FPS::count = 0;
//DWORD FPS::startProsessTime = 0;
//DWORD FPS::nowTime = 0;
// float FPS::deltaTime = 0;
// float FPS::frameRate = 0;
// std::chrono::steady_clock::time_point FPS::lastUpdate = {};
int FPS::fpsCount = 0;

 Window* FPS::win = nullptr;

  LARGE_INTEGER FPS::mTimeEnd = {};
  LARGE_INTEGER FPS::mTimeFreq = {};
  LARGE_INTEGER FPS::mTimeStart = {};
  std::chrono::steady_clock::time_point FPS::lastUpdate = {};

void FPS::Initialize(const float & fps)
{
	FPS::fps = fps;
	//startProsessTime = timeGetTime();
	//nowTime = timeGetTime();
}

void FPS::StartMeasure()
{
	//���g���擾
	QueryPerformanceFrequency(&mTimeFreq);
	//�v���J�n���Ԃ̏�����
	QueryPerformanceCounter(&mTimeStart);


}

void FPS::FPSFixed()
{
	// ���̎��Ԃ��擾
	QueryPerformanceCounter(&mTimeEnd);
	// (���̎��� - �O�t���[���̎���) / ���g�� = �o�ߎ���(�b�P��)
	auto mFrameTime = static_cast<double>(mTimeEnd.QuadPart - mTimeStart.QuadPart) / static_cast<double>(mTimeFreq.QuadPart);

	

#if _DEBUG

	//fpsCount++;
	//static int count = 0;
	//// �o�ߎ��Ԍv��
	//auto now = std::chrono::steady_clock::now();
	//float deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(now - lastUpdate).count() / 1000000.0f;
	//float frameRate = 1.0f / deltaTime;
	//lastUpdate = now;

	//if (fpsCount == 60)
	//{

	//	//char str[50];
	//	char str[50];
	//	sprintf_s(str, 50, "fps=%f", frameRate);
	//	SetWindowTextA(win->GetHwnd(), str);

	//	//printf("FPS:",);
	//	fpsCount = 0;
	//}
#endif // _DEBUG
	//�������Ԃɗ]�T������ꍇ�͂��̕��҂�
	auto FRAME_TIME = 1.0 / fps;

	if (mFrameTime < FRAME_TIME)
	{
		//Sleep�̎��Ԃ��v�Z
		DWORD sleepTime = static_cast<DWORD>((FRAME_TIME - mFrameTime) * 1000);
		timeBeginPeriod(1);
		//�Q��
		Sleep(sleepTime);
		timeEndPeriod(1);
	}

}
