#include "IComponent.h"
#include "GameObject.h"

#include <typeinfo.h>

namespace FTJ 
{
	IComponent::IComponent(CGameObject* _pGameObject)
	{
		m_pGameObject = _pGameObject;

		//_pGameObject->AddComponent(this, typeid(*this).name());
	}

	IComponent::~IComponent()
	{
	}

	/*virtual*/ const char* IComponent::GetComponentType() const
	{
		return typeid(*this).name();
	}

	void IComponent::Enable()
	{
		this->isEnabled = true;
	}
	void IComponent::Disable()
	{
		this->isEnabled = false;
	}
}
