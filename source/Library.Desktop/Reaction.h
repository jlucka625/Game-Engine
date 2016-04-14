#pragma once
namespace Library
{
	//! Reaction Abstract Base Class
	/*! Provides an interface for calling actions in response to an event.
	Guarauntees that anything that derives from it is both an Action List
	and an Event Subscriber.*/
	class Reaction : public ActionList, public EventSubscriber
	{
		RTTI_DECLARATIONS(Reaction, ActionList)
	public:
		//! Default Constructor
		Reaction() = default;

		Reaction(const Reaction& rhs) = delete;
		Reaction& operator=(const Reaction& rhs) = delete;

		//! Default Destructor
		virtual ~Reaction() = default;

		//! Recieve Event Notification
		/*! \param An event publisher that this event is subscribed to.*/
		virtual void Receive(const EventPublisher& e) = 0;

		//! Update override
		/*! We don't want the behavior of Action List Update here since we
		only want actions to run under a certain condition.*/
		virtual void Update(WorldState& worldState) override
		{
			UNREFERENCED_PARAMETER(worldState);
		}
	};

	//! Reaction Attributed
	/*! Implements Reaction and subscribes itself to EventMessageAttributed
	events so that it can recieve event notifications from script.*/
	class ReactionAttributed : public Reaction
	{
		RTTI_DECLARATIONS(ReactionAttributed, Reaction)
	public:
		//!Default Constructor
		ReactionAttributed();
		ReactionAttributed(const ReactionAttributed& rhs) = delete;
		ReactionAttributed(ReactionAttributed&& rhs) = delete;

		ReactionAttributed& operator=(const ReactionAttributed& rhs) = delete;
		ReactionAttributed& operator=(ReactionAttributed&& rhs) = delete;

		//! Default Destructor
		virtual ~ReactionAttributed() = default;

		//! Receive Event Notification
		/*! Takes the arguments from the event message and then updates
		the action list if the event and reaction subtypes match.
		\param An event publisher that this event is subscribed to.*/
		virtual void Receive(const EventPublisher& e) override;

		static const std::string subtypeKey;					//!< Key to access the datum that stores the subtype name

		//! Clear the list of Prescribed Attributes
		static void Clear();

		//! Get Percribed Attribute Names
		/*! \return A Vector containing the names of each prescribed
		attribute in this reaction.*/
		virtual Vector<std::string>& PrescribedAttributes() const override;
	private:
		//! Initiailize any prescribed attribues this Reaction may have
		void InitializeAttributes();

		static Vector<std::string> sPerscribedAttributes;		//!< Keeps track of the names of each prescribed attribute for this reaction

		std::string mSubtype;									//!< Name of the subtype to help filter out incoming events.
	};
	ConcreteFactory(Reaction, ReactionAttributed)
}

