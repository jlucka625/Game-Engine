#include "pch.h"

namespace Library
{
	RTTI_DEFINITIONS(Entity)

	Vector<std::string> Entity::sPerscribedAttributes;

	Entity::Entity() :
		mName("")
	{
		InitializeAttributes();
	}

	void Entity::InitializeAttributes()
	{
		AddExternalAttribute("Name", 1, &mName);
	}

	Vector<std::string>& Entity::PrescribedAttributes() const
	{
		return sPerscribedAttributes;
	}

	const std::string & Entity::Name() const
	{
		return mName;
	}

	void Entity::SetName(const std::string & name)
	{
		Find("Name")->Set(name);
	}

	Sector* Entity::GetSector()
	{
		return parent->As<Sector>();
	}

	void Entity::SetSector(Sector* sector)
	{
		sector->Adopt(*this, "Entities");
	}

	void Entity::Update(WorldState& worldState)
	{
		UNREFERENCED_PARAMETER(worldState);
	}

	void Entity::Clear()
	{
		sPerscribedAttributes.Clear();
	}
}
