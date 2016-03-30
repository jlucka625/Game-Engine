#pragma once
namespace Library
{
	//! Action Assign Test Class
	/*! The purpose of this class is to re-assign a value
	to the condition for an ActionIf.*/
	class ActionAssign : public Action
	{
		RTTI_DECLARATIONS(ActionAssign, Action)
	public:
		//! Default Constructor
		ActionAssign();

		//! Default Destructor
		virtual ~ActionAssign() = default;

		//! Update
		/*! Re-assigns the value of the condition for the target
		ActionIf provided to this action.
		\param worldState A reference to the current world state.*/
		virtual void Update(WorldState& worldState) override;

		//! Clear the list of Prescribed Attributes
		static void Clear();

		//! Get Percribed Attribute Names
		/*! \return A Vector containing the names of each prescribed
		attribute in this action.*/
		virtual Vector<std::string>& PrescribedAttributes() const;

	private:
		std::string mTarget;										//!< Name of the target ActionIf instance to change
		int mValue;													//!< Value to change the condition to

		//! Initiailize any prescribed attribues this action may have
		void InitializeAttributes();
		static Vector<std::string> sPerscribedAssignAttributes;		//!< Keeps track of the names of each prescribed attribute for this entity
	};
	ConcreteFactory(Action, ActionAssign)
}

