#include "ConsoleHandler.h"

ConsoleHandler::ConsoleHandler(const COORD& consoleSize):
    m_consoleScreen(consoleSize), 
    m_consoleScreenLength(consoleSize.X * consoleSize.Y)
{
    m_consoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);

    if (m_consoleOutput == INVALID_HANDLE_VALUE)
    {
        std::stringstream errorMessage;

        errorMessage << "Failed to CreateConsoleScreenBuffer with GLE = ";
        errorMessage << GetLastError() << "." << std::endl;

        throw std::exception(errorMessage.str().c_str());
    }

    m_consoleInput = GetStdHandle(STD_INPUT_HANDLE);

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

    m_eventHandler = new ConsoleEventHandler(m_consoleInput, m_consoleOutput, &m_consoleScreen);
    m_eventHandlingThread = std::thread(&ConsoleHandler::StartEventHandling, this);

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
    //CloseHandle(m_consoleOutput);
    //CloseHandle(m_consoleInput);

    StopEventHandling();
    m_eventHandlingThread.join();

    delete m_eventHandler;
}

HANDLE ConsoleHandler::GetWinAPIConsoleInputHandler()
{
    return m_consoleInput;
}

HANDLE ConsoleHandler::GetWinAPIConsoleOutputHandler()
{
    return m_consoleOutput;
}

COORD ConsoleHandler::GetConsoleSize()
{
    return m_consoleBufferInfo.dwSize;
}

BOOL ConsoleHandler::MakeConsoleActive()
{
    return SetConsoleActiveScreenBuffer(m_consoleOutput);
}

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
                                              m_consoleScreenLength,
                                              insertionTopLeft,
                                              &bytesWritten);

    return result;
}

void ConsoleHandler::ClearDisplay()
{
    m_consoleScreen.ClearFrame();
}

BOOL ConsoleHandler::UpdateDisplay(const wchar_t* buffer, const COORD& bufferSize)
{
    size_t selfScreenBufferBytesSize =
        m_consoleScreenLength * sizeof(wchar_t);
    size_t newScreenBufferBytesSize =
        bufferSize.X * bufferSize.Y * sizeof(wchar_t);

    if (selfScreenBufferBytesSize != newScreenBufferBytesSize)
    {
        return FALSE;
    }

    int result = memcpy_s(m_consoleScreen.GetFrameBuffer(),
                          selfScreenBufferBytesSize,
                          buffer, 
                          newScreenBufferBytesSize);

    return (result == 0) ? TRUE : FALSE;
}

BOOL ConsoleHandler::DrawDisplay()
{
    DWORD bytesWritten = 0;

    return Write(m_consoleScreen.GetFrameBuffer(), m_consoleBufferInfo.dwSize, {0, 0}, bytesWritten);
}

BOOL ConsoleHandler::Update()
{
    BOOL result = GetConsoleScreenBufferInfo(m_consoleOutput, &m_consoleBufferInfo);

    if (result == TRUE)
    {
        //delete[] m_consoleScreen;
        //m_consoleScreen = new wchar_t[m_consoleBufferInfo.dwSize.X * m_consoleBufferInfo.dwSize.Y];

        m_consoleScreen = Frame(m_consoleBufferInfo.dwSize);
        m_consoleScreenLength = m_consoleBufferInfo.dwSize.X * m_consoleBufferInfo.dwSize.Y;

        m_eventHandler->SetConsoleFrame(&m_consoleScreen);

        ClearDisplay();
    }

    return result;
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

void ConsoleHandler::StartEventHandling()
{
    m_isEventHandlingOn = true;

    while (m_isEventHandlingOn == true)
    {
        m_eventHandler->CatchEvent();
        m_eventHandler->ProcessEvent();
    }
}

void ConsoleHandler::StopEventHandling()
{
    m_isEventHandlingOn = false;
}
