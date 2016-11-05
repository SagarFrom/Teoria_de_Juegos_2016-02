#pragma once

#include <string>

namespace FTJ
{
	// type def para poder cambiar facilmente
	typedef std::string EVENTID;
	//typedef unsigned int EVENTID;
	//enum EVENTID { EVENTS_SIZE = 0};

	class CEvent {
		friend class CEventManager;

		EVENTID			m_ID;		// id del evento
		void*			m_pData;	// informacion adjunta
		void*			m_pSender;	// objeto que envio el evento

		CEvent(const CEvent&) = delete;	// Copy constructor
		CEvent& operator=	(const CEvent&) = delete;	// Assignment operator

	public:
		// Overloaded constructor
		CEvent(const char*		eventID,		// event ID / name (limited to 32 characters!)
			void*			data = nullptr,		// attached data (any type)
			void*			sender = nullptr	// object that sent the event
			);

		const EVENTID&	GetEventID	(void)	const	{ return m_ID; }
		void*			GetData		(void)	const	{ return m_pData; }
		void*			GetSender	(void)	const	{ return m_pSender; }

		// Destructor
		virtual ~CEvent(void) = default;

		// EventManager Interaction:
		bool			QueueEvent(const void* destination = nullptr)	const;	// event will be stored & deallocated by the EventManager
		bool			SendEventNow(const void* destination = nullptr)	const;	// event will NOT be stored or deallocated!

	};
	//end CEvent
}