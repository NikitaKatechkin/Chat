#pragma once

#include <windows.h>
#include <cstdio>
#include <iostream>
#include <vector>
#include <memory>
#include <mutex>
#include <thread>

class EventCollector
{
public:
	EventCollector(HANDLE& eventSource, std::vector<INPUT_RECORD>& eventQueue,
				   std::mutex& queueAccess, HANDLE& newEventFlag, 
				   const size_t& inputBufferSize = 128);
	~EventCollector();
private:
	std::thread m_thread;

	void CollectEvent(HANDLE& eventSource, std::vector<INPUT_RECORD>& eventQueue,
					  std::mutex& queueAccess, HANDLE& newEventFlag,
					  const size_t& inputBufferSize = 128);
};