#pragma once
namespace Library 
{
	class Sector;
	//! Entity Class
	/*! A wrapper for a table/attributed scope that represents
	a game object that will populate the world.*/
	class Entity : public Attributed
	{
		RTTI_DECLARATIONS(Entity, Attributed)
	public:
		//! Default Constructor
		Entity();
		//Entity(const Entity& rhs) = default;

		//Entity& operator=(const Entity& rhs) = default;

		virtual ~Entity() = default;

		//! Get Percribed Attribute Names
		/*! \return A Vector containing the names of each prescribed
		attribute in this entity.*/
		virtual Vector<std::string>& PrescribedAttributes() const;

		//! Get Instance Name
		/*! \return The instance name of this entity.*/
		const std::string& Name() const;

		//! Set Instance Name
		/*! \param name The instance name you would like this entity
		to have.*/
		void SetName(const std::string& name);

		//! Get Actions
		/*! \return A Datum of type table that represents an array
		of action pointers.*/
		Datum& Actions() const;

		//! Create Actions
		/*! Instantiates an action and adopts it into this entity
		scope.
		\param instanceName The name of this scope instance
		\return A pointer to the newly created Action*/
		Action* CreateAction(std::string className, std::string instanceName);

		//! Get World
		/*! \return The address of the sector that contains
		this entity.*/
		Sector* GetSector();

		//! Set World
		/*! \param world The address of a sector that will
		now contain this entity.*/
		void SetSector(Sector* sector);

		//! Update World
		/*! Iterates through the contained Actions and calls their
		update methods.
		\param worldState A reference to a world state that keeps track
		of what action is being processed.*/
		virtual void Update(WorldState& worldState);

		//! Clear the list of Prescribed Attributes
		static void Clear();

		static const std::string actionsKey;

	protected:
		std::string mName;									//!< The instance name of this entity object

	private:
		//! Initiailize any prescribed attribues this sector may have
		void InitializeAttributes();
		static Vector<std::string> sPerscribedAttributes;	//!< Keeps track of the names of each prescribed attribute for this entity
	};
	ConcreteFactory(Entity, Entity)
}


