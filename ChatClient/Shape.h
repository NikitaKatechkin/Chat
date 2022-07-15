#pragma once

#include <Windows.h>
#include <algorithm>

class Shape
{
public:
	Shape(const COORD& size);
	virtual ~Shape();

	void Clear() const;
	wchar_t* GetBuffer();
	COORD GetSize() const;
protected:
	wchar_t* m_buffer = nullptr;
	COORD m_size;
};