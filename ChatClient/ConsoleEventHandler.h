#pragma once

#include "EventHandler.h"
#include "Frame.h"

#include <algorithm>

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
	COORD GetCursorPosition() const;
	void SetCursorPosition(const COORD& newPos);

	void PrintCharacterToConsoleFrame(const wchar_t& character);
	void DeleteCharacterFromConsoleFrame();
protected:
	Frame* m_consoleFrame = nullptr;
	COORD m_frameStartPos;

	BOOL WriteToOutputHandle() const;
public:
	void SetConsoleFrame(Frame* consoleFrame, 
						 const COORD& frameStartPos = { 0, 0 });
};