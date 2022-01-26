#pragma once
#include <Windows.h>

class Window
{
public:
	enum WINDOW_TYPE
	{
		WINDOW,
		FULLSCREEN,
	};
	Window();
	Window(const int window_width, const int window_height);
	~Window();
	void CreateWidow(WINDOW_TYPE type);
	WNDCLASSEX getW();
	//�E�B���h�E�T�C�Y
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	HWND GetHwnd();
	int GetWindowWidth();
	int GetWindowHeight();
	void End();
private:
	int window_width;//����
	int window_height;//����
	RECT wrc;
	WNDCLASSEX w{};  //�E�B���h�E�N���X�̐ݒ�
	//�E�B���h�E�I�u�W�F�N�g�̐���
	HWND hwnd;



};

