#include "Event.h"
#include "EventManager.h"

#include <cassert>

namespace FTJ
{
	// Overloaded constructor
	CEvent::CEvent
		(
		const char*		eventID,		// event ID / name
		void*			data,			// attached data (any type)
		void*			sender			// object that sent the event
		)
		: m_pData{ data }, m_pSender{ sender }
	{
		// Validate the ID
		assert(eventID != nullptr && "Event::Event - event ID cannot be null!");
		if (eventID == nullptr)
			return;

		assert(strlen(eventID) < 32 && "Event::Event - event ID length cannot exceed 31 characters!");
		if (strlen(eventID) >= 32)
			return;


		// Store the ID		
		m_ID = eventID;
	}

	// EventManager Interaction:
	bool CEvent::QueueEvent(const void* destination) const
	{
		return CEventManager::GetInstance()->QueueEvent(this, destination);
	}

	bool CEvent::SendEventNow(const void* destination) const
	{
		return CEventManager::GetInstance()->SendEventNow(this, destination);
	}
	//*****************************************************************//


}	// namespace FTJ
