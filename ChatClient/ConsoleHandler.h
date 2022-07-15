#pragma once

#include <windows.h>
#include <sstream>

class ConsoleHandler
{
public:
	ConsoleHandler(const bool makeActive = false);
	~ConsoleHandler();

	HANDLE GetWinAPIConsoleHandler();

	COORD GetConsoleSize();
	BOOL MakeConsoleActive();

	BOOL Write(const wchar_t* bufferToWrite, 
			const COORD& bufferSize, 
			const COORD& insertionTopLeft,
			DWORD& bytesWritten);
	
	void ClearDisplay();
	BOOL UpdateScreenBuffer(const wchar_t* buffer, const COORD& bufferSize);
	BOOL DisplayBuffer();
private:
	HANDLE m_console = INVALID_HANDLE_VALUE;
	wchar_t* m_consoleScreen = nullptr;
	CONSOLE_SCREEN_BUFFER_INFO m_consoleBufferInfo;
};