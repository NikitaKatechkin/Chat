#include "ConsoleHandler.h"

ConsoleHandler::ConsoleHandler(const COORD& consoleSize):
    m_consoleOutput(GetStdHandle(STD_OUTPUT_HANDLE)),
    m_consoleInput(GetStdHandle(STD_INPUT_HANDLE))
{
    if (m_consoleOutput == INVALID_HANDLE_VALUE)
    {
        std::stringstream errorMessage;

        errorMessage << "Failed to CreateConsoleScreenBuffer with GLE = ";
        errorMessage << GetLastError() << "." << std::endl;

        throw std::exception(errorMessage.str().c_str());
    }

    if (m_consoleInput == INVALID_HANDLE_VALUE)
    {
        std::stringstream errorMessage;

        errorMessage << "Failed to CreateConsoleScreenBuffer with GLE = ";
        errorMessage << GetLastError() << "." << std::endl;

        throw std::exception(errorMessage.str().c_str());
    }

    if (GetConsoleScreenBufferInfo(m_consoleOutput, &m_consoleBufferInfo) == FALSE)
    {
        std::stringstream errorMessage;

        errorMessage << "Failed to GetConsoleScreenBufferInfo with GLE = ";
        errorMessage << GetLastError() << "." << std::endl;

        throw std::exception(errorMessage.str().c_str());
    }

    if (SetConsoleSize(consoleSize) == FALSE)
    {
        std::stringstream errorMessage;

        errorMessage << "Failed to SetConsoleSize with GLE = ";
        errorMessage << GetLastError() << "." << std::endl;

        throw std::exception(errorMessage.str().c_str());
    }
}

ConsoleHandler::~ConsoleHandler()
{

}

HANDLE ConsoleHandler::GetWinAPIConsoleInputHandler()
{
    return m_consoleInput;
}

HANDLE ConsoleHandler::GetWinAPIConsoleOutputHandler()
{
    return m_consoleOutput;
}

BOOL ConsoleHandler::SetConsoleSize(const COORD& consoleSize)
{
    auto oldMode = m_consoleBufferInfo;

    SMALL_RECT windowSize = { 0, 0, consoleSize.X - 1, consoleSize.Y - 1 };
    BOOL result = SetConsoleWindowInfo(m_consoleOutput,
        TRUE,
        &windowSize);

    if (result == TRUE)
    {
        result = SetConsoleScreenBufferSize(m_consoleOutput,
            consoleSize);

        if (result == TRUE)
        {
            result = Update();
        }
        else
        {
            SetConsoleWindowInfo(m_consoleOutput,
                TRUE,
                &oldMode.srWindow);
            SetConsoleScreenBufferSize(m_consoleOutput,
                oldMode.dwSize);
        }
    }
    else
    {
        SetConsoleWindowInfo(m_consoleOutput,
            TRUE,
            &oldMode.srWindow);
    }

    return result;
}

COORD ConsoleHandler::GetConsoleSize()
{
    return m_consoleBufferInfo.dwSize;
}

void ConsoleHandler::Clear()
{
    for (auto& widget : m_widgetList)
    {
        widget.ClearWidget();
    }
}

BOOL ConsoleHandler::Draw(const wchar_t* buffer, const COORD& bufferSize)
{
    return FALSE;
}

BOOL ConsoleHandler::Display()
{
    BOOL result = TRUE;

    for (auto& widget : m_widgetList)
    {
        result = widget.DisplayWidget();
        if (result == FALSE)
        {
            break;
        }
    }

    return result;
}

BOOL ConsoleHandler::Update()
{
    BOOL result = GetConsoleScreenBufferInfo(m_consoleOutput, &m_consoleBufferInfo);

    if (result == TRUE)
    {
        for (auto& widget : m_widgetList)
        {
            widget.Update();
        }
        
        Clear();
    }

    return result;
}

BOOL ConsoleHandler::AddWidget(const Widget& widget)
{
    COORD widgetSize = widget.GetWidgetSize();
    COORD startRenderPoint = widget.GetRenderStartPoint();

    BOOL result = (startRenderPoint.Y + widgetSize.Y <= m_consoleBufferInfo.dwSize.Y);

    if ((m_widgetList.empty() == false) && (result == TRUE))
    {
        auto lastWidget = m_widgetList.back();

        COORD lastWidgetSize = lastWidget.GetWidgetSize();
        COORD lastStartRenderPoint = lastWidget.GetRenderStartPoint();

        result = (startRenderPoint.Y >= lastStartRenderPoint.Y + lastWidgetSize.Y);
    }

    if (result == TRUE)
    {
        m_widgetList.push_back(widget);
    }

    return result;
}

/**
BOOL ConsoleHandler::Write(const wchar_t* bufferToWrite,
                           const COORD& bufferSize,
                           const COORD& insertionTopLeft,
                           DWORD& bytesWritten)
{
    if ((m_consoleBufferInfo.dwSize.X < bufferSize.X) ||
        (m_consoleBufferInfo.dwSize.Y < bufferSize.Y))
    {
        return FALSE;
    }

    BOOL result = WriteConsoleOutputCharacter(m_consoleOutput,
                                              bufferToWrite,
                                              bufferSize.X * bufferSize.Y,
                                              insertionTopLeft,
                                              &bytesWritten);

    return result;
}
**/