#include "TextWidget.h"

TextWidget::TextWidget(const COORD& renderStartPoint, 
					   const COORD& widgetSize, 
					   const bool drawBorders):
	Widget(renderStartPoint, widgetSize, drawBorders)
{
}

TextWidget::TextWidget(const Frame& widgetFrame, 
					   const COORD& renderStartPoint, 
					   const COORD& widgetSize):
	Widget(widgetFrame, renderStartPoint, widgetSize)
{
}

BOOL TextWidget::Update()
{
	return TRUE;
}
