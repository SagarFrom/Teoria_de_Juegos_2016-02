#pragma once
#include "IComponent.h"

namespace FTJ
{
	class IBehavior : public IComponent
	{
		friend class CGameObject;

		//EntityManager
		virtual void Start(){};
		virtual void Update(const float fDeltaTime){};//gets call through the ObjectManager

	public:
		IBehavior(CGameObject* _pGameObject);
		virtual ~IBehavior();
	};

}