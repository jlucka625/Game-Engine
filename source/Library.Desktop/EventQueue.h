#pragma once
#include <memory>
#include <thread>
#include <future>
#include <vector>

namespace Library
{
	typedef std::chrono::milliseconds Milliseconds;
	//! Event Queue
	/*! Keeps track of the events ready to be fired after
	a designated amount of time.*/
	class EventQueue
	{
	public:
		//! Default Constructor
		EventQueue();

		EventQueue(const EventQueue& rhs) = delete;
		EventQueue& operator=(const EventQueue& rhs) = delete;

		//! Default Destructor
		~EventQueue() = default;

		//! Enqueue Event
		/*! \param e The event publisher to enqueue
		\param gameTime A reference to a Game Time object that contains 
		the current time when this event was enqueued.
		\param delay How long to wait before firing this event.*/
		void Enqueue(const std::shared_ptr<EventPublisher>& e, GameTime& gameTime, Milliseconds delay);
		
		//! Send Event Immediately
		/*! \param A shared pointer to an EventPublisher that you
		want to fire regardless of delay time.*/
		void Send(const std::shared_ptr<EventPublisher>& e);

		//! Update Queue
		/*!	Publishes any queued events that have expired, and then
		removes the expired events from the queue.
		\param gameTime A reference containing the current time.*/
		void Update(const GameTime& gameTime);

		//! Clear Queue
		/*! Clears out the queue of all events. This does NOT
		fire expired events that have not yet been sent.*/
		void Clear();

		//! Is Empty
		/*! \return If this queue has a size of zero.*/
		bool IsEmpty() const;

		//! Get Size
		/*! \return How many events are stored in this queue.*/
		std::uint32_t Size() const;

	private:
		Vector<std::shared_ptr<EventPublisher>> mPublishers;				//!< Vector of publishers in this queue
		Vector<std::shared_ptr<EventPublisher>> mExpiredPublishers;			//!< Vector of publishers that have expired, been sent, and are waiting to be cleared out from the queue.
		mutable std::mutex mMutex;
	};
}

