#include "InputWidget.h"

InputWidget::InputWidget(const COORD& renderStartPoint, 
						 const COORD& widgetSize):
	Widget(renderStartPoint, widgetSize), 
	m_eventHandler(new ConsoleEventHandler(m_consoleInput,
										   m_consoleOutput,
										   &m_widgetFrame, 
										   renderStartPoint))
{
	m_eventHandlingThread = std::thread(&InputWidget::StartEventHandling, 
										this);
}

InputWidget::~InputWidget()
{
	StopEventHandling();
	m_eventHandlingThread.join();

	//delete m_eventHandler;
}

void InputWidget::StartEventHandling()
{
	m_isEventHandlingOn = true;

	while (m_isEventHandlingOn == true)
	{
		m_eventHandler->CatchEvent();
		m_eventHandler->ProcessEvent();
	}
}

void InputWidget::StopEventHandling()
{
	m_isEventHandlingOn = false;
}

BOOL InputWidget::Update()
{
	m_eventHandler->SetConsoleFrame(&m_widgetFrame);

	return TRUE;
}
