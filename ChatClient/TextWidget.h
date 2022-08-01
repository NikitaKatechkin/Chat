#pragma once

#include "Widget.h"

class TextWidget final : public Widget
{
public:
	TextWidget(const std::wstring& textToPrint = L"",
			   const COORD& renderStartPoint = COORD{ 0, 0 },
			   const COORD& widgetSize = COORD{ 80, 30 },
			   const bool drawBorders = true);

	TextWidget(const Frame& widgetFrame,
			   const COORD& renderStartPoint = COORD{ 0, 0 },
			   const COORD& widgetSize = COORD{ 80, 30 });

public:
	void PrintText(const std::wstring& text);

private:
	virtual BOOL Update() override;

private:
	COORD GetTextTypePosition() const;
	void SetTextTypePosition(const COORD& newPos);

	COORD m_textTypePosition = COORD{ 0, 0 };

private:
	void PrintCharacterToFrame(const wchar_t& character);
	void DeleteCharacterFromFrame();

};