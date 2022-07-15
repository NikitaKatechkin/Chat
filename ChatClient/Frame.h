#pragma once

#include <Windows.h>
#include <algorithm>

class Frame
{
public:
	Frame(const COORD& frameSize);
	~Frame();

	COORD GetFrameSize();
	wchar_t* GetFrameBuffer();

	void PasteShape(const wchar_t* shape, 
					const COORD& shapeSize, 
					const COORD& topLeftInsertPoint);

	void ClearFrame();
private:
	wchar_t* m_frame;
	COORD m_size;
};