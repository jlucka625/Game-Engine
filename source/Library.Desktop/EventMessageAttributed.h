#pragma once
namespace Library
{
	//! Event Message Attributed Class
	/*! Serves as a generic event component that holds the message that
	will be sent to a reaction. This event carries all arguments and the
	subtype from script within its scope.*/
	class EventMessageAttributed : public Attributed
	{
		RTTI_DECLARATIONS(EventMessageAttributed, Attributed)
	public:
		//! Default Constructor
		EventMessageAttributed();

		//! Copy Constructor
		/*! \param rhs The Event Message to deep-copy.*/
		EventMessageAttributed(const EventMessageAttributed& rhs);

		//! Move Constructor
		/*! \param rhs The Event Message to move.*/
		EventMessageAttributed(EventMessageAttributed&& rhs);

		//! Copy Assignment Operator
		/*! \param rhs The Event Message to deep-copy.
		\return The deep-copied Event Message*/
		EventMessageAttributed& operator=(const EventMessageAttributed& rhs);

		//! Move Assignment Operator
		/*! \param rhs The Event Message to move.
		\return The moved Event Message*/
		EventMessageAttributed& operator=(EventMessageAttributed&& rhs);

		//! Default Destructor
		virtual ~EventMessageAttributed() = default;

		//! Get Subtype
		/*! \return A constant reference to the subtype of this event message.*/
		const std::string& GetSubtype() const;

		//! Set Subtype
		/*! \subtype The name you want to give the subtype of this event message.*/
		void SetSubtype(std::string& subtype);

		//! Get World Address
		/*! \return The address of the World that contains this event message.*/
		World* GetWorld() const;

		//! Set World Address
		/*! \param world The address of the World that contains this event message.*/
		void SetWorld(World* world);

		//! Clear the list of Prescribed Attributes
		static void Clear();

		//! Get Percribed Attribute Names
		/*! \return A Vector containing the names of each prescribed
		attribute in this event message.*/
		virtual Vector<std::string>& PrescribedAttributes() const override;

	private:
		static Vector<std::string> sPerscribedAttributes;		//!< Keeps track of the names of each prescribed attribute for this event message

		std::string mSubtype;									//!< Name of the subtype to help the reactions filter out incoming events.
		World* mWorld;											//!< Address to the World that contains this event message.
	};
}

