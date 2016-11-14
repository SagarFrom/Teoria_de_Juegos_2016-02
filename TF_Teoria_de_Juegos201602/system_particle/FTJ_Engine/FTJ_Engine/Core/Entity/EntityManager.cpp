#include "EntityManager.h"
#include "GameObject.h"

#include "Transform.h"
#include "../Render/MeshRenderer.h"
#include "../Render/Camera.h"

namespace FTJ
{

	CEntityManager::CEntityManager(IGameScene* _pGameScene) :
		m_pGameScene(_pGameScene)
	{

	}

	CEntityManager::~CEntityManager()
	{
		int nObjects = m_vGameObjects.size();
		for (int i = 0; i < nObjects; i++)
		{
			delete m_vGameObjects[i];
		}
		m_vGameObjects.clear();
	}

	void CEntityManager::Update(const float _deltaTime)
	{
		int nObjects = m_vGameObjects.size();
		for (int i = 0; i < nObjects; i++)
		{
			m_vGameObjects[i]->UpdateBehaviors(_deltaTime);
		}

	}

	void CEntityManager::DestroyGameObjects()
	{
		int nObjects = m_vGameObjects.size();
		for (int i = 0; i < nObjects; i++)
		{
			if (m_vGameObjects[i]->bToDestroy)
			{
				delete m_vGameObjects[i];
				m_vGameObjects[i] = NULL;

				lastDestroyedIndex = i;
			}
		}
	}

	CGameObject* CEntityManager::Create_EmptyGameObject()
	{
		CGameObject* object = new CGameObject(this);
		
		//add transform
		new CTransform(object);

		return object;
	}

	// Para el dragon
	//CGameObject * CEntityManager::DragonObject(const char  _nombreModel)
	//{
	//	CGameObject * dragon = Create_EmptyGrameObject();
	//	MeshRenderer  * rendered = new MeshRenderer
	//		(dragon, "Dragon.fbx", FTJ::ShaderDefinitions::eDepth::Playable, false);

	//	dragon->GetTransform()->SetPosition(10, 5, 4);

	//	return dragon;
	//}


	CCamera* CEntityManager::Create_CameraGameObject()
	{
		CGameObject* object = Create_EmptyGameObject();

		CCamera *camera = new CCamera(object);

		return camera;
	}
}