#include "EventManager.h"
#include "Listener.h"

#include <assert.h>

namespace FTJ
{

	//*************************************************************//	
	// SINGLETON

	// Instantiate static pointer to null (no instance yet)
	/*static*/ CEventManager* CEventManager::s_Instance = nullptr;

	// Singleton accessor
	/*static*/ CEventManager* CEventManager::GetInstance(void)
	{
		// Allocate singleton on first use
		if (CEventManager::s_Instance == nullptr)
			CEventManager::s_Instance = new CEventManager;

		// Return the singleton
		return CEventManager::s_Instance;
	}

	// Singleton destructor
	/*static*/ void CEventManager::DeleteInstance(void)
	{
		// Deallocate singleton
		delete CEventManager::s_Instance;
		CEventManager::s_Instance = nullptr;
	}
	//*************************************************************//


	//*************************************************************//
	// INITIALIZE
	bool CEventManager::Initialize(void)
	{
		// Sanity-check the wrapper's status
		assert(m_eStatus != E_INITIALIZED && "EventManager::Initialize - wrapper has already been initialized");
		if (m_eStatus == E_INITIALIZED)
			return false;


		// Success!
		m_eStatus = E_INITIALIZED;
		return true;
	}
	//*************************************************************//



	//*************************************************************//
	// UPDATE
	bool CEventManager::Update(void)
	{
		// Sanity-check the wrapper's status
		assert(m_eStatus == E_INITIALIZED && "EventManager::Update - wrapper has not been initialized");
		if (m_eStatus != E_INITIALIZED)
			return false;


		// Iterate through the entire queue
		while (m_qEvents.empty() == false)
		{
			// Dequeue the first event
			EventDestinationPair eventPair = m_qEvents.front();
			m_qEvents.pop();


			// Hold a local ref to the ID (for readability)
			const EVENTID& id = eventPair.first->GetEventID();


			// Send the event to all registered listeners for processing
			LMapRange range = m_mListeners.equal_range(id);
			if (range.first != range.second)
			{
				// Copy the intended listeners
				ListenerVector vec;

				// All listeners?
				if (eventPair.second == nullptr)
				{
					for (LMapIter iter = range.first; iter != range.second; ++iter)
						vec.push_back(iter->second);
				}
				else
				{
					// One intended listener (which may not exist)
					for (LMapIter iter = range.first; iter != range.second; ++iter)
					{
						if (iter->second->GetChild() == eventPair.second)
						{
							vec.push_back(iter->second);
							break;
						}
					}
				}

				// Send the event to the current listeners
				for (unsigned int i = 0; i < vec.size(); i++)
				{
					// Has a listener been removed from this event?
					if (m_mUnlisteners.empty() == false)
					{
						LMapRange removed = m_mUnlisteners.equal_range(id);
						if (removed.first != removed.second)
						{
							// Is this listener marked for removal from this event?
							LMapIter iter = std::find(removed.first, removed.second, LMapValue(id, vec[i]));
							if (iter != removed.second)
							{
								m_mUnlisteners.erase(iter);

								// Skip over the HandleEvent
								continue;
							}
						}
					}

					// Send event
					vec[i]->HandleEvent(eventPair.first);
				}
			}

			// Deallocate the event
			delete eventPair.first;
		}


		// Unregistered listeners have all been processed
		m_mUnlisteners.clear();
		return true;
	}
	//*************************************************************//



	//*************************************************************//
	// TERMINATE
	bool CEventManager::Terminate(void)
	{
		// Sanity-check the wrapper's status
		assert(m_eStatus == E_INITIALIZED && "EventManager::Terminate - wrapper has not been initialized");
		if (m_eStatus != E_INITIALIZED)
			return false;


		// Deallocate all events in the queue
		while (m_qEvents.empty() == false)
		{
			delete m_qEvents.front().first;
			m_qEvents.pop();
		}

		// Remove the registered listeners
		m_mListeners.clear();
		m_mUnlisteners.clear();


		m_eStatus = E_DESTROYED;
		return true;
	}
	//*************************************************************//



	//*************************************************************//
	// REGISTER FOR EVENT
	bool CEventManager::RegisterForEvent(CListener* listener, const char* eventID)
	{
		// Sanity-check the wrapper's status
		assert(m_eStatus == E_INITIALIZED && "EventManager::RegisterForEvent - wrapper has not been initialized");
		if (m_eStatus != E_INITIALIZED)
			return false;

		// Sanity-check the parameters
		assert(listener != nullptr && "EventManager::RegisterForEvent - listener cannot be null");
		if (listener == nullptr)
			return false;

		assert(eventID != nullptr && "EventManager::RegisterForEvent - event ID cannot be null");
		if (eventID == nullptr)
			return false;


		// Convert the C-style string parameter to an EventID
		EVENTID id = eventID;


		// Check if the listener is NOT already registered
		LMapRange range = m_mListeners.equal_range(id);
		for (LMapIter iter = range.first; iter != range.second; ++iter)
			if (iter->second == listener)
				return true;		// already registered!


		// Register the new listener
		m_mListeners.insert(LMapValue(id, listener));
		return true;
	}
	//*************************************************************//



	//*************************************************************//
	// UNREGISTER FROM EVENT
	bool CEventManager::UnregisterFromEvent(CListener* listener, const char* eventID)
	{
		// Sanity-check the wrapper's status
		assert(m_eStatus == E_INITIALIZED && "EventManager::UnregisterFromEvent - wrapper has not been initialized");
		if (m_eStatus != E_INITIALIZED)
			return false;

		// Sanity-check the listener parameter
		assert(listener != nullptr && "EventManager::UnregisterFromEvent - listener cannot be null");
		if (listener == nullptr)
			return false;


		// Should we unregister from all events?
		if (eventID == nullptr)
		{
			// Unregister from all events
			LMapIter iter = m_mListeners.begin();
			while (iter != m_mListeners.end())
			{
				if (iter->second == listener)
				{
					m_mUnlisteners.insert(*iter);
					iter = m_mListeners.erase(iter);
					continue;	// may be listening to another event
				}

				++iter;
			}
		}
		else
		{
			// Find all registered listeners for the event
			EVENTID id = eventID;
			LMapRange range = m_mListeners.equal_range(id);

			// Unregister from the event
			LMapIter iter = range.first;
			while (iter != range.second)
			{
				if (iter->second == listener)
				{
					m_mUnlisteners.insert(*iter);
					iter = m_mListeners.erase(iter);
					break;		// cannot be listening to the same event again
				}

				++iter;
			}
		}

		return true;
	}
	//*************************************************************//



	//*************************************************************//
	// QUEUE EVENT
	bool CEventManager::QueueEvent(const CEvent* pEvent, const void* destination)
	{
		// Sanity-check the wrapper's status
		assert(m_eStatus == E_INITIALIZED && "EventManager::QueueEvent - wrapper has not been initialized");
		if (m_eStatus != E_INITIALIZED)
			return false;

		// Sanity-check the parameter
		assert(pEvent != nullptr && "EventManager::QueueEvent - event parameter cannot be null");
		if (pEvent == nullptr)
			return false;


		// Queue the event
		m_qEvents.push(EventDestinationPair{ pEvent, reinterpret_cast< const CListener* >(destination) });

		return true;
	}
	//*************************************************************//



	//*************************************************************//
	// SEND EVENT NOW
	bool CEventManager::SendEventNow(const CEvent* pEvent, const void* destination)
	{
		// Sanity-check the wrapper's status
		assert(m_eStatus == E_INITIALIZED && "EventManager::SendMessageNow - wrapper has not been initialized");
		if (m_eStatus != E_INITIALIZED)
			return false;

		// Sanity-check the parameter
		assert(pEvent != nullptr && "EventManager::SendEventNow - event parameter cannot be null");
		if (pEvent == nullptr)
			return false;


		// Hold a local ref (for readability)
		const EVENTID& id = pEvent->GetEventID();


		// Send the event to all registered listeners to process
		// (does not deallocate the event)
		LMapRange range = m_mListeners.equal_range(id);
		if (range.first != range.second)
		{
			// Copy the intended listeners
			ListenerVector vec;

			// All listeners?
			if (destination == nullptr)
			{
				for (LMapIter iter = range.first; iter != range.second; ++iter)
					vec.push_back(iter->second);
			}
			else
			{
				// One intended listener (which may not exist)
				for (LMapIter iter = range.first; iter != range.second; ++iter)
				{
					if (iter->second->GetChild() == destination)
					{
						vec.push_back(iter->second);
						break;
					}
				}
			}


			// Send the event to the current listeners
			for (unsigned int i = 0; i < vec.size(); i++)
			{
				// Has this listener been removed from the event?
				if (m_mUnlisteners.empty() == false)
				{
					LMapRange removed = m_mUnlisteners.equal_range(id);
					if (removed.first != removed.second)
					{
						LMapIter iter = std::find(removed.first, removed.second, LMapValue(id, vec[i]));
						if (iter != removed.second)
							continue;
					}
				}

				// Send event
				vec[i]->HandleEvent(pEvent);
			}
		}

		return true;
	}
	//*************************************************************//



	//*************************************************************//
	// CLEAR EVENTS
	bool CEventManager::ClearEvents(void)
	{
		// Sanity-check the wrapper's status
		assert(m_eStatus == E_INITIALIZED && "EventManager::ClearEvents - wrapper has not been initialized");
		if (m_eStatus != E_INITIALIZED)
			return false;


		// Deallocate all messages in the queue
		while (m_qEvents.empty() == false)
		{
			delete m_qEvents.front().first;
			m_qEvents.pop();
		}


		// Unregistered listeners have been effectively been processed
		m_mUnlisteners.clear();
		return true;
	}
	//*************************************************************//


}