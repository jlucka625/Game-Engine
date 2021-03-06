#include "pch.h"

using namespace std;

namespace Library
{
	RTTI_DEFINITIONS(EventPublisher)

	EventPublisher::EventPublisher(Vector<EventSubscriber*>& subscribers, std::recursive_mutex* mutex) :
		mSubscribers(subscribers), mMutex(mutex), mTimeEnqueued(), mDelay()
	{}

	EventPublisher::EventPublisher(const EventPublisher & rhs) :
		mSubscribers(rhs.mSubscribers), mMutex(rhs.mMutex), mTimeEnqueued(rhs.mTimeEnqueued), mDelay(rhs.mDelay)
	{
	}

	EventPublisher::EventPublisher(EventPublisher && rhs) :
		mSubscribers(rhs.mSubscribers), mTimeEnqueued(rhs.mTimeEnqueued), mDelay(rhs.mDelay)
	{
	}

	EventPublisher & EventPublisher::operator=(const EventPublisher & rhs)
	{
		if (this != &rhs)
		{
			mTimeEnqueued = rhs.mTimeEnqueued;
			mDelay = rhs.mDelay;
		}
		return *this;
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
		return (mTimeEnqueued + mDelay <= currentTime);
	}

	void EventPublisher::Send()
	{
		vector<future<void>> futures;

#pragma region Deliver Block
		{
			lock_guard<std::recursive_mutex> lock(*mMutex);
			for (auto& subscriber : mSubscribers)
			{
				futures.emplace_back(async(&EventSubscriber::Receive, subscriber, cref(*this)));
			}
		}
#pragma endregion

		for (auto& future : futures)
		{
			future.get();
		}
	}
}
