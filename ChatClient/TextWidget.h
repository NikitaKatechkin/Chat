#pragma once

#include "Widget.h"

class TextWidget final : public Widget
{
public:
	TextWidget(const COORD& renderStartPoint = COORD{ 0, 0 },
			   const COORD& widgetSize = COORD{ 80, 30 },
			   const bool drawBorders = true);

	TextWidget(const Frame& widgetFrame,
			   const COORD& renderStartPoint = COORD{ 0, 0 },
			   const COORD& widgetSize = COORD{ 80, 30 });
private:
	virtual BOOL Update() override;
};