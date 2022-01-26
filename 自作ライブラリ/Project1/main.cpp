﻿#include <wrl/client.h>

#include"Game.h"
#include"ConsoleWindow.h"
#include <d3d12.h>

// Windows アプリでのエントリーポイント(main 関数)
Game* Game::instance = nullptr;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
#ifdef _DEBUG
	//デバッグレイヤーをオンに
	Microsoft::WRL::ComPtr < ID3D12Debug> debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
	}
	ConsoleWindow* consoleWindow = new ConsoleWindow();
	consoleWindow->CreateConsoleWindow();
#endif // _DEBUG
	srand(time(nullptr));
	Game* game = Game::GetInstance();

	game->Initialize();
	game->Run();
	game->End();
	delete game;

#ifdef _DEBUG
	delete consoleWindow;
#endif // _DEBUG

	return 0;
}



