#pragma once

#include "Publisher.h"

#include <Windows.h>
#include <sstream>
#include <queue>
#include <iostream>
#include <mutex>
#include <thread>

class EventHandler : public Publisher
{
public:
	EventHandler() = delete;
	EventHandler(const EventHandler& other) = delete;
	EventHandler& operator=(const EventHandler& other) = delete;

public:
	EventHandler(const HANDLE& eventSource, 
				 const HANDLE& outputEventSource);

	virtual ~EventHandler() = default;
	
public:
	void CatchEvent(const DWORD inputBufferSize = 12);
	void ProcessEvent();

protected:
	std::queue<INPUT_RECORD> m_eventQueue;
	std::mutex m_queueAccess;

protected:
	HANDLE m_eventSource = INVALID_HANDLE_VALUE;
	HANDLE m_outputEventSource = INVALID_HANDLE_VALUE;

protected:
	std::thread m_eventHandlingThread;
	bool m_isEventHandlingStarted = false;

protected:
	void ServiceStartEventHandling();

public:
	void StartEventHandling();
	void StopEventHandling();

};