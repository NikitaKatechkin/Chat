#pragma once

#include "ConsoleEventHandler.h"
#include "Frame.h"
#include "InputWidget.h"

#include <windows.h>
#include <sstream>
#include <iostream>
#include <thread>

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

	void Clear();
	BOOL Draw(const wchar_t* buffer, const COORD& bufferSize);
	BOOL Display();

	BOOL Update();

public:
	BOOL AddWidget(const Widget& widget);
private:
	HANDLE m_consoleOutput = INVALID_HANDLE_VALUE;
	HANDLE m_consoleInput = INVALID_HANDLE_VALUE;
	CONSOLE_SCREEN_BUFFER_INFO m_consoleBufferInfo;

private:
	std::vector<Widget> m_widgetList;
private:
	/**
	BOOL Write(const wchar_t* bufferToWrite,
			   const COORD& bufferSize,
			   const COORD& insertionTopLeft,
			   DWORD& bytesWritten);
	**/

};