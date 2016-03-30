#pragma once
namespace Library
{
	//! Action Destroy Action Test Class
	/*! Destroys an action based off of instance name.*/
	class ActionDestroyAction : public Action
	{
		RTTI_DECLARATIONS(ActionDestroyAction, Action)
	public:
		//! Default Constructor
		ActionDestroyAction();

		//! Default Destructor
		virtual ~ActionDestroyAction() = default;

		//! Update
		/*! Finds an action based on it's instance name within
		this local action scope and destroys it.
		\param worldState A reference to the current world state.*/
		virtual void Update(WorldState& worldState);

		//! Clear the list of Prescribed Attributes
		static void Clear();

		//! Get Percribed Attribute Names
		/*! \return A Vector containing the names of each prescribed
		attribute in this action.*/
		virtual Vector<std::string>& PrescribedAttributes() const;

	private:
		std::string mTarget;										//!< Instance name of the action to destroy.

		//! Initiailize any prescribed attribues this action may have
		void InitializeAttributes();
		static Vector<std::string> sPerscribedDestroyAttributes;	//!< Keeps track of the names of each prescribed attribute for this entity
	};
	ConcreteFactory(Action, ActionDestroyAction)
}

