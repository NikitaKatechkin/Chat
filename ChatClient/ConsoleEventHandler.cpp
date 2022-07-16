#include "ConsoleEventHandler.h"

COORD GetCursorPosition() 
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
    GetConsoleScreenBufferInfo(h, &bufferInfo);
    return bufferInfo.dwCursorPosition;
}

void SetCursorPosition(int XPos, int YPos) 
{
    COORD coord;
    coord.X = XPos; coord.Y = YPos;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

ConsoleEventHandler::ConsoleEventHandler(const HANDLE& eventSource):
	EventHandler(eventSource)
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
    this->m_newEventFlag = CreateEvent(nullptr, TRUE, FALSE, L"NewEventOccuranceFlag");
    this->m_eventSource = other.m_eventSource;

    return *this;
}

void ConsoleEventHandler::KeyEventProc(KEY_EVENT_RECORD& ker)
{
    if (ker.bKeyDown)
    {
        CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &bufferInfo);

        auto cursorPos = bufferInfo.dwCursorPosition;

        COORD offset;
        switch (ker.wVirtualKeyCode)
        {
        case VK_LEFT:
            offset = COORD{ -1, 0 };

            SetConsoleCursorPosition(
                GetStdHandle(STD_OUTPUT_HANDLE),
                COORD{ static_cast<short>(cursorPos.X + offset.X),
                       static_cast<short>(cursorPos.Y + offset.Y) });

            break;
        case VK_RIGHT:
            offset = COORD{ 1, 0 };

            SetConsoleCursorPosition(
                GetStdHandle(STD_OUTPUT_HANDLE),
                COORD{ static_cast<short>(cursorPos.X + offset.X),
                       static_cast<short>(cursorPos.Y + offset.Y) });

            break;
        case VK_UP:
            offset = COORD{ 0, -1 };

            SetConsoleCursorPosition(
                GetStdHandle(STD_OUTPUT_HANDLE),
                COORD{ static_cast<short>(cursorPos.X + offset.X),
                       static_cast<short>(cursorPos.Y + offset.Y) });

            break;
        case VK_DOWN:
            offset = COORD{ 0, 1 };

            SetConsoleCursorPosition(
                GetStdHandle(STD_OUTPUT_HANDLE),
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
