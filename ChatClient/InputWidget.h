#pragma once

#include "Widget.h"
#include "Subscriber.h"

#include <memory>

class InputWidget final : public Widget, 
						  public Subscriber
{
public:
	InputWidget(const COORD& renderStartPoint = COORD{ 0, 0 },
				const COORD& widgetSize = COORD{ 80, 30 });
	InputWidget(const InputWidget& other);
	InputWidget(const Widget& other);
	virtual ~InputWidget();

private:
	virtual BOOL Update() override;
	virtual void Update(INPUT_RECORD& inputEvent) override;

private:
	COORD GetCursorPosition() const;
	void SetCursorPosition(const COORD& newPos);

	void PrintCharacterToConsoleFrame(const wchar_t& character);
	void DeleteCharacterFromConsoleFrame();
private:
	virtual void EventProc(INPUT_RECORD& inputEvent);
	virtual void KeyEventProc(KEY_EVENT_RECORD& ker);
	virtual void ResizeEventProc(WINDOW_BUFFER_SIZE_RECORD& wbsr);
	virtual void MouseEventProc(MOUSE_EVENT_RECORD& mer);
	virtual void FocusEventProc(FOCUS_EVENT_RECORD& fer);
	virtual void MenuEventProc(MENU_EVENT_RECORD& mer);

};