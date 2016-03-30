#pragma once
namespace Library
{
	//! Action Abstract Base Class
	/*! Represents a behvior or function that will be performed
	within the simulation and can be built from XML Script.*/
	class Action : public Attributed
	{
		RTTI_DECLARATIONS(Action, Attributed)
	public:
		//! Default Constructor
		Action();

		//! Default Destructor
		virtual ~Action() = default;

		//! Pure Virtual Update
		/*! \param worldState A reference to the current world state.*/
		virtual void Update(WorldState& worldState) = 0;

		//! Name Accessor
		/*! \return The instance name of this Action.*/
		const std::string& Name() const;

		//! Name Mutator
		/*! \param name The instance name that this Action
		should have.*/
		void SetName(const std::string& name);

		//! Clear the list of Prescribed Attributes
		static void Clear();

		//! Get Percribed Attribute Names
		/*! \return A Vector containing the names of each prescribed
		attribute in this Action.*/
		virtual Vector<std::string>& PrescribedAttributes() const;

		static const std::string actionsKey;				//!< The key to access the Datum storing Action pointers
		static const std::string targetKey;					//!< The key to access the Datum containing the target name
		static const std::string valueKey;					//!< The key to access the Datum containing a value to be changed.
		static const std::string prototypeKey;				//!< The key to access the Datum containing the name of the prototype object to be created.

	private:
		//! Initiailize any prescribed attribues this action may have
		void InitializeAttributes();
		static Vector<std::string> sPerscribedAttributes;	//!< Keeps track of the names of each prescribed attribute for this entity
	
		std::string mName;									//!< The instance name of this action object				
	};
}



