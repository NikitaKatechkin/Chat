#include "Frame.h"

Frame::Frame(const COORD& frameSize):
	m_size(frameSize), 
	m_frame(new wchar_t[frameSize.X * frameSize.Y]), 
	m_frameCharLength(frameSize.X* frameSize.Y)
{
	ClearFrame();
}

Frame::Frame(const Frame& other):
	Frame(other.m_size)
{
	memcpy_s(m_frame, m_frameCharLength * sizeof(wchar_t), 
			 other.m_frame, m_frameCharLength * sizeof(wchar_t));
}

Frame& Frame::operator=(const Frame& other)
{
	if (this == &other)
	{
		return *this;
	}

	if (this->m_frame != nullptr)
	{
		delete[] this->m_frame;
	}

	this->m_size = other.m_size;
	this->m_frameCharLength = other.m_frameCharLength;
	this->m_frame = new wchar_t[m_frameCharLength];

	memcpy_s(m_frame, m_frameCharLength * sizeof(wchar_t),
			 other.m_frame, m_frameCharLength * sizeof(wchar_t));

	return *this;
}

Frame::~Frame()
{
	delete[] m_frame;
}

COORD Frame::GetFrameSize()
{
	return m_size;
}

wchar_t* Frame::GetFrameBuffer()
{
	/**
	wchar_t* bufferToWrite = new wchar_t[m_size.X * m_size.Y];

	int result = memcpy_s(bufferToWrite, 
						  m_size.X * m_size.Y * sizeof(wchar_t),
						  m_frame,
						  m_size.X * m_size.Y * sizeof(wchar_t));

	if (result != 0)
	{
		delete[] bufferToWrite;
		bufferToWrite = nullptr;
	}

	return bufferToWrite;
	**/

	return m_frame;
}

DWORD Frame::GetFrameCharLength()
{
	return m_frameCharLength;
}

void Frame::SetFrameSize(const COORD& frameSize)
{
	m_size = frameSize;
	m_frameCharLength = m_size.X * m_size.Y;

	if (m_frame != nullptr)
	{
		delete[] m_frame;
		m_frame = nullptr;
	}

	m_frame = new wchar_t[m_frameCharLength];
	ClearFrame();
}

void Frame::PasteShape(const wchar_t* shape, const COORD& shapeSize, const COORD& topLeftInsertPoint)
{
	COORD startPoint;

	startPoint.X = (topLeftInsertPoint.X > m_size.X) ? 
												(m_size.X - 1) : topLeftInsertPoint.X;
	startPoint.Y = (topLeftInsertPoint.Y > m_size.Y) ?
												(m_size.Y - 1) : topLeftInsertPoint.Y;

	COORD endPoint;

	endPoint.X = ((startPoint.X + shapeSize.X) > m_size.X) ? 
										(m_size.X - 1) : (startPoint.X + shapeSize.X);
	endPoint.Y = ((startPoint.Y + shapeSize.Y) > m_size.Y) ?
										(m_size.Y - 1) : (startPoint.Y + shapeSize.Y);

	COORD insertionSize{ endPoint.X - startPoint.X, 
						 endPoint.Y - startPoint.Y };

	for (short row = 0; row < insertionSize.Y; row++)
	{
		for (short column = 0; column < insertionSize.X; column++)
		{
			m_frame[((row + startPoint.Y) * m_size.X) + (column + startPoint.X)] 
				= shape[row * shapeSize.X + column];
			}
	}
}

void Frame::ClearFrame()
{
	std::fill_n(m_frame, m_frameCharLength, L' ');
}
