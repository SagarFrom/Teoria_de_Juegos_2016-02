#include "MainGameScene.h"

#include "../Entity/EntityManager.h"
#include "../Entity/GameObject.h"
#include "../Entity/Transform.h"

#include "../Render/FTJ_RenderManager.h"
#include "../Render/Camera.h"
#include "../Render/MeshRenderer.h"

namespace FTJ
{

	CMainGameScene::CMainGameScene(CGame* _pGame) : IGameScene(_pGame)
	{
	}
	CMainGameScene::~CMainGameScene()
	{
	}

	/*virtual*/ void CMainGameScene::OnScene_Enter() //override
	{
		//create objects'
		m_pCamera = m_EntityManager.Create_CameraGameObject();

		CRenderManager::GetInstance()->RegisterSceneCamera(
			this, m_pCamera);

		m_pCamera->GetGameObject()->GetTransform()->SetPosition(0, 0, -100.f);

		CGameObject* boxObject = m_EntityManager.Create_EmptyGameObject();
		MeshRenderer* meshRenderer = new MeshRenderer(boxObject, "Robot_Death.bin");



	}
	/*virtual*/ void CMainGameScene::OnScene_Update(const float _deltaTime) //override
	{
		m_EntityManager.Update(_deltaTime);

		m_EntityManager.DestroyGameObjects();
	}
	/*virtual*/ void CMainGameScene::OnScene_Exit() //override
	{

	}
}