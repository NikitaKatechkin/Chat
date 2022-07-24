#pragma once

#include "Widget.h"
#include "ConsoleEventHandler.h"

class InputWidget final : public Widget
{
public:
	InputWidget(const COORD& renderStartPoint = COORD{ 0, 0 },
				const COORD& widgetSize = COORD{ 80, 30 });
	virtual ~InputWidget();

private:
	ConsoleEventHandler* m_eventHandler;
	std::thread m_eventHandlingThread;
	bool m_isEventHandlingOn;

	void StartEventHandling();
	void StopEventHandling();
private:
	virtual BOOL Update() override;
};