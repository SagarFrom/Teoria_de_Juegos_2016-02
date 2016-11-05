#pragma once
#include <vector>

namespace FTJ
{
	class IGameScene;
	class CCamera;

	class CEntityManager
	{
		friend class CGameObject;

		int lastDestroyedIndex = -1;
		std::vector<CGameObject*> m_vGameObjects;

		IGameScene* m_pGameScene;

	public:
		CEntityManager(IGameScene* _pGameScene);
		~CEntityManager();

		void Update(const float _deltaTime);
		void DestroyGameObjects();

		CGameObject* Create_EmptyGameObject();
		CCamera* Create_CameraGameObject();

		IGameScene* GetGameScene() const { return m_pGameScene; }

	};
}
