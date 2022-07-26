#pragma once

#include <Windows.h>

class Subscriber
{
public:
	virtual void Update(INPUT_RECORD& inputEvent) = 0;
protected:
};