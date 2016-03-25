#include "pch.h"

namespace Library
{
	RTTI_DEFINITIONS(Sector)

	Vector<std::string> Sector::sPerscribedAttributes;
	EntityFactory Sector::sEntityFactory;

	Sector::Sector() :
		mName("")
	{
		InitializeAttributes();
	}

	void Sector::InitializeAttributes()
	{
		AddExternalAttribute("Name", 1, &mName);
		Datum& datum = Append("Entities");
		datum.SetType(Datum::DatumType::Table);
		sPerscribedAttributes.PushBack("Entities");
	}

	Vector<std::string>& Sector::PrescribedAttributes() const
	{
		return sPerscribedAttributes;
	}

	const std::string & Sector::Name() const
	{
		return mName;
	}

	void Sector::SetName(const std::string & name)
	{
		Find("Name")->Set(name);
	}

	Datum & Sector::Entities() const
	{
		return *Find("Entities");
	}

	Entity * Sector::CreateEntity(std::string className, std::string instanceName)
	{
		Entity* entity = Factory<Entity>::Create(className);
		entity->SetSector(this);
		entity->SetName(instanceName);
		return entity;
	}

	World * Sector::GetWorld() const
	{
		return parent->As<World>();
	}

	void Sector::SetWorld(World * world)
	{
		world->Adopt(*this, "Sectors");
	}

	void Sector::Update(WorldState& worldState)
	{
		Datum* datum = Find("Entities");
		for (std::uint32_t i = 0; i < datum->Size(); ++i)
		{
			Entity* currentEntity = datum->Get<Scope*>(i)->As<Entity>();
			worldState.Entity = currentEntity;
			currentEntity->Update(worldState);
		}
	}

	void Sector::Clear()
	{
		sPerscribedAttributes.Clear();
	}
}
