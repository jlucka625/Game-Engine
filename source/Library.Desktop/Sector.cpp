#include "pch.h"

namespace Library
{
	RTTI_DEFINITIONS(Sector)

	Vector<std::string> Sector::sPerscribedAttributes;
	EntityFactory Sector::sEntityFactory;
	const std::string Sector::entitiesKey = "Entities";

	Sector::Sector() :
		mName("")
	{
		InitializeAttributes();
	}

	void Sector::InitializeAttributes()
	{
		AddExternalAttribute("Name", 1, &mName);
		Datum& datum = Append(entitiesKey);
		datum.SetType(Datum::DatumType::Table);
		sPerscribedAttributes.PushBack(entitiesKey);
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
		Datum* datum = Find(entitiesKey);
		if (!datum)
			throw std::exception("Entities not found.");

		return *datum;
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
		if (!parent)
			return nullptr;

		return parent->As<World>();
	}

	void Sector::SetWorld(World * world)
	{
		if (GetWorld())
			Orphan();

		if (!world)
			throw std::exception("World is null");

		world->Adopt(*this, World::sectorsKey);
	}

	void Sector::Update(WorldState& worldState)
	{
		Datum* datum = Find(entitiesKey);
		if (!datum)
			return;

		for (std::uint32_t i = 0; i < datum->Size(); ++i)
		{
			Entity* currentEntity = static_cast<Entity*>(datum->Get<Scope*>(i));
			worldState.Entity = currentEntity;
			currentEntity->Update(worldState);
		}
	}

	void Sector::Clear()
	{
		sPerscribedAttributes.Clear();
	}
}
