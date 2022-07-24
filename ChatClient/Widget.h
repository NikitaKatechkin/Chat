#pragma once

#include "Frame.h"

#include <Windows.h>
#include <sstream>
#include <iostream>
#include <thread>

class Widget
{
public:
	Widget(const COORD& renderStartPoint = COORD{0, 0}, 
		   const COORD& widgetSize = COORD{80, 30});
	virtual ~Widget() = default;

	virtual HANDLE GetWinAPIConsoleInputHandler();
	virtual HANDLE GetWinAPIConsoleOutputHandler();

	virtual COORD GetRenderStartPoint();
	virtual void SetRenderStartPoint(const COORD& renderStartPoint);

	virtual void SetWidgetSize(const COORD& widgetSize);
	virtual COORD GetWidgetSize();

	virtual void ClearWidget();
	virtual BOOL DrawWidget(const wchar_t* buffer,
					const COORD& bufferSize);
	virtual BOOL DisplayWidget();

	virtual BOOL Update();

protected:
	HANDLE m_consoleInput = INVALID_HANDLE_VALUE;
	HANDLE m_consoleOutput = INVALID_HANDLE_VALUE;

protected:
	Frame m_widgetFrame;
	COORD m_renderStartPoint;
};