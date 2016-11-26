#include "Game.h"

#include "FTJ_Console.h"
#include "Log\Log.h"

using namespace FTJ;

CGame::CGame(HINSTANCE hinst, WNDPROC proc)
{
	m_pCurrentGameScene = nullptr;

	xTime = new XTime();

	CInputManager::CreateInstance();

#pragma region WINDOW_INIT
	// ****************** BEGIN WARNING ***********************// 
	application = hinst;
	appWndProc = proc;

	WNDCLASSEX  wndClass;
	ZeroMemory(&wndClass, sizeof(wndClass));
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.lpfnWndProc = appWndProc;
	wndClass.lpszClassName = L"DirectXApplication";
	wndClass.hInstance = application;
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOWFRAME);
	//wndClass.hIcon			= LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_FSICON));
	RegisterClassEx(&wndClass);

	RECT window_size = { 0, 0, BACKBUFFER_WIDTH, BACKBUFFER_HEIGHT };
	AdjustWindowRect(&window_size, WS_OVERLAPPEDWINDOW, false);

	window = CreateWindow(L"DirectXApplication", L"", WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX),
		CW_USEDEFAULT, CW_USEDEFAULT, window_size.right - window_size.left, window_size.bottom - window_size.top,
		NULL, NULL, application, this);

	ShowWindow(window, SW_SHOW);
	//********************* END WARNING ************************//
#pragma endregion

	FTJ::Console::CreateConsoleWindow();
	//LogSetUp(L"FTJ");

	m_pRenderManager = CRenderManager::GetInstance();
	m_pRenderManager->Initialize(window, BACKBUFFER_WIDTH, BACKBUFFER_HEIGHT);

	m_pEventManager = CEventManager::GetInstance();
	m_pEventManager->Initialize();

	m_pInputManager = CInputManager::GetInstance();

	m_vGameScenes.push_back(new CMainGameScene(this));
	SwitchGameScene(GAME_SCENE::SCENE_MAIN);
}

//************************************************************
//************ EXECUTION *************************************
//************************************************************

bool CGame::Run()
{
	xTime->Signal();
	float dt = (float)xTime->Delta();

	static DWORD frameCount = 0;
	frameCount++;
	static DWORD framesPast = frameCount;
	static DWORD prevCount = GetTickCount();
	if (GetTickCount() - prevCount > 1000)
	{
		char buffer[256];
		sprintf_s(buffer, "FTJ Engine | FPS - %d", frameCount - framesPast);
		SetWindowTextA(window, buffer);
		framesPast = frameCount;
		prevCount = GetTickCount();
	}

	m_pEventManager->Update();


	for (size_t i = 0; i < m_vGameScenes.size(); i++)
	{
		m_vGameScenes[i]->OnScene_Update(dt);
	}
	m_pInputManager->UpdateKeyStates(dt);

	//RENDER
	m_pRenderManager->Render(m_pCurrentGameScene);

	return true;
}

//************************************************************
//************ DESTRUCTION ***********************************
//************************************************************

bool CGame::ShutDown()
{
	CInputManager::DestroyInstance();
	CEventManager::GetInstance()->Terminate();

	FTJ::Console::DestroyConsole();

	delete xTime;

	UnregisterClass(L"DirectXApplication", application);
	return true;
}

void CGame::SwitchGameScene(GAME_SCENE _newScene)
{
	if (m_pCurrentGameScene == m_vGameScenes[_newScene])
		return;//cant switch to same state

	if (m_pCurrentGameScene != NULL)
		m_pCurrentGameScene->OnScene_Exit();

	m_pCurrentGameScene = m_vGameScenes[_newScene];

	m_pCurrentGameScene->OnScene_Enter();
}