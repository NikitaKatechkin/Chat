#include "TextWidget.h"

TextWidget::TextWidget(const std::wstring& textToPrint,
                       const COORD& renderStartPoint,
					   const COORD& widgetSize, 
					   const bool drawBorders):
	Widget(renderStartPoint, widgetSize, drawBorders)
{
    COORD startTextTypeposition = (drawBorders == true) ? COORD{ 1, 1 } : 
                                                          COORD{ 0, 0 };
    SetTextTypePosition(startTextTypeposition);

    PrintText(textToPrint);

    DisplayWidget();
}

TextWidget::TextWidget(const Frame& widgetFrame, 
					   const COORD& renderStartPoint, 
					   const COORD& widgetSize):
	Widget(widgetFrame, renderStartPoint, widgetSize)
{
    SetTextTypePosition(COORD{ 0, 0 });
}

BOOL TextWidget::Update()
{
	return TRUE;
}

COORD TextWidget::GetTextTypePosition() const
{
	return m_textTypePosition;
}

void TextWidget::SetTextTypePosition(const COORD& newPos)
{
	m_textTypePosition = newPos;
}

void TextWidget::PrintCharacterToFrame(const wchar_t& character)
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
}

void TextWidget::DeleteCharacterFromFrame()
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

void TextWidget::PrintText(const std::wstring& text)
{
    for (size_t charIndex = 0; charIndex < text.size(); charIndex++)
    {
        PrintCharacterToFrame(text[charIndex]);
    }
}
