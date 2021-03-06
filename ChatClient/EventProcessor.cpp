#include "EventProcessor.h"

EventProcessor::~EventProcessor()
{
    if (m_isActive == true)
    {
        Stop();
    }
}

void EventProcessor::Start(std::vector<INPUT_RECORD>& eventQueue,
                           std::mutex& queueAccess,
                           HANDLE& newEventFlag)
{
    m_isActive = true;

    m_thread = std::thread(&EventProcessor::ProcessEventQueue,
                            this,
                            std::ref(eventQueue),
                            std::ref(queueAccess),
                            std::ref(newEventFlag));
}

void EventProcessor::Stop()
{
    m_isActive = false;
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

            EventProc(eventQueue.front());
            eventQueue.erase(eventQueue.begin());

            (eventQueue.empty() == true) ? ResetEvent(newEventFlag) : SetEvent(newEventFlag);
        }
    }
}

void EventProcessor::EventProc(INPUT_RECORD& inputEvent)
{
    switch (inputEvent.EventType)
    {
    case KEY_EVENT:
        KeyEventProc(inputEvent.Event.KeyEvent);
        break;
    case MOUSE_EVENT:
        MouseEventProc(inputEvent.Event.MouseEvent);
        break;
    case WINDOW_BUFFER_SIZE_EVENT:
        ResizeEventProc(inputEvent.Event.WindowBufferSizeEvent);
        break;
    case FOCUS_EVENT:
        FocusEventProc(inputEvent.Event.FocusEvent);
        break;
    case MENU_EVENT:
        MenuEventProc(inputEvent.Event.MenuEvent);
        break;
    default:
        std::cout << "Unknown event type occured." << std::endl;
        break;
    }
}

void EventProcessor::KeyEventProc(KEY_EVENT_RECORD& ker)
{
    std::cout << "Key event: ";

    if (ker.bKeyDown)
    {
        std::cout << static_cast<char>(ker.uChar.UnicodeChar) << " key pressed." << std::endl;
    }
    else
    {
        std::cout << static_cast<char>(ker.uChar.UnicodeChar) << " key released." << std::endl;
    }
}

void EventProcessor::ResizeEventProc(WINDOW_BUFFER_SIZE_RECORD& wbsr)
{
    std::cout << "Resize event occured." << std::endl;
    std::cout << "Columns = " << wbsr.dwSize.X << "; ";
    std::cout << "Rows = " << wbsr.dwSize.Y << ";" << std::endl;
}

void EventProcessor::MouseEventProc(MOUSE_EVENT_RECORD& mer)
{
    std::cout << "Mouse event: ";

    switch (mer.dwEventFlags)
    {
    case 0:
        if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
        {
            std::cout << "left button pressed." << std::endl;
        }
        else if (mer.dwButtonState == RIGHTMOST_BUTTON_PRESSED)
        {
            std::cout << "right button pressed." << std::endl;
        }
        else
        {
            std::cout << "button pressed." << std::endl;
        }

        break;
    case DOUBLE_CLICK:
        std::cout << " double click." << std::endl;
        break;
    case MOUSE_HWHEELED:
        std::cout << " horizontal mouse wheel." << std::endl;
        break;
    case MOUSE_MOVED:
        std::cout << " mouse moved." << std::endl;
        break;
    case MOUSE_WHEELED:
        std::cout << " vertical mouse wheel." << std::endl;
        break;
    default:
        break;
    }
}

void EventProcessor::FocusEventProc(FOCUS_EVENT_RECORD& fer)
{
    std::cout << "Focus: switched." << std::endl;
}

void EventProcessor::MenuEventProc(MENU_EVENT_RECORD& mer)
{
}
