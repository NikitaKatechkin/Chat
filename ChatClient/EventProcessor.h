#pragma once

#ifndef MOUSE_HWHEELED
#define MOUSE_HWHEELED 0x0008
#endif

#include <windows.h>
#include <vector>
#include <mutex>
#include <thread>
#include <iostream>

class EventProcessor
{
public:
	EventProcessor() = default;
	~EventProcessor();

private:
	std::thread m_thread;
	bool m_isActive = false;

public:
	void Start(std::vector<INPUT_RECORD>& eventQueue, std::mutex& queueAccess, HANDLE& newEventFlag);
	void Stop();

private:
	void ProcessEventQueue(std::vector<INPUT_RECORD>& eventQueue,
						   std::mutex& queueAccess, HANDLE& newEventFlag);

protected:
	virtual void EventProc(INPUT_RECORD& inputEvent);
	virtual void KeyEventProc(KEY_EVENT_RECORD& ker);
	virtual void ResizeEventProc(WINDOW_BUFFER_SIZE_RECORD& wbsr);
	virtual void MouseEventProc(MOUSE_EVENT_RECORD& mer);
	virtual void FocusEventProc(FOCUS_EVENT_RECORD& fer);
	virtual void MenuEventProc(MENU_EVENT_RECORD& mer);
};