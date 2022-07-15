#include "EventCollector.h"

EventCollector::EventCollector(HANDLE& eventSource, std::vector<INPUT_RECORD>& eventQueue,
							   std::mutex& queueAccess, HANDLE& newEventFlag, const size_t& inputBufferSize)
{
    /**
	m_thread = std::thread(EventProcToolbox::EventCollector,
						   std::ref(eventSource),
						   std::ref(eventQueue),
						   std::ref(queueAccess),
						   std::ref(newEventFlag),
						   std::ref(inputBufferSize));
    **/

    m_thread = std::thread(&EventCollector::CollectEvent,
                           this,
                           std::ref(eventSource),
                           std::ref(eventQueue),
                           std::ref(queueAccess),
                           std::ref(newEventFlag),
                           std::ref(inputBufferSize));
}

EventCollector::~EventCollector()
{
	m_thread.join();
}

void EventCollector::CollectEvent(HANDLE& eventSource, std::vector<INPUT_RECORD>& eventQueue, std::mutex& queueAccess, HANDLE& newEventFlag, const size_t& inputBufferSize)
{
    DWORD numberOccuredEvents = 0;
    auto inputBuffer = std::shared_ptr<INPUT_RECORD[]>(new INPUT_RECORD[inputBufferSize]);

    while (true)
    {
        if (ReadConsoleInput(eventSource, inputBuffer.get(),
            static_cast<DWORD>(inputBufferSize), &numberOccuredEvents) == TRUE)
        {
            for (DWORD i = 0; i < numberOccuredEvents; i++)
            {
                std::lock_guard<std::mutex> lock(queueAccess);

                eventQueue.push_back(inputBuffer[i]);
            }

            SetEvent(newEventFlag);
        }
    }
}
