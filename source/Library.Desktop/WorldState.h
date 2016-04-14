#pragma once
namespace Library
{
	//! Represents current state of the world scope
	class WorldState
	{
	public:
		//! Default Constructor
		WorldState();

		//!Default Destructor
		~WorldState() = default;

		//! Get GameTime
		/*! \return A game time object that provides total time since
		game began and elapsed time between frames.*/
		GameTime& GetGameTime();

		//! Set GameTime
		/*! \param gameTime A reference to a game time that you want
		to be seeded into the world state.*/
		void SetGameTime(GameTime& gameTime);

		EventQueue& GetEventQueue();

		class World* World;			//!< Address of world currently being processed
		class Sector* Sector;		//!< Address of sector currently being processed
		class Entity* Entity;		//!< Address of entity currently being processed
		class Action* Action;		//!< Address of action currently being processed

	private:
		GameTime mGameTime;			//!< A gameTime object to be seeded into this world state
		EventQueue mEventQueue;
	};
}


