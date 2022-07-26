#include "EventHandler.h"

EventHandler::EventHandler(const HANDLE& eventSource, 
                           const HANDLE& outputEventSource):
	m_eventSource(eventSource), 
    m_outputEventSource(outputEventSource)
{
    
}

/**
EventHandler::EventHandler(const EventHandler& other):
    EventHandler(other.m_eventSource, other.m_outputEventSource)
{
    m_eventQueue = other.m_eventQueue;
}

EventHandler& EventHandler::operator=(const EventHandler& other)
{
    if (this == &other)
    {
        return *this;
    }


    this->m_eventQueue = other.m_eventQueue;
    this->m_eventSource = other.m_eventSource;
    this->m_outputEventSource = other.m_outputEventSource;

    return *this;
}
**/

void EventHandler::CatchEvent(const DWORD inputBufferSize)
{
	DWORD numberOccuredEvents = 0;
	auto inputBuffer = std::shared_ptr<INPUT_RECORD[]>(new INPUT_RECORD[inputBufferSize]);

	if (ReadConsoleInput(m_eventSource, 
                         inputBuffer.get(),
		                 static_cast<DWORD>(inputBufferSize), 
                         &numberOccuredEvents) 
        == TRUE)
	{
        std::lock_guard<std::mutex> lock(m_queueAccess);

		for (size_t i = 0; i < numberOccuredEvents; i++)
		{
			m_eventQueue.push(inputBuffer[i]);
		}
	}
}

void EventHandler::ProcessEvent()
{
	std::lock_guard<std::mutex> lock(m_queueAccess);

    for (size_t i = 0; i < m_eventQueue.size(); i++)
    {
        //EventProc(m_eventQueue.front());
        Notify(m_eventQueue.front());
        m_eventQueue.pop();
    }
}
