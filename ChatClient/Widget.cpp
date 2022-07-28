#include "Widget.h"

Widget::Widget(const COORD& renderStartPoint, 
               const COORD& widgetSize, 
               const bool drawBorders):
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

    if (drawBorders == true)
    {
        std::shared_ptr<BorderShape> border = std::make_shared<BorderShape>(
                                                    m_widgetFrame.GetFrameSize());

        DrawWidget(border->GetBuffer(), border->GetSize());
        DisplayWidget();
    }
}

Widget::Widget(const Frame& widgetFrame, 
               const COORD& renderStartPoint, 
               const COORD& widgetSize):
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

    COORD ownWidgetFrameSize = m_widgetFrame.GetFrameSize();
    COORD newWidgetFrameSize = widgetFrame.GetFrameSize();

    if ((ownWidgetFrameSize.X == newWidgetFrameSize.X) &&
        (ownWidgetFrameSize.Y == newWidgetFrameSize.Y))
    {
        DrawWidget(widgetFrame.GetFrameBuffer(), widgetFrame.GetFrameSize());
        DisplayWidget();
    }
}

/**
Widget::Widget(const Widget& other):
    m_widgetFrame(other.m_widgetFrame)
{
    m_consoleInput = other.m_consoleInput;
    m_consoleOutput = other.m_consoleOutput;
    m_renderStartPoint = other.m_renderStartPoint;
}
**/

HANDLE Widget::GetWinAPIConsoleInputHandler() const
{
	return m_consoleInput;
}

HANDLE Widget::GetWinAPIConsoleOutputHandler() const
{
	return m_consoleOutput;
}

COORD Widget::GetRenderStartPoint() const
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

COORD Widget::GetWidgetSize() const
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

BOOL Widget::DisplayWidget() const
{
    DWORD bytesWritten = 0;
    
    BOOL result = WriteConsoleOutputCharacter(m_consoleOutput,
                                              m_widgetFrame.GetFrameBuffer(),
                                              m_widgetFrame.GetFrameCharLength(),
                                              m_renderStartPoint,
                                              &bytesWritten);

    return result;
}
