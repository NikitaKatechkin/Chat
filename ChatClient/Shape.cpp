#include "Shape.h"

Shape::Shape(const COORD& size):
	m_size(size), m_buffer(new wchar_t[size.X * size.Y])
{
	Clear();
}

Shape::~Shape()
{
	delete[] m_buffer;
}

void Shape::Clear() const
{
	std::fill_n(m_buffer, m_size.X * m_size.Y, L' ');
}

wchar_t* Shape::GetBuffer()
{
	return m_buffer;
}

COORD Shape::GetSize() const
{
	return m_size;
}
