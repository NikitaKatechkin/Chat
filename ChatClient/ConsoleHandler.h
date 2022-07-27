#pragma once

//#include "ConsoleEventHandler.h"
#include "Frame.h"
#include "InputWidget.h"
#include "BorderShape.h"
#include "EventHandler.h"

#include <windows.h>
#include <sstream>
#include <iostream>
#include <thread>
#include <vector>

class ConsoleHandler
{
public:
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
	BOOL AddWidget(Widget* widget);

private:
	HANDLE m_consoleOutput = INVALID_HANDLE_VALUE;
	HANDLE m_consoleInput = INVALID_HANDLE_VALUE;
	CONSOLE_SCREEN_BUFFER_INFO m_consoleBufferInfo;

private:
	std::vector<Widget*> m_widgetList;

private:
	std::unique_ptr<EventHandler> m_eventHandler;
	std::thread m_eventHandlingThread;
	bool m_isEventHandlingOn;

	void StartEventHandling();
	void StopEventHandling();
private:
	static Frame CreateMainScreen(const COORD& frameSize);
	static Frame CreateInfoFrame(const COORD& frameSize);
	static Frame CreateMessageFrame(const COORD& frameSize);
	static Frame CreateInputFrame(const COORD& frameSize);
};