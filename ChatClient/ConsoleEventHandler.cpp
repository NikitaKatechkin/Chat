#include "ConsoleEventHandler.h"

ConsoleEventHandler::ConsoleEventHandler(const HANDLE& eventSource, 
                                         const HANDLE& outputEventSource, 
                                         Frame* consoleFrame):
	EventHandler(eventSource, outputEventSource), m_consoleFrame(consoleFrame)
{
    
}

ConsoleEventHandler::ConsoleEventHandler(const ConsoleEventHandler& other):
    EventHandler(other)
{
    this->m_consoleFrame = other.m_consoleFrame;
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
    this->m_consoleFrame = other.m_consoleFrame;

    return *this;
}

void ConsoleEventHandler::EventProc(INPUT_RECORD& inputEvent)
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
        case VK_BACK:
        {
            std::wstring charToType = { L' ' };
            m_consoleFrame->PasteShape(charToType.c_str(), COORD{ 1, 1 }, cursorPos);

            offset = COORD{ -1, 0 };
            SetCursorPosition(
                COORD{ static_cast<short>(cursorPos.X + offset.X),
                       static_cast<short>(cursorPos.Y + offset.Y) });

            break;
        }
        case VK_SPACE:
        {
            std::wstring charToType = { L' ' };
            m_consoleFrame->PasteShape(charToType.c_str(), COORD{ 1, 1 }, cursorPos);

            offset = COORD{ 1, 0 };
            SetCursorPosition(
                COORD{ static_cast<short>(cursorPos.X + offset.X),
                       static_cast<short>(cursorPos.Y + offset.Y) });

            break;
        }
        default:
        {
            if ((ker.wVirtualKeyCode >= 0x41) && (ker.wVirtualKeyCode <= 0x5A))
            {
                std::wstring charToType = { static_cast<wchar_t>(ker.wVirtualKeyCode) };
                m_consoleFrame->PasteShape(charToType.c_str(), COORD{ 1, 1 }, cursorPos);

                offset = COORD{ 1, 0 };
                SetCursorPosition(
                    COORD{ static_cast<short>(cursorPos.X + offset.X),
                           static_cast<short>(cursorPos.Y + offset.Y) });
            }

            break;
        }
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

BOOL ConsoleEventHandler::WriteToOutputHandle(wchar_t* bufferToWrite, const COORD& bufferSize)
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(m_outputEventSource, &csbi);

    if ((csbi.dwSize.X != bufferSize.X) || (csbi.dwSize.Y != bufferSize.Y))
    {
        return FALSE;
    }

    DWORD bytesWritten = 0;

    BOOL result = WriteConsoleOutputCharacter(m_outputEventSource,
                                              bufferToWrite,
                                              bufferSize.X * bufferSize.Y,
                                              COORD {0, 0},
                                              &bytesWritten);

    return result;
}

void ConsoleEventHandler::SetConsoleFrame(Frame* consoleFrame)
{
    m_consoleFrame = consoleFrame;
}
