#include "Listener.h"

// Uses assert for debug breaks
#include <cassert>

// Uses Event Manager
#include "EventManager.h"


namespace FTJ
{
	//*****************************************************************//
	// LISTENER METHODS

	// Overloaded constructor
	CListener::CListener(const void* child)
	{
		// Sanity-check the parameter
		assert(child != nullptr && "Listener::Listener - self cannot be null (should be the child object)");

		m_pChild = child;
	}

	// Destructor
	/*virtual*/ CListener::~CListener(void)
	{
		// Unregister from all events
		UnregisterFromEvent();
	}

	// EventManager Interactions:
	bool CListener::RegisterForEvent(const char* eventID)
	{
		return CEventManager::GetInstance()->RegisterForEvent(this, eventID);
	}

	bool CListener::UnregisterFromEvent(const char* eventID)
	{
		return CEventManager::GetInstance()->UnregisterFromEvent(this, eventID);
	}

	// Listener Interface:
	/*virtual*/ void CListener::HandleEvent(const CEvent* pEvent)
	{
		// DUMMY IMPLEMENTATION
		(void)pEvent;
	}
	//*****************************************************************//


}	// namespace FTJ
