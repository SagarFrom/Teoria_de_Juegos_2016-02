#pragma once
#include <string>

#define OEAddComponent(gObj, T) gObj->AddComponent(T, typeid(*T).name())

namespace FTJ
{
	class CGameObject;

	class IComponent
	{
	protected:
		CGameObject* m_pGameObject;
		bool isEnabled;

	public:
		IComponent(CGameObject* _pGameObject);
		virtual ~IComponent();

		virtual const char* GetComponentType() const;

		void Enable();
		void Disable();

		bool IsEnabled() { return isEnabled; }

		CGameObject* GetGameObject() const { return m_pGameObject; }
	};

}