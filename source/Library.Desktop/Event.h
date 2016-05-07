#pragma once
#include <thread>
#include <future>
namespace Library
{
	template <typename MessageT>
	//! Event Template
	/*! Concrete Event Publisher template that is 
	reponsible for keeping track of subscribers and the
	data payload that will be sent to them.*/
	class Event : public EventPublisher
	{
		RTTI_DECLARATIONS(Event<MessageT>, EventPublisher)
	public:
		//! Default Constructor
		/*! \param message A reference to the data that will
		be sent to subscribers upon this event firing.*/
		Event(const MessageT& message);

		//! Copy Constructor
		/*! \param rhs The Event to deep-copy.*/
		Event(const Event& rhs);

		//! Move Constructor
		/*! \param rhs The Event to move.*/
		Event(Event&& rhs);

		//! Copy Assignment Operator
		/*! \param rhs The Event to deep-copy.
		\return The deep-copied Event*/
		Event& operator=(const Event& rhs);

		//! Move Assignment Operator
		/*! \param rhs The Event to move.
		\return The moved Event*/
		Event& operator=(Event&& rhs);

		//! Default Destructor
		virtual ~Event() = default;

		//! Add Subscriber
		/*! \param subscriber The address of the subscriber to 
		add to the list of subscribers for this event type.*/
		static void Subscribe(EventSubscriber& subscriber);

		//! Remove Subscriber
		/*! \param subscriber The address of the subscriber to
		remove from the list of subscribers for this event type.*/
		static void Unsubscribe(EventSubscriber& subscriber);

		//! Remove All Subscribers
		/*! Unsubscribes all subscribers to this event type.*/
		static void UnsubscribeAll();

		//! Get Message
		/*! \return A reference to the message payload for this
		event.*/
		const MessageT& Message() const;

	private:
		static Vector<EventSubscriber*> sSubscribers;		//!< A static vector of subscribers so that all events of this type share the same subscribers
		MessageT mMessage;							//!< The data that will be sent to subscribers upon this event firing.
		static std::recursive_mutex mMutex;
	};
}

#include "Event.inl"

