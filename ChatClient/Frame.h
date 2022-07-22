#pragma once

#include <Windows.h>
#include <algorithm>

class Frame
{
public:
	Frame(const COORD& frameSize);
	~Frame();

	Frame(const Frame& other);
	Frame& operator=(const Frame& other);
public:
	COORD GetFrameSize();
	wchar_t* GetFrameBuffer();
	DWORD GetFrameCharLength();

	void SetFrameSize(const COORD& frameSize);

	void PasteShape(const wchar_t* shape, 
					const COORD& shapeSize, 
					const COORD& topLeftInsertPoint);

	void ClearFrame();
private:
	wchar_t* m_frame;
	DWORD m_frameCharLength;
	COORD m_size;
};
