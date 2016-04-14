#include "pch.h"
#include <memory>

namespace Library
{
	RTTI_DEFINITIONS(ActionEvent)
	Vector<std::string> ActionEvent::sPerscribedAttributes;
	const std::string ActionEvent::delayKey = "delay";

	ActionEvent::ActionEvent() :
		mDelay(0), mSubtype("")
	{
		InitializeAttributes();
	}

	void ActionEvent::Update(WorldState & worldState)
	{
		EventMessageAttributed message;
		message.SetSubtype(mSubtype);
		message.SetWorld(worldState.World);

		for (auto& pair : mOrderList)
		{
			std::string& name = pair->first;
			if (IsAuxiliaryAttribute(name))
			{
				//e->Message().Append(name, pair->second);
				message.Append(name, pair->second);
			}
		}

		std::shared_ptr<Event<EventMessageAttributed>> e =
			std::make_shared<Event<EventMessageAttributed>>(message);

		Milliseconds delay(mDelay);
		worldState.GetEventQueue().Enqueue(e, worldState.GetGameTime(), delay);
	}

	void ActionEvent::Clear()
	{
		sPerscribedAttributes.Clear();
	}

	Vector<std::string>& ActionEvent::PrescribedAttributes() const
	{
		return sPerscribedAttributes;
	}

	void ActionEvent::InitializeAttributes()
	{
		AddExternalAttribute(delayKey, 1, &mDelay);
		AddExternalAttribute(ReactionAttributed::subtypeKey, 1, &mSubtype);
	}
}
