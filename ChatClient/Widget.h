#pragma once

#include "Frame.h"
#include "BorderShape.h"

#include <Windows.h>
#include <sstream>
#include <iostream>
#include <thread>

class Widget
{
public:
	Widget(const COORD& renderStartPoint = COORD{0, 0}, 
		   const COORD& widgetSize = COORD{80, 30},
		   const bool drawBorders = true);

	Widget(const Frame& widgetFrame, 
		   const COORD& renderStartPoint = COORD{ 0, 0 },
		   const COORD& widgetSize = COORD{ 80, 30 });

	Widget(const Widget& other) = delete;

	virtual ~Widget() = default;

	HANDLE GetWinAPIConsoleInputHandler() const;
	HANDLE GetWinAPIConsoleOutputHandler() const;

	COORD GetRenderStartPoint() const;
	void SetRenderStartPoint(const COORD& renderStartPoint);

	void SetWidgetSize(const COORD& widgetSize);
	COORD GetWidgetSize() const;

	void ClearWidget();
	virtual BOOL DrawWidget(const wchar_t* buffer,
							const COORD& bufferSize);
	BOOL DisplayWidget() const;

	virtual BOOL Update() = 0;

protected:
	HANDLE m_consoleInput = INVALID_HANDLE_VALUE;
	HANDLE m_consoleOutput = INVALID_HANDLE_VALUE;

protected:
	Frame m_widgetFrame;
	COORD m_renderStartPoint;
};