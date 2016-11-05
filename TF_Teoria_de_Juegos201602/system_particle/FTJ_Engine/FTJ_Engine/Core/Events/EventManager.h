#pragma once

#include "Event.h"

#include <map>
#include <queue>

namespace FTJ
{
	// Forward declarations
	class CListener;

	// EventManager
	//	- SINGLETON class for queuing events sent to registered Listeners
	class CEventManager
	{
	public:
		static	CEventManager*	GetInstance(void);
		static	void			DeleteInstance(void);


		virtual	bool		Initialize(void) ;
		virtual	bool		Update(void) ;
		virtual	bool		Terminate(void) ;

		virtual bool		RegisterForEvent(CListener* listener, const char* eventID);
		virtual bool		UnregisterFromEvent(CListener* listener, const char* eventID = nullptr);

		virtual bool		QueueEvent(const CEvent* pEvent, const void* destination = nullptr);
		virtual bool		SendEventNow(const CEvent* pEvent, const void* destination = nullptr);

		virtual bool		ClearEvents(void);


	protected:
		// SINGLETON
		static	CEventManager*		s_Instance;		// the ONE instance

		CEventManager(void) = default;		// Default constructor
		virtual	~CEventManager(void) = default;		// Destructor

		CEventManager(const CEventManager&) = delete;		// Copy constructor
		CEventManager&	operator=	(const CEventManager&) = delete;		// Assignment operator


		// Wrapper Status
		enum EEventManagerStatus
		{
			E_UNINITIALIZED,
			E_INITIALIZED,
			E_DESTROYED
		};

		EEventManagerStatus			m_eStatus = E_UNINITIALIZED;		// wrapper initialization status

		typedef	std::pair< const CEvent*, const void* >	EventDestinationPair;
		typedef std::queue< EventDestinationPair >		EventQueue;
		EventQueue					m_qEvents;							// event queue

		typedef std::multimap< EVENTID, CListener* >	ListenerMap;
		typedef ListenerMap::const_iterator			LMapIter;
		typedef ListenerMap::value_type				LMapValue;
		typedef std::pair< LMapIter, LMapIter >		LMapRange;
		typedef std::vector< CListener* >			ListenerVector;
		ListenerMap					m_mListeners;				// registered listeners
		ListenerMap					m_mUnlisteners;				// unregistered listeners
	};

}	// namespace FTJ
