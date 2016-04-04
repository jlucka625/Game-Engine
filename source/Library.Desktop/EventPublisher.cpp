#include "pch.h"

namespace Library
{
	RTTI_DEFINITIONS(EventPublisher)

	EventPublisher::EventPublisher(Vector<EventSubscriber*>* subscribers) :
		mSubscribers(subscribers)
	{}

	EventPublisher::EventPublisher(EventPublisher && rhs)
	{
		operator=(std::move(rhs));
	}

	EventPublisher & EventPublisher::operator=(EventPublisher && rhs)
	{
		if (this != &rhs)
		{
			mTimeEnqueued = rhs.mTimeEnqueued;
			mDelay = rhs.mDelay;
		}
		return *this;
	}

	void EventPublisher::SetTime(const TimePoint & timeEnqueued, const Milliseconds & delay)
	{
		mTimeEnqueued = timeEnqueued;
		mDelay = delay;
	}

	const TimePoint & EventPublisher::TimeEnqueued() const
	{
		return mTimeEnqueued;
	}

	const Milliseconds & EventPublisher::Delay() const
	{
		return mDelay;
	}

	bool EventPublisher::IsExpired(const TimePoint & currentTime) const
	{
		return (mTimeEnqueued + mDelay >= currentTime);
	}

	void EventPublisher::Send()
	{
		for (auto& subscriber : *mSubscribers)
		{
			subscriber->Receive(this);
		}
	}
}
