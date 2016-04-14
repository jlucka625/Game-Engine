#include "pch.h"

namespace Library
{
	WorldState::WorldState() :
		mGameTime(), mEventQueue()
	{}

	GameTime & WorldState::GetGameTime()
	{
		return mGameTime;
	}

	void WorldState::SetGameTime(GameTime& gameTime)
	{
		mGameTime = gameTime;
	}

	EventQueue& WorldState::GetEventQueue()
	{
		return mEventQueue;
	}
}



