#include "EventHandler.h"

EventHandler::EventHandler(const HANDLE& eventSource):
	m_eventSource(eventSource)
{
	m_newEventFlag = CreateEvent(nullptr, TRUE, FALSE, L"NewEventOccuranceFlag");

	if (m_newEventFlag == NULL)
	{
		std::stringstream errorMessage;

		errorMessage << "Failed to CreateEvent with GLE = ";
		errorMessage << GetLastError() << "." << std::endl;

		throw std::exception(errorMessage.str().c_str());
	}
}

EventHandler::EventHandler(const EventHandler& other):
    EventHandler(other.m_eventSource)
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
    this->m_newEventFlag = CreateEvent(nullptr, TRUE, FALSE, L"NewEventOccuranceFlag");
    this->m_eventSource = other.m_eventSource;

    return *this;
}

EventHandler::~EventHandler()
{
	if (m_newEventFlag != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_newEventFlag);
		m_newEventFlag = INVALID_HANDLE_VALUE;
	}
}

void EventHandler::CatchEvent(const DWORD inputBufferSize)
{
	DWORD numberOccuredEvents = 0;
	auto inputBuffer = std::shared_ptr<INPUT_RECORD[]>(new INPUT_RECORD[inputBufferSize]);

	if (ReadConsoleInput(m_eventSource, inputBuffer.get(),
		static_cast<DWORD>(inputBufferSize), &numberOccuredEvents) == TRUE)
	{
        std::lock_guard<std::mutex> lock(m_queueAccess);

		for (DWORD i = 0; i < numberOccuredEvents; i++)
		{
			m_eventQueue.push_back(inputBuffer[i]);
		}

		//SetEvent(m_newEventFlag);
	}
}

void EventHandler::ProcessEvent()
{
	//if (WaitForSingleObject(m_newEventFlag, INFINITE) == WAIT_OBJECT_0)
	//{
		std::lock_guard<std::mutex> lock(m_queueAccess);

        for (size_t i = 0; i < m_eventQueue.size(); i++)
        {
            EventProc(m_eventQueue.front());
            m_eventQueue.erase(m_eventQueue.begin());
        }

		//(m_eventQueue.empty() == true) ? ResetEvent(m_newEventFlag) : 
		//								 SetEvent(m_newEventFlag);
	//}
}

void EventHandler::EventProc(INPUT_RECORD& inputEvent)
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

void EventHandler::KeyEventProc(KEY_EVENT_RECORD& ker)
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

void EventHandler::ResizeEventProc(WINDOW_BUFFER_SIZE_RECORD& wbsr)
{
    std::cout << "Resize event occured." << std::endl;
    std::cout << "Columns = " << wbsr.dwSize.X << "; ";
    std::cout << "Rows = " << wbsr.dwSize.Y << ";" << std::endl;
}

void EventHandler::MouseEventProc(MOUSE_EVENT_RECORD& mer)
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

void EventHandler::FocusEventProc(FOCUS_EVENT_RECORD& fer)
{
    std::cout << "Focus: switched." << std::endl;
}

void EventHandler::MenuEventProc(MENU_EVENT_RECORD& mer)
{
}
