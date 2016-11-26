#include "GameObject.h"
#include "../Render/FTJ_RenderManager.h"
#include "EntityManager.h"
#include "IComponent.h"
#include "IBehavior.h"
#include "Transform.h"

namespace FTJ
{
	CGameObject::CGameObject(CEntityManager* _em)
	{
		m_pEntityManager = _em;
		m_pEntityManager->m_vGameObjects.push_back(this);

		bToDestroy = false;
	}
	CGameObject::~CGameObject()
	{
		//delete all components
		auto iter = m_mComponents.begin();
		for (; iter != m_mComponents.end(); ++iter)
			delete iter->second;
		m_mComponents.clear();

		m_vBehaviors.clear();
		//transform = nullptr;
	}

	void CGameObject::SetActive(bool _b)
	{
		bIsActive = _b;

		auto iter = m_mComponents.begin();
		for (; iter != m_mComponents.end(); ++iter)
		{
			if (_b)
				iter->second->Enable();
			else
				iter->second->Disable();
		}
	}
	void CGameObject::Destroy()
	{
		bToDestroy = true;
	}

	void CGameObject::StartBehaviors()
	{
		unsigned size = m_vBehaviors.size();
		for (unsigned i = 0; i < size; i++)
		{
			m_vBehaviors[i]->Start();
		}
	}

	/*virtual*/ void CGameObject::UpdateBehaviors(const float _fDeltaTime)
	{
		if (bIsActive == false) return;

		unsigned size = m_vBehaviors.size();
		for (unsigned i = 0; i < size; i++)
		{
			if (m_vBehaviors[i]->IsEnabled())
				m_vBehaviors[i]->Update(_fDeltaTime);
		}
	}

	//COMPONENTS
	bool CGameObject::AddComponent(IComponent* component, std::string key)
	{
		if (component == nullptr)
			return false;

		IBehavior* beh = dynamic_cast<IBehavior*>(component);
		if (beh)
		{//is a behavior
			m_vBehaviors.push_back(beh);
		}

		CTransform* transform = dynamic_cast<CTransform*>(component);
		if (transform)
		{//is a behavior
			m_pTransform = transform;
		}

		m_mComponents[key] = component;
		return true;
	}
	bool CGameObject::RemoveComponent(std::string componentName)
	{
		auto iter = m_mComponents.find(componentName);
		if (iter == m_mComponents.end())
			return false;

		m_mComponents.erase(iter);

		IBehavior* beh = dynamic_cast<IBehavior*>(iter->second);
		if (beh)
		{
			//remove from behavior list too
			auto behIter = m_vBehaviors.begin();
			for (; behIter != m_vBehaviors.end(); ++behIter)
			{
				if (*behIter == beh)
				{
					m_vBehaviors.erase(behIter);
					break;
				}
			}
		}

		delete iter->second;
		return true;
	}
	IComponent* CGameObject::GetComponent(std::string componentName) const
	{
		auto iter = m_mComponents.find(componentName);
		if (iter == m_mComponents.end())
			return nullptr;

		IComponent* comp = iter->second;
		return comp;
	}

	IGameScene* CGameObject::GetGameScene() const { return m_pEntityManager->GetGameScene(); }
}