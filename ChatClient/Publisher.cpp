#include "Publisher.h"

bool Publisher::Observe(Subscriber* subscriberToObserve)
{
	auto checkIter = std::find(m_subscribers.begin(), 
							   m_subscribers.end(),
							   subscriberToObserve);

	bool result = (checkIter == m_subscribers.end());

	if (result == true)
	{
		m_subscribers.push_back(subscriberToObserve);
	}

	return result;
}

bool Publisher::Detach(Subscriber* subscriberToDetach)
{
	auto checkIter = std::find(m_subscribers.begin(),
							   m_subscribers.end(),
							   subscriberToDetach);

	bool result = (checkIter != m_subscribers.end());

	if (result == true)
	{
		m_subscribers.erase(checkIter);
	}

	return result;
}

void Publisher::Notify(INPUT_RECORD& inputEvent)
{
	for (auto& subscriber : m_subscribers)
	{
		subscriber->Update(inputEvent);
	}
}
