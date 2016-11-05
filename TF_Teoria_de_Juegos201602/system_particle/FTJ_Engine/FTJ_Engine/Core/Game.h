#pragma once

#pragma region DEFINITIONS

#define BACKBUFFER_WIDTH	800
#define BACKBUFFER_HEIGHT	600

#include <vector>

#include "Time\XTime.h"

#include <d3d11.h>
#include <DirectXMath.h>
#pragma comment(lib,"d3d11.lib")

using namespace DirectX;
using namespace std;

#define MOVE_SENSITIVITY 600.0f
#define MOUSE_SENSITIVITY 1200

#pragma endregion

#include "Render\FTJ_RenderManager.h"
#include "Events\EventManager.h"
#include "Input\InputManager.h"

#include "GameScenes\MainGameScene.h"

using namespace FTJ;

class CGame
{
	HINSTANCE			application;
	WNDPROC				appWndProc;
	HWND				window;
	XTime*				xTime;

	FTJ::CInputManager*		m_pInputManager;
	FTJ::CEventManager*		m_pEventManager;
	FTJ::CRenderManager*	m_pRenderManager;

	FTJ::IGameScene* m_pCurrentGameScene;
	std::vector<FTJ::IGameScene*> m_vGameScenes;

public:

	CGame(HINSTANCE hinst, WNDPROC proc);
	bool Run();
	bool ShutDown();

	void SwitchGameScene(GAME_SCENE _newScene);
};