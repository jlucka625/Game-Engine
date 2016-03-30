#pragma once
namespace Library
{
	//! Action If
	/*! An action the represents an if statement. This runs
	a certain set of actions depending on it's condition.*/
	class ActionIf : public Action
	{
		RTTI_DECLARATIONS(ActionIf, Action)
	public:
		//! Default Constructor
		ActionIf();

		//! Default Destructor
		virtual ~ActionIf() = default;

		//! Update
		/*! If this action's condition evaluates to true,
		then it's "Then" actions will be updated, otherwise,
		it's "Else" actions will be updated.
		\param worldState A reference to the current world state.*/
		virtual void Update(WorldState& worldState) override;

		//! Clear the list of Prescribed Attributes
		static void Clear();

		//! Get Percribed Attribute Names
		/*! \return A Vector containing the names of each prescribed
		attribute in this action list.*/
		virtual Vector<std::string>& PrescribedAttributes() const;

		//! "Then" ActionList Accessor
		/*! \return A reference to the actions that will execute
		if the condition is true.*/
		ActionList& ThenList();

		//! "Else" ActionList Accessor
		/*! \return A reference to the actions that will execute
		if the condition is false.*/
		ActionList& ElseList();

		static const std::string conditionKey;				//!< The key to access the Datum storing the condition
		static const std::string thenKey;					//!< The key to access the Datum storing "then" action list
		static const std::string elseKey;					//!< The key to access the Datum storing "else" action list

	private:
		//! Initiailize any prescribed attribues this action may have
		void InitializeAttributes();
		static Vector<std::string> sPerscribedIfAttributes;	//!< Keeps track of the names of each prescribed attribute for this entity
		
		int mCondition;										//!< Represents the condition to evalute which list of actions.
		ActionList mThenList;								//!< List of all the actions that will execute if the condition is true
		ActionList mElseList;								//!< List of all the actions that will execute if the condition is false
	};
	ConcreteFactory(Action, ActionIf)
}


