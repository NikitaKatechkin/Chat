#include "EventHandler.h"

EventHandler::EventHandler(const HANDLE& eventSource, 
                           const HANDLE& outputEventSource):
	m_eventSource(eventSource), 
    m_outputEventSource(outputEventSource)
{
    
}

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

void EventHandler::ServiceStartEventHandling()
{
    m_isEventHandlingStarted = true;

    while (m_isEventHandlingStarted == true)
    {
        CatchEvent();
        ProcessEvent();
    }
}

void EventHandler::StartEventHandling()
{
    m_eventHandlingThread = std::thread(&EventHandler::ServiceStartEventHandling, 
                                        this);
}

void EventHandler::StopEventHandling()
{
    m_isEventHandlingStarted = false;
    m_eventHandlingThread.join();
}
