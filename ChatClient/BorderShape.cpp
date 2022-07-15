#include "BorderShape.h"

BorderShape::BorderShape(const COORD& size):
	Shape(size)
{
	CreateWcharBorder();
}

BorderShape::BorderShape(const BorderShape& other):
    BorderShape(other.GetSize())
{
}

BorderShape& BorderShape::operator=(const BorderShape& other)
{
    if (this == &other)
    {
        return *this;
    }

    delete[] this->m_buffer;

    this->m_size = other.m_size;
    this->m_buffer = new wchar_t[m_size.X * m_size.Y];
    
    memcpy_s(this->m_buffer, m_size.X * m_size.Y * sizeof(wchar_t),
             other.m_buffer, other.m_size.X * other.m_size.Y * sizeof(wchar_t));

    return *this;
}

void BorderShape::CreateWcharBorder()
{
    std::fill_n(m_buffer, m_size.X * m_size.Y, L' ');

    std::fill_n(m_buffer, m_size.X, L'═');
    m_buffer[0] = L'╔';
    m_buffer[m_size.X - 1] = L'╗';

    std::fill_n(&m_buffer[(m_size.Y - 1) * m_size.X], m_size.X, L'═');
    m_buffer[(m_size.Y - 1) * m_size.X] = L'╚';
    m_buffer[m_size.Y * m_size.X - 1] = L'╝';

    for (short row = 1; row < (m_size.Y - 1); row++)
    {
        m_buffer[row * m_size.X] = L'║';
        m_buffer[(row + 1) * m_size.X - 1] = L'║';
    }
}
