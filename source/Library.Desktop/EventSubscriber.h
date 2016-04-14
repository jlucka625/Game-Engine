#pragma once

namespace Library
{
	class EventPublisher;
	//! Event Subscriber
	/*! Abstract interface for objects that want to
	subscribe to a particular event and be notified
	of changes.*/
	class EventSubscriber
	{
	public:
		//! Default Constructor
		EventSubscriber() = default;

		//! Default Destructor
		virtual ~EventSubscriber() = default;

		//! Receive Event 
		/*! \param e The EventPublisher that has been fired. 
		Contains a message with data that the subscriber
		is interested in.*/
		virtual void Receive(const EventPublisher& e) = 0;
	};
}


