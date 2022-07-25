#include "ConsoleEventHandler.h"

ConsoleEventHandler::ConsoleEventHandler(const HANDLE& eventSource, 
                                         const HANDLE& outputEventSource, 
                                         Frame* consoleFrame,
                                         const COORD& frameStartPos):
	EventHandler(eventSource, outputEventSource), 
    m_consoleFrame(consoleFrame), 
    m_frameStartPos(frameStartPos)
{
    
}

/**
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
**/

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
        COORD localFrameCursorPos = COORD{
            static_cast<short>(cursorPos.X - m_frameStartPos.X),
            static_cast<short>(cursorPos.Y - m_frameStartPos.Y)};

        COORD offset{0, 0};
        switch (ker.wVirtualKeyCode)
        {
        case VK_LEFT:
            offset = COORD{ -1, 0 };
            SetCursorPosition(COORD{ static_cast<short>(cursorPos.X + offset.X),
                                     static_cast<short>(cursorPos.Y + offset.Y) });

            break;
        case VK_RIGHT:
            offset = COORD{ 1, 0 };
            SetCursorPosition(COORD{ static_cast<short>(cursorPos.X + offset.X),
                                     static_cast<short>(cursorPos.Y + offset.Y) });

            break;
        case VK_UP:
            offset = COORD{ 0, -1 };
            SetCursorPosition(COORD{ static_cast<short>(cursorPos.X + offset.X),
                                     static_cast<short>(cursorPos.Y + offset.Y) });

            break;
        case VK_DOWN:
            offset = COORD{ 0, 1 };
            SetCursorPosition(COORD{ static_cast<short>(cursorPos.X + offset.X),
                                     static_cast<short>(cursorPos.Y + offset.Y) });

            break;
        case VK_BACK:
        {
            /**
            offset = COORD{ -1, 0 };

            std::wstring charToType = { L' ' };
            m_consoleFrame->PasteShape(charToType.c_str(), 
                                       COORD{ 1, 1 }, 
                                       localFrameCursorPos);

            SetCursorPosition(COORD{ static_cast<short>(cursorPos.X + offset.X),
                                     static_cast<short>(cursorPos.Y + offset.Y) });
            **/

            DeleteCharacterFromConsoleFrame();

            break;
        }
        case VK_SPACE:
        {
            /**
            offset = COORD{ 1, 0 };

            std::wstring charToType = { L' ' };
            m_consoleFrame->PasteShape(charToType.c_str(), 
                                       COORD{ 1, 1 }, 
                                       localFrameCursorPos);

            SetCursorPosition(COORD{ static_cast<short>(cursorPos.X + offset.X),
                                     static_cast<short>(cursorPos.Y + offset.Y) });
            **/

            PrintCharacterToConsoleFrame(L' ');

            break;
        }
        default:
        {
            if ((ker.wVirtualKeyCode >= 0x41) && (ker.wVirtualKeyCode <= 0x5A))
            {
                /**
                offset = COORD{ 1, 0 };
             
                std::wstring charToType = { static_cast<wchar_t>(ker.wVirtualKeyCode) };
                m_consoleFrame->PasteShape(charToType.c_str(), 
                                           COORD{ 1, 1 }, 
                                           localFrameCursorPos);

                SetCursorPosition(COORD{ static_cast<short>(cursorPos.X + offset.X),
                                     static_cast<short>(cursorPos.Y + offset.Y) });
                **/

                PrintCharacterToConsoleFrame(static_cast<wchar_t>(ker.wVirtualKeyCode));
            }

            break;
        }
        }

        //SetCursorPosition(COORD{ static_cast<short>(cursorPos.X + offset.X),
        //                         static_cast<short>(cursorPos.Y + offset.Y) });
        WriteToOutputHandle();
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

COORD ConsoleEventHandler::GetCursorPosition() const
{
    CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
    GetConsoleScreenBufferInfo(m_outputEventSource, &bufferInfo);

    return bufferInfo.dwCursorPosition;
}

void ConsoleEventHandler::SetCursorPosition(const COORD& newPos)
{
    SetConsoleCursorPosition(m_outputEventSource, newPos);
}

void ConsoleEventHandler::PrintCharacterToConsoleFrame(const wchar_t& character)
{
    COORD cursorPos = GetCursorPosition();
    COORD localFrameCursorPos = COORD{
        static_cast<short>(cursorPos.X - m_frameStartPos.X),
        static_cast<short>(cursorPos.Y - m_frameStartPos.Y) };
    COORD frameSize = m_consoleFrame->GetFrameSize();

    if ((localFrameCursorPos.X < frameSize.X - 1) &&
        (localFrameCursorPos.X > 0) && 
        (localFrameCursorPos.Y < frameSize.Y - 1) &&
        (localFrameCursorPos.Y > 0))
    {
        // Character can be printed
        m_consoleFrame->PasteShape(&character,
                                   COORD{ 1, 1 },
                                   localFrameCursorPos);

        COORD offset{ 1, 0 };

        if (localFrameCursorPos.X == frameSize.X - 2)
        {
            //Cursor is right before border
            if (localFrameCursorPos.Y == frameSize.Y - 2)
            {
                //Cursor is in right bottom corner
                offset = COORD{ 0, 0 };
            }
            else
            {
                offset = COORD{ static_cast<short>(1 - localFrameCursorPos.X), 1 };
            }
        }

        SetCursorPosition(COORD{ static_cast<short>(cursorPos.X + offset.X),
                                 static_cast<short>(cursorPos.Y + offset.Y) });
    }
}

void ConsoleEventHandler::DeleteCharacterFromConsoleFrame()
{
    COORD cursorPos = GetCursorPosition();
    COORD localFrameCursorPos = COORD{
        static_cast<short>(cursorPos.X - m_frameStartPos.X),
        static_cast<short>(cursorPos.Y - m_frameStartPos.Y) };
    COORD frameSize = m_consoleFrame->GetFrameSize();

    if ((localFrameCursorPos.X < frameSize.X - 1) &&
        (localFrameCursorPos.X > 0) &&
        (localFrameCursorPos.Y < frameSize.Y - 1) &&
        (localFrameCursorPos.Y > 0))
    {
        // Character can be printed
        m_consoleFrame->PasteShape(L" ",
                                   COORD{ 1, 1 },
                                   localFrameCursorPos);

        COORD offset{ -1, 0 };

        if (localFrameCursorPos.X == 1)
        {
            //Cursor is right after border
            if (localFrameCursorPos.Y == 1)
            {
                //Cursor is in left top corner
                offset = COORD{ 0, 0 };
            }
            else
            {
                offset = COORD{ static_cast<short>(frameSize.X - 3), -1 };
            }
        }

        SetCursorPosition(COORD{ static_cast<short>(cursorPos.X + offset.X),
                                 static_cast<short>(cursorPos.Y + offset.Y) });
    }
}

BOOL ConsoleEventHandler::WriteToOutputHandle() const
{
    if (m_consoleFrame->GetFrameBuffer() == nullptr)
    {
        return FALSE;
    }

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(m_outputEventSource, &csbi);

    COORD frameSize = m_consoleFrame->GetFrameSize();

    /**
    if ((csbi.dwSize.X != frameSize.X) || (csbi.dwSize.Y != frameSize.Y))
    {
        return FALSE;
    }
    **/

    DWORD bytesWritten = 0;

    BOOL result = WriteConsoleOutputCharacter(m_outputEventSource,
                                              m_consoleFrame->GetFrameBuffer(),
                                              m_consoleFrame->GetFrameCharLength(),
                                              m_frameStartPos,
                                              &bytesWritten);

    return result;
}

void ConsoleEventHandler::SetConsoleFrame(Frame* consoleFrame, 
                                          const COORD& frameStartPos)
{
    m_consoleFrame = consoleFrame;
    m_frameStartPos = frameStartPos;
}
