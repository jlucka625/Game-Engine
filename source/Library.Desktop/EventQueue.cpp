#include "pch.h"

using namespace std;

namespace Library
{
	EventQueue::EventQueue() :
		mPublishers(0), mExpiredPublishers(0)
	{}

	void EventQueue::Enqueue(const std::shared_ptr<EventPublisher>& e, GameTime& gameTime, Milliseconds delay)
	{
		std::lock_guard<std::mutex> lock(mMutex);
		if (mPublishers.Find(e) == mPublishers.end())
		{
			e->SetTime(gameTime.CurrentTime(), delay);
			mPublishers.PushBack(e);
		}
	}

	void EventQueue::Send(const std::shared_ptr<EventPublisher>& e)
	{
		std::lock_guard<std::mutex> lock(mMutex);
		e->Send();
		mPublishers.Remove(e);
	}

	void EventQueue::Update(const GameTime & gameTime)
	{
		vector<future<void>> futures;

#pragma region Deliver Block
		{
			std::lock_guard<std::mutex> lock(mMutex);
			for (auto& e : mPublishers)
			{
				if (e->IsExpired(gameTime.CurrentTime()))
				{
					futures.emplace_back(async(&EventPublisher::Send, e));
					mExpiredPublishers.PushBack(e);
				}
			}
		}
#pragma endregion

		for (auto& future : futures)
		{
			future.get();
		}

		for (auto& e : mExpiredPublishers)
		{
			mPublishers.Remove(e);
		}

		mExpiredPublishers.Clear();
	}

	void EventQueue::Clear()
	{
		std::lock_guard<std::mutex> lock(mMutex);
		mPublishers.Clear();
	}

	bool EventQueue::IsEmpty() const
	{
		std::lock_guard<std::mutex> lock(mMutex);
		return mPublishers.IsEmpty();
	}

	std::uint32_t EventQueue::Size() const
	{
		std::lock_guard<std::mutex> lock(mMutex);
		return mPublishers.Size();
	}
}
