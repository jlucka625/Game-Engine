#include "pch.h"

namespace Library
{
	EventQueue::EventQueue() :
		mPublishers(0), mExpiredPublishers(0)
	{}

	void EventQueue::Enqueue(const std::shared_ptr<EventPublisher>& e, GameTime& gameTime, Milliseconds delay)
	{
		e->SetTime(gameTime.CurrentTime(), delay);
		mPublishers.PushBack(e);
	}

	void EventQueue::Send(const std::shared_ptr<EventPublisher>& e)
	{
		e->Send();
		mPublishers.Remove(e);
	}

	void EventQueue::Update(const GameTime & gameTime)
	{
		for (auto& e : mPublishers)
		{
			if (e->IsExpired(gameTime.CurrentTime()))
			{
				e->Send();
				mExpiredPublishers.PushBack(e);
			}
		}

		for (auto& e : mExpiredPublishers)
		{
			mPublishers.Remove(e);
		}

		mExpiredPublishers.Clear();
	}

	void EventQueue::Clear()
	{
		mPublishers.Clear();
	}

	bool EventQueue::IsEmpty() const
	{
		return mPublishers.IsEmpty();
	}

	std::uint32_t EventQueue::Size() const
	{
		return mPublishers.Size();
	}
}
