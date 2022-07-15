#include "EventProcToolbox.h"


void EventProcToolbox::KeyEventProc(KEY_EVENT_RECORD& ker)
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

void EventProcToolbox::ResizeEventProc(WINDOW_BUFFER_SIZE_RECORD& wbsr)
{
    std::cout << "Resize event occured." << std::endl;
    std::cout << "Columns = " << wbsr.dwSize.X << "; ";
    std::cout << "Rows = " << wbsr.dwSize.Y << ";" << std::endl;
}

void EventProcToolbox::MouseEventProc(MOUSE_EVENT_RECORD& mer)
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

void EventProcToolbox::FocusEventProc(FOCUS_EVENT_RECORD& fer)
{
    std::cout << "Focus: switched." << std::endl;
}

void EventProcToolbox::EventProc(INPUT_RECORD& inputEvent,
                                 void (*KeyEventProc)(KEY_EVENT_RECORD&),
                                 void (*ResizeEventProc)(WINDOW_BUFFER_SIZE_RECORD&),
                                 void (*MouseEventProc)(MOUSE_EVENT_RECORD&),
                                 void (*FocusEventProc)(FOCUS_EVENT_RECORD&))
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
        break;
    default:
        std::cout << "Unknown event type occured." << std::endl;
        break;
    }
}
