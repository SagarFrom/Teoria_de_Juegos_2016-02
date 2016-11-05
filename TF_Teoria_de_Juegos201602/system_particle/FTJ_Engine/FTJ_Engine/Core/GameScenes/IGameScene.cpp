#include "IGameScene.h"

namespace FTJ
{

	IGameScene::IGameScene(CGame* _pGame) : 
		m_pGame(_pGame), m_EntityManager(this)
	{
	}

	IGameScene::~IGameScene(){}
}