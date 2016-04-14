#include "pch.h"

namespace Library
{
	RTTI_DEFINITIONS(World)

	Vector<std::string> World::sPerscribedAttributes;
	const std::string World::sectorsKey = "Sectors";

	World::World() :
		mName(""), mWorldState(), mGameClock()
	{
		mWorldState.World = this;
		InitializeAttributes();
	}

	WorldState& World::GetWorldState()
	{
		return mWorldState;
	}

	void World::InitializeAttributes()
	{
		AddExternalAttribute("Name", 1, &mName);
		Datum& datum = Append("Sectors");
		datum.SetType(Datum::DatumType::Table);
		sPerscribedAttributes.PushBack("Sectors");
	}

	Vector<std::string>& World::PrescribedAttributes() const
	{
		return sPerscribedAttributes;
	}

	const std::string & World::Name() const
	{
		return mName;
	}

	void World::SetName(const std::string & name)
	{
		Find("Name")->Set(name);
	}

	Datum & World::Sectors() const
	{
		Datum* datum = Find(sectorsKey);
		if (!datum)
			throw std::exception("Sectors not found.");

		return *datum;
	}

	Sector * World::CreateSector(const std::string& instanceName)
	{
		Sector* sector = new Sector();
		sector->SetWorld(this);
		sector->SetName(instanceName);
		return sector;
	}

	void World::Update()
	{
		mGameClock.UpdateGameTime(mWorldState.GetGameTime());
		mWorldState.GetEventQueue().Update(mWorldState.GetGameTime());
		Datum* datum = Find(sectorsKey);
		if (!datum)
			return;

		for (std::uint32_t i = 0; i < datum->Size(); ++i)
		{
			Sector* currentSector = static_cast<Sector*>(datum->Get<Scope*>(i));
			mWorldState.Sector = currentSector;
			currentSector->Update(mWorldState);
		}
	}

	void World::Clear()
	{
		sPerscribedAttributes.Clear();
	}
}
