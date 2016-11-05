#include "IBehavior.h"

namespace FTJ
{
	IBehavior::IBehavior(CGameObject* _pGameObject) : IComponent(_pGameObject)
	{
	}


	IBehavior::~IBehavior()
	{
	}
}