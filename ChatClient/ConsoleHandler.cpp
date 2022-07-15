#include "ConsoleHandler.h"

ConsoleHandler::ConsoleHandler(const bool makeActive)
{
	m_console = CreateConsoleScreenBuffer((GENERIC_READ | GENERIC_WRITE),
                                          0,
                                          nullptr,
                                          CONSOLE_TEXTMODE_BUFFER,
                                          nullptr);

    if (m_console == INVALID_HANDLE_VALUE)
    {
        std::stringstream errorMessage;

        errorMessage << "Failed to CreateConsoleScreenBuffer with GLE = ";
        errorMessage << GetLastError() << "." << std::endl;

        throw std::exception(errorMessage.str().c_str());
    }

    if (GetConsoleScreenBufferInfo(m_console, &m_consoleBufferInfo) == FALSE)
    {
        std::stringstream errorMessage;

        errorMessage << "Failed to GetConsoleScreenBufferInfo with GLE = ";
        errorMessage << GetLastError() << "." << std::endl;

        throw std::exception(errorMessage.str().c_str());
    }

    m_consoleScreen = new wchar_t[m_consoleBufferInfo.dwSize.X * m_consoleBufferInfo.dwSize.Y];
    ClearDisplay();

    if (makeActive == true)
    {
        if (MakeConsoleActive() == FALSE)
        {
            std::stringstream errorMessage;

            errorMessage << "Failed to SetConsoleActiveScreenBuffer with GLE = ";
            errorMessage << GetLastError() << "." << std::endl;

            throw std::exception(errorMessage.str().c_str());
        }
    }
}

ConsoleHandler::~ConsoleHandler()
{
    CloseHandle(m_console);
    delete[] m_consoleScreen;
}

HANDLE ConsoleHandler::GetWinAPIConsoleHandler()
{
    return m_console;
}

COORD ConsoleHandler::GetConsoleSize()
{
    return m_consoleBufferInfo.dwSize;
}

BOOL ConsoleHandler::MakeConsoleActive()
{
    return SetConsoleActiveScreenBuffer(m_console);
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

    BOOL result = WriteConsoleOutputCharacter(m_console,
                                              bufferToWrite,
                                              bufferSize.X * bufferSize.Y,
                                              insertionTopLeft,
                                              &bytesWritten);

    return result;
}

void ConsoleHandler::ClearDisplay()
{
    std::fill_n(m_consoleScreen, m_consoleBufferInfo.dwSize.X * m_consoleBufferInfo.dwSize.Y, L' ');
}

BOOL ConsoleHandler::UpdateScreenBuffer(const wchar_t* buffer, const COORD& bufferSize)
{
    if ((m_consoleBufferInfo.dwSize.X * m_consoleBufferInfo.dwSize.Y) != 
        (bufferSize.X * bufferSize.Y))
    {
        return FALSE;
    }

    size_t selfScreenBufferBytesSize = 
        m_consoleBufferInfo.dwSize.X * m_consoleBufferInfo.dwSize.Y * sizeof(wchar_t);
    size_t newScreenBufferBytesSize = 
        bufferSize.X * bufferSize.Y * sizeof(wchar_t);

    int result = memcpy_s(m_consoleScreen, 
                          selfScreenBufferBytesSize,
                          buffer, 
                          newScreenBufferBytesSize);

    return (result == 0) ? TRUE : FALSE;
}

BOOL ConsoleHandler::DisplayBuffer()
{
    DWORD bytesWritten = 0;

    return Write(m_consoleScreen, m_consoleBufferInfo.dwSize, {0, 0}, bytesWritten);
}
