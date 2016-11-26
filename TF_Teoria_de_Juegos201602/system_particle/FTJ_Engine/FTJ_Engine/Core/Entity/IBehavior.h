#pragma once
#include "IComponent.h"
#include "Transform.h"

namespace FTJ
{
	 class IBehavior : public IComponent
	{
		friend class CGameObject;

		//EntityManager
		virtual void Start(){};
		virtual void Update(const float fDeltaTime){};//gets call through the ObjectManager
		
	protected:
		CTransform* m_pTransform;

	public:
		IBehavior(CGameObject* _pGameObject);
		virtual ~IBehavior();
	};

}