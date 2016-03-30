#include "pch.h"

namespace Library
{
	RTTI_DEFINITIONS(Entity)

	Vector<std::string> Entity::sPerscribedAttributes;
	const std::string Entity::actionsKey = "Actions";

	Entity::Entity() :
		mName("")
	{
		InitializeAttributes();
	}

	void Entity::InitializeAttributes()
	{
		AddExternalAttribute("Name", 1, &mName);
		Datum& datum = Append(actionsKey);
		datum.SetType(Datum::DatumType::Table);
		sPerscribedAttributes.PushBack(actionsKey);
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

	Datum & Entity::Actions() const
	{
		Datum* datum = Find(actionsKey);
		if (!datum)
			throw std::exception("Actions not found.");

		return *datum;
	}

	Action * Entity::CreateAction(std::string className, std::string instanceName)
	{
		Action* action = Factory<Action>::Create(className);
		Adopt(*action, actionsKey);
		action->SetName(instanceName);
		return action;
	}

	Sector* Entity::GetSector()
	{
		if (!parent)
			return nullptr;

		return parent->As<Sector>();
	}

	void Entity::SetSector(Sector* sector)
	{
		if (GetSector())
			Orphan();

		if (!sector)
			throw std::exception("Sector is null");

		sector->Adopt(*this, Sector::entitiesKey);
	}

	void Entity::Update(WorldState& worldState)
	{
		Datum* datum = Find(actionsKey);
		if (!datum)
			return;

		for (std::uint32_t i = 0; i < datum->Size(); ++i)
		{
			Action* currentAction = static_cast<Action*>(datum->Get<Scope*>(i));
			worldState.Action = currentAction;
			currentAction->Update(worldState);
		}
		worldState.Action = nullptr;
	}

	void Entity::Clear()
	{
		sPerscribedAttributes.Clear();
	}
}