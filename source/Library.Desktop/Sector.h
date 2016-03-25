#pragma once
namespace Library
{
	class World;
	//! Sector Class
	/*! A container that is meant to partition code and
	data into different pieces that can be loaded or
	unloaded into a global Scope.*/
	class Sector final : public Attributed
	{
		RTTI_DECLARATIONS(Sector, Attributed)
	public:
		//! Default Constructor
		Sector();
		Sector(const Sector& rhs) = default;

		Sector& operator=(const Sector& rhs) = default;

		virtual ~Sector() = default;

		//! Initiailize any prescribed attribues this sector may have
		void InitializeAttributes();

		//! Get Percribed Attribute Names
		/*! \return A Vector containing the names of each prescribed
		attribute in this sector.*/
		virtual Vector<std::string>& PrescribedAttributes() const;

		//! Get Instance Name
		/*! \return The instance name of this sector.*/
		const std::string& Name() const;

		//! Set Instance Name
		/*! \param name The instance name you would like this sector
		to have.*/
		void SetName(const std::string& name);

		//! Get Entities
		/*! \return A Datum of type table that represents an array
		of entity pointers.*/
		Datum& Entities() const;

		//! Create Entity
		/*! Instantiates an entity and adopts it into this world
		scope.
		\param instanceName The name of this scope instance
		\return A pointer to the newly created Entity*/
		Entity* CreateEntity(std::string className, std::string instanceName);

		//! Get World
		/*! \return The address of the world that contains
		this sector.*/
		World* GetWorld() const;

		//! Set World
		/*! \param world The address of a world scope that will
		now contain this sector.*/
		void SetWorld(World* world);

		//! Update World
		/*! Iterates through the contained Entities and calls their
		update methods. 
		\param worldState A reference to a world state that keeps track
		of what entity is being processed.*/
		void Update(WorldState& worldState);

		//! Clear the list of Prescribed Attributes
		static void Clear();

	private:
		static Vector<std::string> sPerscribedAttributes;	//!< Keeps track of the names of each prescribed attribute for this sector
		static EntityFactory sEntityFactory;				//!< A static factory used in Create Entity to instantiate entity objects
		std::string mName;									//!< The instance name of this sector object
	};
}


