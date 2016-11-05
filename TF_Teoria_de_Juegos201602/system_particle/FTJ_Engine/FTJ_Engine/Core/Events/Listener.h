#pragma once

namespace FTJ
{
	class CEvent;

	//*****************************************************************//
	// Listener
	//	- parent class for objects registered with the Event Manager
	//	- children classes override the HandleEvent method, which is called
	//	  when a registered Event is processed
	//	- MUST be given a pointer to the child object when constructed!
	class CListener
	{
	public:
		CListener(const void* child);		// Overloaded constructor
		virtual ~CListener(void);					// Destructor


		// EventManager Interactions:
		bool			RegisterForEvent(const char* eventID);
		bool			UnregisterFromEvent(const char* eventID = nullptr);
		const void*		GetChild(void) const	{ return m_pChild; }


		// Listener Interface:
		virtual void	HandleEvent(const CEvent* pEvent);	// Callback function to process events

	private:
		const void*		m_pChild;	// address of the uncasted child object
	};

}