#pragma once

#include "EventHandler.h"

class ConsoleEventHandler final : public EventHandler
{
public:
	ConsoleEventHandler() = default;
	ConsoleEventHandler(const HANDLE& eventSource);
	ConsoleEventHandler(const ConsoleEventHandler& other);
	ConsoleEventHandler& operator=(const ConsoleEventHandler& other);

	~ConsoleEventHandler() = default;
protected:
	virtual void KeyEventProc(KEY_EVENT_RECORD& ker) override;
	virtual void ResizeEventProc(WINDOW_BUFFER_SIZE_RECORD& wbsr) override;
	virtual void MouseEventProc(MOUSE_EVENT_RECORD& mer) override;
	virtual void FocusEventProc(FOCUS_EVENT_RECORD& fer) override;
	virtual void MenuEventProc(MENU_EVENT_RECORD& mer) override;
};