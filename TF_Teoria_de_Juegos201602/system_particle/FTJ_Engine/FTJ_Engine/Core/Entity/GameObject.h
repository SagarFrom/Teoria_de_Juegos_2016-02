#pragma once
#include "IEntity.h"

#include <vector>
#include <string>
#include <unordered_map>
#include <typeinfo.h>

//--------------------------------------------------------------------------------


namespace FTJ
{
	#define OEGetComponent(gObj, T) reinterpret_cast<T*>(gObj->GetComponent(typeid(T).name()))
	class IComponent;
	class IBehavior;
	class CTransform;

	class IGameScene;

	class CGameObject : public IEntity
	{
		friend class CEntityManager;

		bool bIsActive;
		bool bToDestroy;

		CEntityManager* m_pEntityManager;
		std::unordered_map<std::string, IComponent*> m_mComponents;
		std::vector<IBehavior*> m_vBehaviors;

		//basic components references
		CTransform* m_pTransform;

		~CGameObject();
		CGameObject(CEntityManager* _em);

	public:
		void Destroy();
		void SetActive(bool _b);

		//COMPONENTS
		bool AddComponent(IComponent* componentType, std::string key);
		bool RemoveComponent(std::string componentName);
		IComponent* GetComponent(std::string componentName) const;

		template <typename T>
		T* FindComponentOfType()
		{
			auto iter = components.begin();
			for (; iter != components.end(); ++iter)
			{
				T* p = dynamic_cast<T*>(iter->second);
				if (p)
					return p;
			}
			return nullptr;
		}
		//END COMPS

		//behaviors
		void StartBehaviors();
		virtual void UpdateBehaviors(const float _fDeltaTime);

		CEntityManager* GetEntityManager() const { return m_pEntityManager; }
		IGameScene* GetGameScene() const;

		inline CTransform* GetTransform() const { return m_pTransform; }
		//friend bool ToDestroy(CEntityManager* _om, CGameObject* obj);
	};

}