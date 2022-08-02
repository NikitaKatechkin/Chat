#include "EventHandler.h"

EventHandler::EventHandler(const HANDLE& eventSource, 
                           const HANDLE& outputEventSource):
	m_eventSource(eventSource), 
    m_outputEventSource(outputEventSource)
{
    if (m_eventSource == INVALID_HANDLE_VALUE)
    {
        std::stringstream errorMessage;

        errorMessage << "Invalid event source provided." << std::endl;

        throw std::exception(errorMessage.str().c_str());
    }

    if (m_outputEventSource == INVALID_HANDLE_VALUE)
    {
        std::stringstream errorMessage;

        errorMessage << "Invalid output of event source provided." << std::endl;

        throw std::exception(errorMessage.str().c_str());
    }
}

EventHandler::~EventHandler()
{
    if (m_isEventHandlingStarted == true)
    {
        StopEventHandling();
    }
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
    if (m_isEventHandlingStarted == false)
    {
        //std::cout << "Starting" << std::endl;

        m_eventHandlingThread = std::thread(&EventHandler::ServiceStartEventHandling,
                                            this);
    }
}

void EventHandler::StopEventHandling()
{
    if (m_isEventHandlingStarted == true)
    {
        m_isEventHandlingStarted = false;

        /**
        {
            COORD consoleSize = COORD{ 80, 30 };
            SMALL_RECT windowSize = { 0,
                                      0,
                                      consoleSize.X - 1,
                                      consoleSize.Y - 1 };

            SetConsoleWindowInfo(m_outputEventSource, TRUE, &windowSize);
            SetConsoleScreenBufferSize(m_outputEventSource, consoleSize);
        }
        **/

        //std::cout << "Stopping" << std::endl;

        m_eventHandlingThread.join();
    }
}
