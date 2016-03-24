#pragma once
namespace Library
{
	class WorldState
	{
	public:
		WorldState();
		~WorldState() = default;

		GameTime& GetGameTime();
		void SetGameTime(GameTime& gameTime);

		class World* World;
		class Sector* Sector;
		class Entity* Entity;
		//class Action* Action;

	private:
		GameTime mGameTime;
	};
}


