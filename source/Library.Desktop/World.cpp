#include "pch.h"

namespace Library
{
	RTTI_DEFINITIONS(World)

	Vector<std::string> World::sPerscribedAttributes;

	World::World() :
		mSectors(), mName(""), mWorldState()
	{
		mWorldState.World = this;
		InitializeAttributes();
	}

	void World::InitializeAttributes()
	{
		AddExternalAttribute("Name", 1, &mName);
		AddNestedScope("Sectors", mSectors);
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
		return *Find("Sectors");
	}

	Sector * World::CreateSector(const std::string& instanceName)
	{
		Sector* sector = Factory<Sector>::Create("Sector")->As<Sector>();
		sector->SetWorld(this);
		sector->SetName(instanceName);
		return sector;
	}

	void World::Update()
	{
		Datum* datum = Find("Sectors");
		for (std::uint32_t i = 0; i < datum->Size(); ++i)
		{
			Sector* currentSector = datum->Get<Scope*>(i)->As<Sector>();
			mWorldState.Sector = currentSector;
			currentSector->Update(mWorldState);
		}
	}
}
