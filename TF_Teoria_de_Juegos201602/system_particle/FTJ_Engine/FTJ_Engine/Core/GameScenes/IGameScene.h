#pragma once
#include "../Entity/EntityManager.h"

class CGame;

namespace FTJ
{
	enum GAME_SCENE
	{
		SCENE_MAIN
	};

	class IGameScene
	{
	protected:
		CGame* m_pGame;

		CEntityManager m_EntityManager;

	public:
		IGameScene(CGame* _pGame);
		virtual ~IGameScene();

		virtual void OnScene_Enter(){}
		virtual void OnScene_Update(const float _deltaTime) = 0;
		virtual void OnScene_Exit() {}

		virtual GAME_SCENE GetGameScene() = 0;

		CGame* GetGame()const { return m_pGame; }
	};

}