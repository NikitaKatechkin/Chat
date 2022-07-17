#include "ConsoleEventHandler.h"

ConsoleEventHandler::ConsoleEventHandler(const HANDLE& eventSource, const HANDLE& outputEventSource):
	EventHandler(eventSource, outputEventSource)
{
}

ConsoleEventHandler::ConsoleEventHandler(const ConsoleEventHandler& other):
    EventHandler(other)
{
}

ConsoleEventHandler& ConsoleEventHandler::operator=(const ConsoleEventHandler& other)
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

void ConsoleEventHandler::KeyEventProc(KEY_EVENT_RECORD& ker)
{
    if (ker.bKeyDown)
    {
        auto cursorPos = GetCursorPosition();

        COORD offset;
        switch (ker.wVirtualKeyCode)
        {
        case VK_LEFT:
            offset = COORD{ -1, 0 };

            SetCursorPosition(
                COORD{ static_cast<short>(cursorPos.X + offset.X),
                       static_cast<short>(cursorPos.Y + offset.Y) });

            break;
        case VK_RIGHT:
            offset = COORD{ 1, 0 };

            SetCursorPosition(
                COORD{ static_cast<short>(cursorPos.X + offset.X),
                       static_cast<short>(cursorPos.Y + offset.Y) });

            break;
        case VK_UP:
            offset = COORD{ 0, -1 };

            SetCursorPosition(
                COORD{ static_cast<short>(cursorPos.X + offset.X),
                       static_cast<short>(cursorPos.Y + offset.Y) });

            break;
        case VK_DOWN:
            offset = COORD{ 0, 1 };

            SetCursorPosition(
                COORD{ static_cast<short>(cursorPos.X + offset.X),
                       static_cast<short>(cursorPos.Y + offset.Y) });

            break;
        default:
            break;
        }
    }
    else
    {
        //std::cout << "Key released." << std::endl;
    }
}

void ConsoleEventHandler::ResizeEventProc(WINDOW_BUFFER_SIZE_RECORD& wbsr)
{
}

void ConsoleEventHandler::MouseEventProc(MOUSE_EVENT_RECORD& mer)
{
}

void ConsoleEventHandler::FocusEventProc(FOCUS_EVENT_RECORD& fer)
{
}

void ConsoleEventHandler::MenuEventProc(MENU_EVENT_RECORD& mer)
{
}

COORD ConsoleEventHandler::GetCursorPosition()
{
    CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
    GetConsoleScreenBufferInfo(m_outputEventSource, &bufferInfo);

    return bufferInfo.dwCursorPosition;
}

void ConsoleEventHandler::SetCursorPosition(const COORD& newPos)
{
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), newPos);
}
