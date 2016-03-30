#pragma once
namespace Library
{
	class ActionList : public Action
	{
		RTTI_DECLARATIONS(ActionList, Action)
	public:
		ActionList() = default;
		virtual ~ActionList() = default;

		virtual void Update(WorldState& worldState);

		//! Clear the list of Prescribed Attributes
		static void Clear();

		//! Get Percribed Attribute Names
		/*! \return A Vector containing the names of each prescribed
		attribute in this entity.*/
		virtual Vector<std::string>& PrescribedAttributes() const;

	private:
		//! Initiailize any prescribed attribues this sector may have
		void InitializeAttributes();
		static Vector<std::string> sPerscribedListAttributes;	//!< Keeps track of the names of each prescribed attribute for this entity
	};
	ConcreteFactory(Action, ActionList)
}
