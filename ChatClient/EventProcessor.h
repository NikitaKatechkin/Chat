#pragma once

#include "EventProcToolbox.h"

class EventProcessor
{
public:
	EventProcessor(std::vector<INPUT_RECORD>& eventQueue, std::mutex& queueAccess, 
				   HANDLE& newEventFlag);
	~EventProcessor();
private:
	std::thread m_thread;

private:
	void ProcessEventQueue(std::vector<INPUT_RECORD>& eventQueue,
						   std::mutex& queueAccess, HANDLE& newEventFlag);
};