#pragma once

#include <windows.h>
#include <cstdio>
#include <iostream>
#include <vector>
#include <memory>
#include <mutex>
#include <thread>

namespace EventProcToolbox
{
	void KeyEventProc(KEY_EVENT_RECORD& ker);
	void ResizeEventProc(WINDOW_BUFFER_SIZE_RECORD& wbsr);
	void MouseEventProc(MOUSE_EVENT_RECORD& mer);
	void FocusEventProc(FOCUS_EVENT_RECORD& fer);
	void EventProc(INPUT_RECORD& inputEvent,
			void (*KeyEventProc)(KEY_EVENT_RECORD&) = EventProcToolbox::KeyEventProc,
			void (*ResizeEventProc)(WINDOW_BUFFER_SIZE_RECORD&) = EventProcToolbox::ResizeEventProc,
			void (*MouseEventProc)(MOUSE_EVENT_RECORD&) = EventProcToolbox::MouseEventProc,
			void (*FocusEventProc)(FOCUS_EVENT_RECORD&) = EventProcToolbox::FocusEventProc);
}