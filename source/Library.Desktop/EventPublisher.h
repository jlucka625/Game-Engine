#pragma once
#include <chrono>

namespace Library
{ 
	typedef std::chrono::high_resolution_clock::time_point TimePoint;
	typedef std::chrono::milliseconds Milliseconds;

	//! EventPublisher
	/*! Abstract base class providing an interface for sending
	data to subscribers.*/
	class EventPublisher : public RTTI
	{
		RTTI_DECLARATIONS(EventPublisher, RTTI)
	public:
		//! Default Constructor
		/*!\param subscribers The address of a vector of subscribers
		that want to be notified from this particular event.*/
		EventPublisher(Vector<EventSubscriber*>* subscribers);

		//! Copy Constructor
		/*! \param rhs The EventPublisher to deep-copy.*/
		EventPublisher(const EventPublisher& rhs) = default;

		//! Move Constructor
		/*! \param rhs The EventPublisher to move.*/
		EventPublisher(EventPublisher&& rhs);

		//! Copy Assignment Operator
		/*! \param rhs The EventPublisher to deep-copy.
		\return The deep-copied EventPublisher*/
		EventPublisher& operator=(const EventPublisher& rhs) = default;

		//! Move Assignment Operator
		/*! \param rhs The EventPublisher to move.
		\return The moved EventPublisher*/
		EventPublisher& operator=(EventPublisher&& rhs);

		//! Default Destructor
		virtual ~EventPublisher() = default;

		//! Set Expiration Time
		/*!\param timeEnqueued Point in time when this event was placed into
		the event queue.
		\param delay How long after being placed in the event queue before this 
		event expires.*/
		void SetTime(const TimePoint& timeEnqueued, const Milliseconds& delay);
		
		//! Get Time Enqueued
		/*!\return Point in time when this event was placed into
		the event queue.*/
		const TimePoint& TimeEnqueued() const;

		//! Get Delay
		/*! How long after being placed in the event queue before this 
		event expires.*/
		const Milliseconds& Delay() const;

		//! Is Expired
		/*! \param currentTime The current time point within this
		simulation.
		\return If the event has been enqueued for longer than it's
		delay time.*/
		bool IsExpired(const TimePoint& currentTime) const;

		//! Notify Subscribers
		/*! Calls the recieve method of all subscribers
		to this event.*/
		void Send();

	private:
		Vector<EventSubscriber*>* mSubscribers;			//!< Pointer to a vector of subscribers to this event
		TimePoint mTimeEnqueued;						//!< Point in time when this event was enqueued.
		Milliseconds mDelay;							//!< How long to wait before notifying this event's subscribers
	};
}

