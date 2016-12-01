#pragma once
//************************************************************
//************ INCLUDES & DEFINES ****************************
//************************************************************
#include <time.h>

#include "FTJ_Console.h"
#include "Game.h"
#include "../systemclass.h"

using namespace FTJ;

//************************************************************
//************ WINDOWS RELATED *******************************
//************************************************************

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR, int)
{
	//srand(unsigned int(time(0)));
	//CGame game(hInstance, (WNDPROC)WndProc);
	//MSG msg; ZeroMemory(&msg, sizeof(msg));


	////LOOP
	//while (msg.message != WM_QUIT && game.Run())
	//{
	//	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	//	{
	//		TranslateMessage(&msg);
	//		DispatchMessage(&msg);
	//	}
	//}
	//game.ShutDown();
	//return 0;

	SystemClass* System;
	bool result;


	// Create the system object.
	System = new SystemClass;
	if (!System)
	{
		return 0;
	}

	// Initialize and run the system object.
	result = System->Initialize();
	if (result)
	{
		System->Run();
	}

	// Shutdown and release the system object.
	System->Shutdown();
	delete System;
	System = 0;

	return 0;

}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (GetAsyncKeyState(VK_ESCAPE))
		message = WM_DESTROY;
	switch (message)
	{
	case WM_CREATE:
		break;

	case (WM_DESTROY) : { PostQuitMessage(0); }
						break;
	default:
			CInputManager::GetInstance()->HandleWindowMessages(message, wParam, lParam);
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}
//********************* END WINDOWS ************************//