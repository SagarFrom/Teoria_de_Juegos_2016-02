#pragma once
#include "IGameScene.h"

namespace FTJ
{
	class CCamera;

	class CMainGameScene :
		public IGameScene
	{
		CCamera* m_pCamera;

	public:
		CMainGameScene(CGame* _pGame);
		~CMainGameScene();


		virtual void OnScene_Enter() override;
		virtual void OnScene_Update(const float _deltaTime) override;
		virtual void OnScene_Exit() override;

		virtual GAME_SCENE GetGameScene() { return GAME_SCENE::SCENE_MAIN; }

	};

}