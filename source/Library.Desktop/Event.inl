//#include "Event.h"
namespace Library
{
	template <typename MessageT>
	RTTI_DEFINITIONS(Event<MessageT>)

	template <typename MessageT>
	Vector<EventSubscriber*> Event<MessageT>::sSubscribers(0);

	template <typename MessageT>
	std::recursive_mutex Event<MessageT>::mMutex;

	template <typename MessageT>
	Event<MessageT>::Event(const MessageT& message) :
		mMessage(message), EventPublisher(sSubscribers, &mMutex)
	{}

	template<typename MessageT>
	Event<MessageT>::Event(const Event& rhs) :
		mMessage(rhs.mMessage), EventPublisher(rhs)
	{}

	template<typename MessageT>
	Event<MessageT>::Event(Event && rhs) :
		mMessage(std::move(rhs.mMessage)), EventPublisher(std::move(rhs))
	{}

	template<typename MessageT>
	Event<MessageT>& Event<MessageT>::operator=(const Event & rhs)
	{
		if (this != &rhs)
		{
			EventPublisher::operator=(rhs);
			mMessage = rhs.mMessage;
		}
		return *this;
	}

	template<typename MessageT>
	Event<MessageT>& Event<MessageT>::operator=(Event && rhs)
	{
		if (this != &rhs)
		{
			EventPublisher::operator=(std::move(rhs));
			mMessage = std::move(rhs.mMessage);
		}
		return *this;
	}

	template <typename MessageT>
	void Event<MessageT>::Subscribe(EventSubscriber& subscriber)
	{
		std::lock_guard<std::recursive_mutex> lock(mMutex);
		if(sSubscribers.Find(&subscriber) == sSubscribers.end())
			sSubscribers.PushBack(&subscriber);
	}

	template <typename MessageT>
	void Event<MessageT>::Unsubscribe(EventSubscriber& subscriber)
	{
		std::lock_guard<std::recursive_mutex> lock(mMutex);
		sSubscribers.Remove(&subscriber);
	}

	template <typename MessageT>
	void Event<MessageT>::UnsubscribeAll()
	{
		std::lock_guard<std::recursive_mutex> lock(mMutex);
		sSubscribers.Clear();
	}

	template <typename MessageT>
	const MessageT& Event<MessageT>::Message() const
	{
		return mMessage;
	}
}
