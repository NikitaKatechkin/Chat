#pragma once

#include "ConsoleEventHandler.h"
#include "Frame.h"

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

	BOOL SetConsoleSize(const COORD& consoleSize);
	COORD GetConsoleSize();

	BOOL MakeConsoleActive();

	void ClearDisplay();
	BOOL UpdateDisplay(const wchar_t* buffer, const COORD& bufferSize);
	BOOL DrawDisplay();

	BOOL Update();

private:
	HANDLE m_consoleOutput = INVALID_HANDLE_VALUE;
	HANDLE m_consoleInput = INVALID_HANDLE_VALUE;

	//wchar_t* m_consoleScreen = nullptr;
	Frame m_consoleScreen;
	CONSOLE_SCREEN_BUFFER_INFO m_consoleBufferInfo;

private:
	ConsoleEventHandler m_eventHandler;
	bool m_isEventHandlingOn = false;
	std::thread m_eventHandlingThread;

	void StartEventHandling();
	void StopEventHandling();

private:
	BOOL Write(const wchar_t* bufferToWrite,
			   const COORD& bufferSize,
			   const COORD& insertionTopLeft,
			   DWORD& bytesWritten);

};