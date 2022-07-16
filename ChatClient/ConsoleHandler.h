#pragma once

#include <windows.h>
#include <sstream>
#include <iostream>

class ConsoleHandler
{
public:
	//ConsoleHandler(const bool makeActive);
	ConsoleHandler(const COORD& consoleSize = COORD {80, 30});
	~ConsoleHandler();

	HANDLE GetWinAPIConsoleInputHandler();
	HANDLE GetWinAPIConsoleOutputHandler();

	COORD GetConsoleSize();
	BOOL MakeConsoleActive();

	BOOL Write(const wchar_t* bufferToWrite, 
			const COORD& bufferSize, 
			const COORD& insertionTopLeft,
			DWORD& bytesWritten);
	
	void ClearDisplay();
	BOOL UpdateDisplay(const wchar_t* buffer, const COORD& bufferSize);
	BOOL DrawDisplay();

	BOOL Update();
	BOOL SetConsoleSize(const COORD& consoleSize);
private:
	HANDLE m_consoleOutput = INVALID_HANDLE_VALUE;
	HANDLE m_consoleInput = INVALID_HANDLE_VALUE;

	wchar_t* m_consoleScreen = nullptr;
	CONSOLE_SCREEN_BUFFER_INFO m_consoleBufferInfo;
};