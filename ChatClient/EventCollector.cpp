#include "EventCollector.h"

EventCollector::~EventCollector()
{
    if (m_isActive == true)
    {
        Stop();
    }
}

void EventCollector::Start(HANDLE& eventSource, std::vector<INPUT_RECORD>& eventQueue,
    std::mutex& queueAccess, HANDLE& newEventFlag,
    const size_t& inputBufferSize)
{
    m_isActive = true;
    m_thread = std::thread(&EventCollector::CollectEvent,
                            this,
                            std::ref(eventSource),
                            std::ref(eventQueue),
                            std::ref(queueAccess),
                            std::ref(newEventFlag),
                            std::ref(inputBufferSize));
}

void EventCollector::Stop()
{
    m_isActive = false;
    m_thread.join();
}

void EventCollector::CollectEvent(HANDLE& eventSource, std::vector<INPUT_RECORD>& eventQueue, std::mutex& queueAccess, HANDLE& newEventFlag, const size_t& inputBufferSize)
{
    DWORD numberOccuredEvents = 0;
    auto inputBuffer = std::shared_ptr<INPUT_RECORD[]>(new INPUT_RECORD[inputBufferSize]);

    while (m_isActive == true)
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
