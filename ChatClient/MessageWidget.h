#pragma once

#include "Widget.h"
#include <memory>

class MessageWidget final : public Widget
{
public:
	MessageWidget(const COORD& renderStartPoint = COORD{ 0, 0 },
				  const COORD& widgetSize = COORD{ 80, 30 },
				  const bool drawBorders = true);

	MessageWidget(const Frame& widgetFrame,
				  const COORD& renderStartPoint = COORD{ 0, 0 },
				  const COORD& widgetSize = COORD{ 80, 30 });
private:
	virtual BOOL Update() override;

private:
	COORD GetTextTypePosition() const;
	void SetTextTypePosition(const COORD& newPos);
	void MoveOnNextLine();

private:
	COORD m_textTypePosition = COORD{ 0, 0 };

private:
	void PrintCharacterToFrame(const wchar_t& character);
	void DeleteCharacterFromFrame();
	void PrintText(const std::wstring& text);

private:

public:
	void MoveBufferOneLineAbove();
	void PrintMessage(const std::wstring& message);
};