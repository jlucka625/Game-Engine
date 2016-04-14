#pragma once
namespace Library
{
	//! World Class
	/*! Defines a global Scope - the Scope that all other objects
	in the simulation share.*/
	class World final : public Attributed
	{
		RTTI_DECLARATIONS(World, Attributed)
	public:
		//! Default Constructor
		World();
		World(const World& rhs) = delete;

		World& operator=(const World& rhs) = delete;

		virtual ~World() = default;

		WorldState& GetWorldState();

		//! Initiailize any prescribed attribues this world may have
		void InitializeAttributes();

		//! Get Percribed Attribute Names
		/*! \return A Vector containing the names of each prescribed
		attribute in this world.*/
		virtual Vector<std::string>& PrescribedAttributes() const;

		//! Get Instance Name
		/*! \return The instance name of this world.*/
		const std::string& Name() const;

		//! Set Instance Name
		/*! \param name The instance name you would like this world
		to have.*/
		void SetName(const std::string& name);

		//! Get Sectors
		/*! \return A Datum of type table that represents an array
		of sector pointers.*/
		Datum& Sectors() const;

		//! Create Sector
		/*! Instantiates a sector and adopts it into this world 
		scope.
		\param instanceName The name of this scope instance
		\return A pointer to the newly created Sector*/
		Sector* CreateSector(const std::string& instanceName);

		//! Update World
		/*! Iterates through the contained Sectors and calls their
		update methods. This also updates the world state to whatever
		sector is currently being processed.*/
		void Update();

		//! Clear the list of Prescribed Attributes
		static void Clear();

		static const std::string sectorsKey;

	private:
		static Vector<std::string> sPerscribedAttributes;	//!< Keeps track of the names of each prescribed attribute for this world
		std::string mName;									//!< The instance name of this world object
		WorldState mWorldState;								//!< A world state instance to keep track of what is being processed
		GameClock mGameClock;								//!< Responsible for updating the world state's game time.
	};
}

