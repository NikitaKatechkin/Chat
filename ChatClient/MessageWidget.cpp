#include "MessageWidget.h"

MessageWidget::MessageWidget(const COORD& renderStartPoint, 
							 const COORD& widgetSize, 
							 const bool drawBorders):
	Widget(renderStartPoint, widgetSize, drawBorders)
{
    COORD startTextTypeposition = COORD{ 1, 1 };
    SetTextTypePosition(startTextTypeposition);
}

MessageWidget::MessageWidget(const Frame& widgetFrame, 
							 const COORD& renderStartPoint, 
							 const COORD& widgetSize):
	Widget(widgetFrame, renderStartPoint, widgetSize)
{
    COORD startTextTypeposition = COORD{ 1, 1 };
    SetTextTypePosition(startTextTypeposition);
}

BOOL MessageWidget::Update()
{
	return TRUE;
}

COORD MessageWidget::GetTextTypePosition() const
{
    return m_textTypePosition;
}

void MessageWidget::SetTextTypePosition(const COORD& newPos)
{
    m_textTypePosition = newPos;
}

void MessageWidget::MoveOnNextLine()
{
    m_textTypePosition = COORD{ 1, 
                                static_cast<short>(m_textTypePosition.Y + 1) };
}

void MessageWidget::PrintCharacterToFrame(const wchar_t& character)
{
    COORD cursorPos = GetTextTypePosition();
    COORD localFrameCursorPos = cursorPos;
    COORD frameSize = m_widgetFrame.GetFrameSize();

    if ((localFrameCursorPos.X < frameSize.X - 1) &&
        (localFrameCursorPos.X > 0) &&
        (localFrameCursorPos.Y < frameSize.Y - 1) &&
        (localFrameCursorPos.Y > 0))
    {
        // Character can be printed
        m_widgetFrame.PasteShape(&character, COORD{ 1, 1 }, localFrameCursorPos);

        COORD offset{ 1, 0 };

        if (localFrameCursorPos.X == frameSize.X - 2)
        {
            //Cursor is right before border
            if (localFrameCursorPos.Y == frameSize.Y - 2)
            {
                //Cursor is in right bottom corner
                offset = COORD{ 0, 0 };
            }
            else
            {
                offset = COORD{ static_cast<short>(1 - localFrameCursorPos.X), 1 };
            }
        }

        SetTextTypePosition(COORD{ static_cast<short>(cursorPos.X + offset.X),
                                   static_cast<short>(cursorPos.Y + offset.Y) });
    }

    DisplayWidget();
}

void MessageWidget::DeleteCharacterFromFrame()
{
    COORD cursorPos = GetTextTypePosition();
    COORD localFrameCursorPos = cursorPos;
    COORD frameSize = m_widgetFrame.GetFrameSize();

    if ((localFrameCursorPos.X < frameSize.X - 1) &&
        (localFrameCursorPos.X > 0) &&
        (localFrameCursorPos.Y < frameSize.Y - 1) &&
        (localFrameCursorPos.Y > 0))
    {
        // Character can be printed
        m_widgetFrame.PasteShape(L" ", COORD{ 1, 1 }, localFrameCursorPos);

        COORD offset{ -1, 0 };

        if (localFrameCursorPos.X == 1)
        {
            //Cursor is right after border
            if (localFrameCursorPos.Y == 1)
            {
                //Cursor is in left top corner
                offset = COORD{ 0, 0 };
            }
            else
            {
                offset = COORD{ static_cast<short>(frameSize.X - 3), -1 };
            }
        }

        SetTextTypePosition(COORD{ static_cast<short>(cursorPos.X + offset.X),
                                   static_cast<short>(cursorPos.Y + offset.Y) });
    }
}

void MessageWidget::PrintText(const std::wstring& text)
{
    for (size_t charIndex = 0; charIndex < text.size(); charIndex++)
    {
        PrintCharacterToFrame(text[charIndex]);
    }
}

void MessageWidget::MoveBufferOneLineAbove()
{
    COORD frameSize = m_widgetFrame.GetFrameSize();
    wchar_t* frameBuffer = m_widgetFrame.GetFrameBuffer();

    wchar_t* nextLineBuffer = new wchar_t[frameSize.X];
    
    for (size_t rowIndex = 1; rowIndex < frameSize.Y - 2; rowIndex++)
    {
        size_t copyLineOffset = (rowIndex + 1) * frameSize.X;
        for (size_t charIndex = copyLineOffset; charIndex < copyLineOffset + frameSize.X; charIndex++)
        {
            nextLineBuffer[charIndex - copyLineOffset] = frameBuffer[charIndex];
        }

        m_widgetFrame.PasteShape(nextLineBuffer, 
                                 COORD {frameSize.X, 1}, 
                                 COORD {0, static_cast<short>(rowIndex)});
    }

    {
        // Clear last text row

        std::fill_n(nextLineBuffer, frameSize.X, L' ');
        nextLineBuffer[0] = frameBuffer[frameSize.X];
        nextLineBuffer[frameSize.X - 1] = frameBuffer[2 * frameSize.X - 1];

        m_widgetFrame.PasteShape(nextLineBuffer,
                                 COORD{ frameSize.X, 1 },
                                 COORD{ 0, static_cast<short>(frameSize.Y - 2) });
    }

    SetTextTypePosition(COORD{ m_textTypePosition.X,
                               static_cast<short>(m_textTypePosition.Y - 1) });

    delete[] nextLineBuffer;

    DisplayWidget();
}

void MessageWidget::PrintMessage(const std::wstring& message)
{
    {
        //Free space for new message to be printed

        COORD frameSize = m_widgetFrame.GetFrameSize();

        size_t linesRequired = message.size() / frameSize.X + 1;
        size_t availableLines = (frameSize.Y - 2) - m_textTypePosition.Y + 1;

        if (availableLines < linesRequired)
        {
            for (size_t i = 0; i < (linesRequired - availableLines); i++)
            {
                MoveBufferOneLineAbove();
            }
        }
    }

    PrintText(message);

    MoveOnNextLine();

    DisplayWidget();
}
