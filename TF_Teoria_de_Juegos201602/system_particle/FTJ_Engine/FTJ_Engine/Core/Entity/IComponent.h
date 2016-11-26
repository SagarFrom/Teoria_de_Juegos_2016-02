#pragma once
#include <string>

#define OEAddComponent(gObj, T) gObj->AddComponent(T, typeid(*T).name())

namespace FTJ
{
	class CGameObject;

	__declspec(align(16)) class IComponent
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

		void* operator new(size_t i)
		{
			return _mm_malloc(i, 16);
		}

			void operator delete(void* p)
		{
			_mm_free(p);
		}
	};

}