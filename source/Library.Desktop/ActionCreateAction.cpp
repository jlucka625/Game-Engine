#include "pch.h"

namespace Library
{
	RTTI_DEFINITIONS(ActionCreateAction)
	Vector<std::string> ActionCreateAction::sPerscribedCreateAttributes;

	ActionCreateAction::ActionCreateAction() :
		mActionName(""), mPrototypeName("")
	{
		InitializeAttributes();
	}

	void ActionCreateAction::Update(WorldState & worldState)
	{
		UNREFERENCED_PARAMETER(worldState);
		Action* action = Factory<Action>::Create(mPrototypeName);
		parent->Adopt(*action, actionsKey);
		action->SetName(mActionName);
	}
	
	void ActionCreateAction::Clear()
	{
		sPerscribedCreateAttributes.Clear();
	}

	Vector<std::string>& ActionCreateAction::PrescribedAttributes() const
	{
		return sPerscribedCreateAttributes;
	}

	void ActionCreateAction::InitializeAttributes()
	{
		AddExternalAttribute(Action::targetKey, 1, &mActionName);
		AddExternalAttribute(Action::prototypeKey, 1, &mPrototypeName);
	}
}
