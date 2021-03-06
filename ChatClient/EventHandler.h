#pragma once

#include <Windows.h>
#include <sstream>
#include <queue>
#include <iostream>
#include <mutex>

class EventHandler
{
public:
	EventHandler() = delete;
	virtual ~EventHandler() = default;
	
public:
	EventHandler(const HANDLE& eventSource, 
				 const HANDLE& outputEventSource);

	EventHandler(const EventHandler& other) = delete;
	EventHandler& operator=(const EventHandler& other) = delete;

public:
	void CatchEvent(const DWORD inputBufferSize = 12);
	void ProcessEvent();

protected:
	std::queue<INPUT_RECORD> m_eventQueue;
	std::mutex m_queueAccess;
	//HANDLE m_newEventFlag = INVALID_HANDLE_VALUE; DEPRICATED

protected:
	HANDLE m_eventSource = INVALID_HANDLE_VALUE;
	HANDLE m_outputEventSource = INVALID_HANDLE_VALUE;

protected:
	virtual void EventProc(INPUT_RECORD& inputEvent);
	virtual void KeyEventProc(KEY_EVENT_RECORD& ker);
	virtual void ResizeEventProc(WINDOW_BUFFER_SIZE_RECORD& wbsr);
	virtual void MouseEventProc(MOUSE_EVENT_RECORD& mer);
	virtual void FocusEventProc(FOCUS_EVENT_RECORD& fer);
	virtual void MenuEventProc(MENU_EVENT_RECORD& mer);
};