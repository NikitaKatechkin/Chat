#pragma once

#include "EventHandler.h"
#include "Frame.h"

class ConsoleEventHandler final : public EventHandler
{
public:
	ConsoleEventHandler() = delete;
	virtual ~ConsoleEventHandler() = default;

public:
	ConsoleEventHandler(const HANDLE& eventSource, 
						const HANDLE& outputEventSource, 
						Frame* consoleFrame,
						const COORD& frameStartPos = {0, 0});
	ConsoleEventHandler(const ConsoleEventHandler& other) = delete;
	ConsoleEventHandler& operator=(const ConsoleEventHandler& other) = delete;

protected:
	virtual void EventProc(INPUT_RECORD& inputEvent) override;
	virtual void KeyEventProc(KEY_EVENT_RECORD& ker) override;
	virtual void ResizeEventProc(WINDOW_BUFFER_SIZE_RECORD& wbsr) override;
	virtual void MouseEventProc(MOUSE_EVENT_RECORD& mer) override;
	virtual void FocusEventProc(FOCUS_EVENT_RECORD& fer) override;
	virtual void MenuEventProc(MENU_EVENT_RECORD& mer) override;

protected:
	COORD GetCursorPosition();
	void SetCursorPosition(const COORD& newPos);

protected:
	Frame* m_consoleFrame = nullptr;
	COORD m_frameStartPos;

	BOOL WriteToOutputHandle();
public:
	void SetConsoleFrame(Frame* consoleFrame, 
						 const COORD& frameStartPos = { 0, 0 });
};