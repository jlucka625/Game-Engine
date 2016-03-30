#pragma once
namespace Library
{
	//! Action Create Action Test Class
	/*! Creates actions and places them within the local container
	based on an Action type and an instance name.*/
	class ActionCreateAction : public Action
	{
		RTTI_DECLARATIONS(ActionCreateAction, Action)
	public:
		//! Default Constructor
		ActionCreateAction();

		//! Default Destructor
		virtual ~ActionCreateAction() = default;

		//! Update
		/*! Creates an action based off the prototype name and gives
		it the target instance name."
		\param worldState A reference to the current world state.*/
		virtual void Update(WorldState& worldState) override;

		//! Clear the list of Prescribed Attributes
		static void Clear();

		//! Get Percribed Attribute Names
		/*! \return A Vector containing the names of each prescribed
		attribute in this action.*/
		virtual Vector<std::string>& PrescribedAttributes() const;

	private:
		std::string mActionName;								//!< The instance name of the newly created Action
		std::string mPrototypeName;								//!< The class name of the action you want to create.

		//! Initiailize any prescribed attribues this action may have
		void InitializeAttributes();
		static Vector<std::string> sPerscribedCreateAttributes;	//!< Keeps track of the names of each prescribed attribute for this entity
	};
	ConcreteFactory(Action, ActionCreateAction)
}


