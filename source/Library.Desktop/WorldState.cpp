#include "pch.h"

namespace Library
{
	WorldState::WorldState() :
		mGameTime()
	{}

	GameTime & WorldState::GetGameTime()
	{
		return mGameTime;
	}

	void WorldState::SetGameTime(GameTime& gameTime)
	{
		mGameTime = gameTime;
	}
}



