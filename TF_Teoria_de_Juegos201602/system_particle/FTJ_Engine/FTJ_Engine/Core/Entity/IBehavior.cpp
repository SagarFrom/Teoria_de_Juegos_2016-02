#include "IBehavior.h"
#include "GameObject.h"

namespace FTJ
{
	IBehavior::IBehavior(CGameObject* _pGameObject) : IComponent(_pGameObject)
	{
		m_pTransform = _pGameObject->GetTransform();
	}


	IBehavior::~IBehavior()
	{
	}
}