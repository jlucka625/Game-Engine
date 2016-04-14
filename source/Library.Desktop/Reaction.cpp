#include "pch.h"

namespace Library
{
	RTTI_DEFINITIONS(Reaction)
	RTTI_DEFINITIONS(ReactionAttributed)
	Vector<std::string> ReactionAttributed::sPerscribedAttributes;
	const std::string ReactionAttributed::subtypeKey = "subtype";

	ReactionAttributed::ReactionAttributed() :
		mSubtype("")
	{
		InitializeAttributes();
	}

	void ReactionAttributed::Receive(const EventPublisher& e)
	{
		Event<EventMessageAttributed>* attributedEvent = e.As<Event<EventMessageAttributed>>();
		if (attributedEvent)
		{
			const EventMessageAttributed& message = attributedEvent->Message();
			if (message.GetSubtype() == mSubtype)
			{
				for (auto& pair : message.GetData())
				{
					std::string& name = pair->first;
					if (message.IsAuxiliaryAttribute(name))
					{
						Append(name, pair->second);
					}
				}
				ActionList::Update(message.GetWorld()->GetWorldState());
			}
		}
	}

	void ReactionAttributed::Clear()
	{
		sPerscribedAttributes.Clear();
	}

	Vector<std::string>& ReactionAttributed::PrescribedAttributes() const
	{
		return sPerscribedAttributes;
	}

	void ReactionAttributed::InitializeAttributes()
	{
		AddExternalAttribute(subtypeKey, 1, &mSubtype);
	}
}
