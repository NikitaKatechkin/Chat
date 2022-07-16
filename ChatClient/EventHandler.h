#pragma once

#include "EventCollector.h"
#include "EventProcessor.h"
#include <sstream>

class EventHandler
{
public:
	EventHandler(const HANDLE& eventSource);
	virtual ~EventHandler();

public:
	void CatchEvent(const DWORD inputBufferSize = 12);
	void ProcessEvent();

protected:
	std::vector<INPUT_RECORD> m_eventQueue;
	std::mutex m_queueAccess;
	HANDLE m_newEventFlag = INVALID_HANDLE_VALUE;
	HANDLE m_eventSource = INVALID_HANDLE_VALUE;

protected:
	virtual void EventProc(INPUT_RECORD& inputEvent);
	virtual void KeyEventProc(KEY_EVENT_RECORD& ker);
	virtual void ResizeEventProc(WINDOW_BUFFER_SIZE_RECORD& wbsr);
	virtual void MouseEventProc(MOUSE_EVENT_RECORD& mer);
	virtual void FocusEventProc(FOCUS_EVENT_RECORD& fer);
	virtual void MenuEventProc(MENU_EVENT_RECORD& mer);
};