#include "pch.h"

namespace Library
{
	RTTI_DEFINITIONS(ActionDestroyAction)
	Vector<std::string> ActionDestroyAction::sPerscribedDestroyAttributes;

	ActionDestroyAction::ActionDestroyAction() :
		mTarget("")
	{
		InitializeAttributes();
	}

	void ActionDestroyAction::Update(WorldState & worldState)
	{
		UNREFERENCED_PARAMETER(worldState);
		Datum* datum = parent->Find(Action::actionsKey);
		if (!datum)
			throw std::exception("Actions were not found within this scope.");

		for (std::uint32_t i = 0; i < datum->Size(); ++i)
		{
			Action* action = static_cast<Action*>(datum->Get<Scope*>(i));
			if (action->Name() == mTarget)
			{
				datum->RemoveAt(i);
				delete action;
				break;
			}
		}
	}

	void ActionDestroyAction::Clear()
	{
		sPerscribedDestroyAttributes.Clear();
	}

	Vector<std::string>& ActionDestroyAction::PrescribedAttributes() const
	{
		return sPerscribedDestroyAttributes;
	}

	void ActionDestroyAction::InitializeAttributes()
	{
		AddExternalAttribute(Action::targetKey, 1, &mTarget);
	}
}
