#include "EventProcessor.h"

EventProcessor::EventProcessor(std::vector<INPUT_RECORD>& eventQueue, 
							   std::mutex& queueAccess, HANDLE& newEventFlag)
{
    /**
    m_thread = std::thread(EventProcToolbox::ProcessEventQueue,
                           std::ref(eventQueue), 
                           std::ref(queueAccess),
                           std::ref(newEventFlag));
    **/

    m_thread = std::thread(&EventProcessor::ProcessEventQueue,
                           this,
                           std::ref(eventQueue),
                           std::ref(queueAccess),
                           std::ref(newEventFlag));
}

EventProcessor::~EventProcessor()
{
    m_thread.join();
}

void EventProcessor::ProcessEventQueue(std::vector<INPUT_RECORD>& eventQueue, 
                                       std::mutex& queueAccess, 
                                       HANDLE& newEventFlag)
{
    while (true)
    {
        if (WaitForSingleObject(newEventFlag, INFINITE) == WAIT_OBJECT_0)
        {
            std::lock_guard<std::mutex> lock(queueAccess);

            EventProcToolbox::EventProc(eventQueue.front(), 
                                        EventProcToolbox::KeyEventProc, 
                                        EventProcToolbox::ResizeEventProc, 
                                        EventProcToolbox::MouseEventProc, 
                                        EventProcToolbox::FocusEventProc);
            eventQueue.erase(eventQueue.begin());

            (eventQueue.empty() == true) ? ResetEvent(newEventFlag) : SetEvent(newEventFlag);
        }
    }
}
