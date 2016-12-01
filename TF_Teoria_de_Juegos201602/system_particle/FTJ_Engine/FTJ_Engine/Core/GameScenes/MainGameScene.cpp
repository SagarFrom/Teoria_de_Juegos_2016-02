#include "MainGameScene.h"

#include "../Entity/EntityManager.h"
#include "../Entity/GameObject.h"
#include "../Entity/Transform.h"

#include "../Render/FTJ_RenderManager.h"
#include "../Render/Camera.h"
#include "../Render/MeshRenderer.h"

#include "../../Game/Entities/CameraBehavior.h"

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

		m_pCamera->GetGameObject()->GetTransform()->SetPosition(0, 3, -6.f);
		new CameraBehavior(m_pCamera->GetGameObject());

		CGameObject* boxObject = m_EntityManager.Create_EmptyGameObject();
		MeshRenderer* meshRenderer = new MeshRenderer(boxObject, "Robot_Walk.bin");

		XMVECTOR lookat = { 5.f, 0.f, 0.f, 1.f };
		boxObject->GetTransform()->LookAt(lookat);

		meshRenderer->SetTexture(L"Default");
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