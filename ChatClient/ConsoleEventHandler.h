#pragma once

#include "EventHandler.h"

class ConsoleEventHandler final : public EventHandler
{
public:
	ConsoleEventHandler() = default;
	virtual ~ConsoleEventHandler() = default;

public:
	ConsoleEventHandler(const HANDLE& eventSource, const HANDLE& outputEventSource, 
						wchar_t* consoleBuffer, const COORD& consoleBufferSize);
	ConsoleEventHandler(const ConsoleEventHandler& other);
	ConsoleEventHandler& operator=(const ConsoleEventHandler& other);

protected:
	virtual void EventProc(INPUT_RECORD& inputEvent) override;
	virtual void KeyEventProc(KEY_EVENT_RECORD& ker) override;
	/**
	virtual void KeyEventProc(KEY_EVENT_RECORD& ker, 
							  wchar_t* bufferToWrite, 
							  const COORD& bufferSize);
	**/
	virtual void ResizeEventProc(WINDOW_BUFFER_SIZE_RECORD& wbsr) override;
	virtual void MouseEventProc(MOUSE_EVENT_RECORD& mer) override;
	virtual void FocusEventProc(FOCUS_EVENT_RECORD& fer) override;
	virtual void MenuEventProc(MENU_EVENT_RECORD& mer) override;

protected:
	COORD GetCursorPosition();
	void SetCursorPosition(const COORD& newPos);

protected:
	wchar_t* m_consoleBuffer = nullptr;
	COORD m_consoleBufferSize = COORD{0, 0};

	BOOL WriteToOutputHandle(wchar_t* bufferToWrite, const COORD& bufferSize);
public:
	void SetConsoleBuffer(wchar_t* buffer);
	void SetConsoleBufferSize(const COORD& bufferSize);
};