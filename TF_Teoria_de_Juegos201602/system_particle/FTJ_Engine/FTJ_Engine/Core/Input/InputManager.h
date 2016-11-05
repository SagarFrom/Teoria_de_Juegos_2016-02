#pragma once
#include <Windows.h>
#include <vector>

#include "../Events/Event.h"

namespace FTJ
{
	class CInputManager
	{
		enum eKeyStates { KEY_CLEAR = 0, KEY_PRESS, KEY_DOWN, KEY_RELEASE, KEY_UP };
		struct tMouseInput {
			unsigned int x, y, prevX, prevY;
			int m_nScrollPos;
		};
		struct tKey {
			eKeyStates		nKeyState;
			float			fHeldDuration;
			unsigned int	unRefCount;
		};

		tKey m_Keys[256];
		tMouseInput m_tMouse;
		std::vector<unsigned char> m_ActiveKeys;
		std::vector<EVENTID> m_ActiveKeyPressEvents;
		std::vector<EVENTID> m_ActiveKeyDownEvents;
		std::vector<EVENTID> m_ActiveKeyReleaseEvents;

		// for tracking any key useful for typing or waking up from attract states
		unsigned char	m_ucLastKeyCode;
		tKey			m_LastKeyState;

		// helper functions
		void KeyDown(const unsigned char VKcode);
		void KeyUp(const unsigned char VKcode);
		void ScrollUp() { ++m_tMouse.m_nScrollPos; }
		void ScrollDown() { m_tMouse.m_nScrollPos--; }

		//Por Hacer:
		//Declarar unica instancia
		static CInputManager* s_pInstance;

		CInputManager();
		~CInputManager();
		CInputManager(const CInputManager &);
		CInputManager & operator=(const CInputManager &);

	public:
		bool HandleWindowMessages(const UINT uMsg, const WPARAM wParam, const LPARAM lParam);
		
		static CInputManager* GetInstance();
		static void CreateInstance();
		static void DestroyInstance();

		// starts tracking the key
		void ActivateKey(const unsigned char VKcode);
		// activates the key if needed
		// if an event already exists it will return the old event (easier for multiple event handlers)
		EVENTID BindKeyEvent(const unsigned char VKcode, eKeyStates keyState, EVENTID Event);
		EVENTID BindKeyPressEvent(const unsigned char VKcode, EVENTID Event);
		EVENTID BindKeyDownEvent(const unsigned char VKcode, EVENTID Event);
		EVENTID BindKeyReleaseEvent(const unsigned char VKcode, EVENTID Event);
		// will stop throwing the event bound to this keystate
		void UnBindKeyEvent(const unsigned char VKcode, eKeyStates keyState);
		// will unbind all events and no longer track the key
		void DeActivateKey(const unsigned char VKcode);
		// sends events for current key states and then updates the keys for next frame
		// Note: if multiple keys were pressed between the frame the events won't match the order they were pressed.
		void UpdateKeyStates(const float fDeltaTime);
		inline const tMouseInput& GetMouseInput() const { return m_tMouse; }

		// used for when losing window focus
		void ResetInput();

		// this is not the most efficient way to do things but is very simple for testing purposes
		// Note: the key to be checked must be active in order for this to return an accurate state
		const eKeyStates GetKeyState(const unsigned char VKcode)const;
		const bool GetKeyDown(const unsigned char VKcode)const;
		const bool GetKeyUp(const unsigned char VKcode)const;
		const bool GetKeyRelease(const unsigned char VKcode)const;
		const bool GetKeyPress(const unsigned char VKcode)const;
	};

}