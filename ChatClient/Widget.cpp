#include "Widget.h"

Widget::Widget(const COORD& renderStartPoint, const COORD& widgetSize):
	m_renderStartPoint(renderStartPoint),
	m_widgetFrame(widgetSize), 
	m_consoleInput(GetStdHandle(STD_INPUT_HANDLE)),
	m_consoleOutput(GetStdHandle(STD_OUTPUT_HANDLE))
{
    if (m_consoleOutput == INVALID_HANDLE_VALUE)
    {
        std::stringstream errorMessage;

        errorMessage << "Failed to CreateConsoleScreenBuffer with GLE = ";
        errorMessage << GetLastError() << "." << std::endl;

        throw std::exception(errorMessage.str().c_str());
    }

    if (m_consoleInput == INVALID_HANDLE_VALUE)
    {
        std::stringstream errorMessage;

        errorMessage << "Failed to CreateConsoleScreenBuffer with GLE = ";
        errorMessage << GetLastError() << "." << std::endl;

        throw std::exception(errorMessage.str().c_str());
    }
}

HANDLE Widget::GetWinAPIConsoleInputHandler()
{
	return m_consoleInput;
}

HANDLE Widget::GetWinAPIConsoleOutputHandler()
{
	return m_consoleOutput;
}

COORD Widget::GetRenderStartPoint()
{
    return m_renderStartPoint;
}

void Widget::SetRenderStartPoint(const COORD& renderStartPoint)
{
    m_renderStartPoint = renderStartPoint;
}

void Widget::SetWidgetSize(const COORD& widgetSize)
{
    m_widgetFrame.SetFrameSize(widgetSize);

    this->Update();
}

COORD Widget::GetWidgetSize()
{
    return m_widgetFrame.GetFrameSize();
}

void Widget::ClearWidget()
{
    m_widgetFrame.ClearFrame();
}

BOOL Widget::DrawWidget(const wchar_t* buffer, const COORD& bufferSize)
{
    size_t selfScreenBufferBytesSize = m_widgetFrame.GetFrameCharLength() * sizeof(wchar_t);
    size_t newScreenBufferBytesSize = bufferSize.X * bufferSize.Y * sizeof(wchar_t);

    if (selfScreenBufferBytesSize != newScreenBufferBytesSize)
    {
        return FALSE;
    }

    int result = memcpy_s(m_widgetFrame.GetFrameBuffer(),
                          selfScreenBufferBytesSize,
                          buffer,
                          newScreenBufferBytesSize);

    return (result == 0) ? TRUE : FALSE;
}

BOOL Widget::DisplayWidget()
{
    DWORD bytesWritten = 0;
    
    BOOL result = WriteConsoleOutputCharacter(m_consoleOutput,
                                              m_widgetFrame.GetFrameBuffer(),
                                              m_widgetFrame.GetFrameCharLength(),
                                              m_renderStartPoint,
                                              &bytesWritten);

    return result;
}

BOOL Widget::Update()
{
    return TRUE;
}
