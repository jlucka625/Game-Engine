#pragma once
namespace Library
{
	//! Action Event
	/*! An action who's purpose is to fire events that this simulation
	will react to via script.*/
	class ActionEvent : public Action
	{
		RTTI_DECLARATIONS(ActionEvent, Action)
	public:
		//! Default Constructor
		ActionEvent();
		ActionEvent(const ActionEvent& rhs) = delete;
		ActionEvent(ActionEvent&& rhs) = delete;

		ActionEvent& operator=(const ActionEvent& rhs) = delete;
		ActionEvent& operator=(ActionEvent&& rhs) = delete;

		//! Default Destructor
		virtual ~ActionEvent() = default;

		//! Update
		/*! Creates an Event containing an Event Message and populates
		it with any arguments that were provided with this action. The
		event is the enqueued.
		\param worldState A reference to the current world state.*/
		virtual void Update(WorldState& worldState) override;

		//! Clear the list of Prescribed Attributes
		static void Clear();

		//! Get Percribed Attribute Names
		/*! \return A Vector containing the names of each prescribed
		attribute in this action event.*/
		virtual Vector<std::string>& PrescribedAttributes() const;

		static const std::string delayKey;						//!< The key to access the Datum storing the delay amount

	private:
		//! Initiailize any prescribed attribues this action may have
		void InitializeAttributes();

		static Vector<std::string> sPerscribedAttributes;		//!< Keeps track of the names of each prescribed attribute for this action

		int mDelay;												//!< The amount of milliseconds to delay the firing of this event
		std::string mSubtype;									//!< The subtype to help the reaction filter out incoming events
	};
	ConcreteFactory(Action, ActionEvent)
}

