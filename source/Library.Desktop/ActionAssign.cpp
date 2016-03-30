#include "pch.h"

namespace Library
{
	RTTI_DEFINITIONS(ActionAssign)
	Vector<std::string> ActionAssign::sPerscribedAssignAttributes;

	ActionAssign::ActionAssign() :
		mTarget(""), mValue(0)
	{
		InitializeAttributes();
	}

	void ActionAssign::Update(WorldState & worldState)
	{
		UNREFERENCED_PARAMETER(worldState);
		Datum* datum = parent->Find(Action::actionsKey);
		if (!datum)
			throw std::exception("Actions were not found within this scope.");

		for (std::uint32_t i = 0; i < datum->Size(); ++i)
		{
			ActionIf* action = static_cast<ActionIf*>(datum->Get<Scope*>(i));
			if (action->Name() == mTarget)
			{
				datum = action->Find(ActionIf::conditionKey);
				if (!datum)
					throw std::exception("Condition not found.");

				datum->Set(mValue);
			}
		}
	}

	void ActionAssign::Clear()
	{
		sPerscribedAssignAttributes.Clear();
	}

	Vector<std::string>& ActionAssign::PrescribedAttributes() const
	{
		return sPerscribedAssignAttributes;
	}

	void ActionAssign::InitializeAttributes()
	{
		AddExternalAttribute(Action::targetKey, 1, &mTarget);
		AddExternalAttribute(Action::valueKey, 1, &mValue);
	}
}

