#include "pch.h"

namespace Library
{
	RTTI_DEFINITIONS(EventMessageAttributed)
	Vector<std::string> EventMessageAttributed::sPerscribedAttributes;

	EventMessageAttributed::EventMessageAttributed() :
		mSubtype(""), mWorld(nullptr)
	{}

	EventMessageAttributed::EventMessageAttributed(const EventMessageAttributed & rhs) :
		Attributed(rhs), mSubtype(rhs.mSubtype), mWorld(rhs.mWorld)
	{}

	EventMessageAttributed::EventMessageAttributed(EventMessageAttributed && rhs) :
		Attributed(std::move(rhs)), mSubtype(rhs.mSubtype), mWorld(rhs.mWorld)
	{
		rhs.mWorld = nullptr;
	}

	EventMessageAttributed & EventMessageAttributed::operator=(const EventMessageAttributed & rhs)
	{
		if (this != &rhs)
		{
			Attributed::operator=(rhs);
			mSubtype = rhs.mSubtype;
			mWorld = rhs.mWorld;
		}
		return *this;
	}

	EventMessageAttributed & EventMessageAttributed::operator=(EventMessageAttributed && rhs)
	{
		if (this != &rhs)
		{
			Attributed::operator=(std::move(rhs));
			mSubtype = rhs.mSubtype;
			mWorld = rhs.mWorld;

			rhs.mWorld = nullptr;
		}
		return *this;
	}

	const std::string & EventMessageAttributed::GetSubtype() const
	{
		return mSubtype;
	}
	void EventMessageAttributed::SetSubtype(std::string & subtype)
	{
		mSubtype = subtype;
	}
	World * EventMessageAttributed::GetWorld() const
	{
		return mWorld;
	}
	void EventMessageAttributed::SetWorld(World * world)
	{
		mWorld = world;
	}

	void EventMessageAttributed::Clear()
	{
		sPerscribedAttributes.Clear();
	}

	Vector<std::string>& EventMessageAttributed::PrescribedAttributes() const
	{
		return sPerscribedAttributes;
	}
}



