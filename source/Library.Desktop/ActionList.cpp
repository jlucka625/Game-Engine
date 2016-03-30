#include "pch.h"

namespace Library
{
	RTTI_DEFINITIONS(ActionList)

	Vector<std::string> ActionList::sPerscribedListAttributes;

	void ActionList::Update(WorldState & worldState)
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

	void ActionList::Clear()
	{
		sPerscribedListAttributes.Clear();
	}

	Vector<std::string>& ActionList::PrescribedAttributes() const
	{
		return sPerscribedListAttributes;
	}

	void ActionList::InitializeAttributes()
	{
		Datum& datum = Append(actionsKey);
		datum.SetType(Datum::DatumType::Table);
		sPerscribedListAttributes.PushBack(actionsKey);
	}
}

