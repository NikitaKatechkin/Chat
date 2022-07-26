#pragma once

#include "Subscriber.h"

#include <Windows.h>
#include <vector>
#include <algorithm>

class Publisher
{
public:
	virtual bool Observe(Subscriber* subscriberToObserve);
	virtual bool Detach(Subscriber* subscriberToDetach);

	virtual void Notify(INPUT_RECORD& inputEvent);
protected:
	std::vector<Subscriber*> m_subscribers;
};