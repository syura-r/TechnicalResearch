#include "window.h"
#include <imgui_impl_win32.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


Window::Window()
{
	window_width = 1920;
	window_height = 1080;
}

Window::Window(const int window_width, const int window_height):window_width(window_width),window_height(window_height)
{
}


Window::~Window()
{
}

LRESULT Window::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) {
		return 1;
	}

	//���b�Z�[�W�ŕ���
	switch (msg)
	{
	case WM_DESTROY://�E�B���h�E���j�����ꂽ
		PostQuitMessage(0);//OS�ɑ΂��āA�A�v���̏I����`����
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);//�����̏������s��
}

void Window::CreateWidow(WINDOW_TYPE type)
{
	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = (WNDPROC)WindowProc; //�E�B���h�E�v���V�[�W����ݒ�
	w.lpszClassName = L"DirectXGame";//�E�B���h�E�N���X��
	w.hInstance = GetModuleHandle(nullptr); //�E�B���h�E�n���h��
	w.hCursor = LoadCursor(NULL, IDC_ARROW); //�J�[�\���w��
			//�E�B���h�E�N���X��OS�ɓo�^
	RegisterClassEx(&w);

	//�E�B���h�E�T�C�Y{X���W Y���W ���� �c��}
	wrc = { 0,0,window_width,window_height };

	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);//�����ŃT�C�Y��␳

	switch (type)
	{
	case Window::WINDOW:
		hwnd = CreateWindow(w.lpszClassName,//�N���X��
			L"DirectXGame",			//�^�C�g���o�[�̕���//
			WS_OVERLAPPEDWINDOW,	//�W���I�ȃE�B���h�E�X�^�C��
			CW_USEDEFAULT,			//�\��X���W�iOS�ɔC����j
			CW_USEDEFAULT,			//�\��Y���W�iOS�ɔC����)
			wrc.right - wrc.left,	//�E�B���h�E����
			wrc.bottom - wrc.top,	//�E�B���h�E����
			nullptr,				//�e�E�B���h�E�n���h��
			nullptr,				//���j���[�n���h��
			w.hInstance,			//�Ăяo���A�v���P�[�V�����n���h��
			nullptr);				//�I�v�V����
		break;
	case Window::FULLSCREEN:
		hwnd = CreateWindow(w.lpszClassName,//�N���X��
			L"DirectXGame",			//�^�C�g���o�[�̕���//
			WS_VISIBLE | WS_POPUP, //�W���I�ȃE�B���h�E�X�^�C��
			0,	
			0,	
			window_width, //�E�B���h�E����
			window_height, //�E�B���h�E�c��
			nullptr,				//�e�E�B���h�E�n���h��
			nullptr,				//���j���[�n���h��
			w.hInstance,			//�Ăяo���A�v���P�[�V�����n���h��
			nullptr);				//�I�v�V����

		break;
	default:
		break;
	}

	//�E�B���h�E�\��
	ShowWindow(hwnd, SW_SHOW);
}

WNDCLASSEX Window::getW()
{
	return w;
}

HWND Window::GetHwnd()
{
	return hwnd;
}

int Window::GetWindowWidth()
{
	return window_width;
}

int Window::GetWindowHeight()
{
	return window_height;
}

void Window::End()
{
	// �E�B���h�E�N���X��o�^����
	UnregisterClass(w.lpszClassName,w.hInstance);
}
