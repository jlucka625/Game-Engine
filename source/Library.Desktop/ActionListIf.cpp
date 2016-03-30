#include "pch.h"

namespace Library
{
	RTTI_DEFINITIONS(ActionIf)

	Vector<std::string> ActionIf::sPerscribedIfAttributes;
	const std::string ActionIf::conditionKey = "Condition";
	const std::string ActionIf::thenKey = "Then";
	const std::string ActionIf::elseKey = "Else";

	ActionIf::ActionIf() :
		mCondition(0)
	{
		InitializeAttributes();
	}

	void ActionIf::Update(WorldState & worldState)
	{
		if (mCondition)
		{
			worldState.Action = &mThenList;
			mThenList.Update(worldState);
		}
		else
		{
			worldState.Action = &mElseList;
			mElseList.Update(worldState);
		}

		worldState.Action = nullptr;
	}

	void ActionIf::Clear()
	{
		sPerscribedIfAttributes.Clear();
	}

	Vector<std::string>& ActionIf::PrescribedAttributes() const
	{
		return sPerscribedIfAttributes;
	}

	ActionList& ActionIf::ThenList()
	{
		return mThenList;
	}

	ActionList& ActionIf::ElseList()
	{
		return mElseList;
	}

	void ActionIf::InitializeAttributes()
	{
		AddExternalAttribute(conditionKey, 1, &mCondition);
		AddNestedScope(ActionIf::thenKey, mThenList);
		AddNestedScope(ActionIf::elseKey, mElseList);
	}
}

