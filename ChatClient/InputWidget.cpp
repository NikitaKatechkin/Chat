#include "InputWidget.h"

InputWidget::InputWidget(const COORD& renderStartPoint, 
						 const COORD& widgetSize,
                         const bool drawBorders):
	Widget(renderStartPoint, widgetSize, drawBorders)
{
	
}

InputWidget::InputWidget(const Frame& widgetFrame, 
                         const COORD& renderStartPoint, 
                         const COORD& widgetSize):
    Widget(widgetFrame, renderStartPoint, widgetSize)
{

}

BOOL InputWidget::Update()
{
	return TRUE;
}

void InputWidget::Update(INPUT_RECORD& inputEvent)
{
    EventProc(inputEvent);
}

COORD InputWidget::GetCursorPosition() const
{
    CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
    GetConsoleScreenBufferInfo(m_consoleOutput, &bufferInfo);

    return bufferInfo.dwCursorPosition;
}

void InputWidget::SetCursorPosition(const COORD& newPos)
{
    SetConsoleCursorPosition(m_consoleOutput, newPos);
}

void InputWidget::PrintCharacterToConsoleFrame(const wchar_t& character)
{
    COORD cursorPos = GetCursorPosition();
    COORD localFrameCursorPos = COORD{
        static_cast<short>(cursorPos.X - m_renderStartPoint.X),
        static_cast<short>(cursorPos.Y - m_renderStartPoint.Y) };
    COORD frameSize = m_widgetFrame.GetFrameSize();

    if ((localFrameCursorPos.X < frameSize.X - 1) &&
        (localFrameCursorPos.X > 0) &&
        (localFrameCursorPos.Y < frameSize.Y - 1) &&
        (localFrameCursorPos.Y > 0))
    {
        // Character can be printed
        m_widgetFrame.PasteShape(&character, COORD{ 1, 1 }, localFrameCursorPos);

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

void InputWidget::DeleteCharacterFromConsoleFrame()
{
    COORD cursorPos = GetCursorPosition();
    COORD localFrameCursorPos = COORD{
        static_cast<short>(cursorPos.X - m_renderStartPoint.X),
        static_cast<short>(cursorPos.Y - m_renderStartPoint.Y) };
    COORD frameSize = m_widgetFrame.GetFrameSize();

    if ((localFrameCursorPos.X < frameSize.X - 1) &&
        (localFrameCursorPos.X > 0) &&
        (localFrameCursorPos.Y < frameSize.Y - 1) &&
        (localFrameCursorPos.Y > 0))
    {
        // Character can be printed
        m_widgetFrame.PasteShape(L" ", COORD{ 1, 1 }, localFrameCursorPos);

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

void InputWidget::EventProc(INPUT_RECORD& inputEvent)
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

void InputWidget::KeyEventProc(KEY_EVENT_RECORD& ker)
{
    if (ker.bKeyDown)
    {
        auto cursorPos = GetCursorPosition();
        COORD localFrameCursorPos = COORD{
            static_cast<short>(cursorPos.X - m_renderStartPoint.X),
            static_cast<short>(cursorPos.Y - m_renderStartPoint.Y) };

        COORD offset{ 0, 0 };
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
            DeleteCharacterFromConsoleFrame();

            break;
        }
        case VK_SPACE:
        {
            PrintCharacterToConsoleFrame(L' ');

            break;
        }
        default:
        {
            if ((ker.wVirtualKeyCode >= 0x41) && (ker.wVirtualKeyCode <= 0x5A))
            {
                PrintCharacterToConsoleFrame(static_cast<wchar_t>(ker.wVirtualKeyCode));
            }

            break;
        }
        }

        DisplayWidget();
    }
    else
    {
        //std::cout << "Key released." << std::endl;
    }
}

void InputWidget::ResizeEventProc(WINDOW_BUFFER_SIZE_RECORD& wbsr)
{

}

void InputWidget::MouseEventProc(MOUSE_EVENT_RECORD& mer)
{
    
}

void InputWidget::FocusEventProc(FOCUS_EVENT_RECORD& fer)
{

}

void InputWidget::MenuEventProc(MENU_EVENT_RECORD& mer)
{

}
