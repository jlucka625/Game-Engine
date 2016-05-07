#include "pch.h"

namespace Library
{
	XmlParseHelperAction::XmlParseHelperAction()
	{
		mParseFunctions["Action"] = &XmlParseHelperAction::ParseActionAttribute;
		mParseFunctions["Condition"] = &XmlParseHelperAction::ParseConditionAttribute;
		mParseFunctions["Then"] = &XmlParseHelperAction::ParseThenAttribute;
		mParseFunctions["Else"] = &XmlParseHelperAction::ParseElseAttribute;
		mParseFunctions["Reaction"] = &XmlParseHelperAction::ParseReactionAttribute;
	}

	bool XmlParseHelperAction::StartElementHandler(XmlParseMaster::SharedData & userData, const std::string & name, StringMap & data)
	{
		XmlParseHelperTable::ScopeSharedData& sharedData = *userData.As<XmlParseHelperTable::ScopeSharedData>();
		if (!&sharedData || !mParseFunctions.ContainsKey(name))
			return false;

		ParseMap::Iterator it = mParseFunctions.Find(name);
		(this->*(it->second))(sharedData, data);
		return true;
	}

	bool XmlParseHelperAction::EndElementHandler(XmlParseMaster::SharedData & userData, const std::string & name)
	{
		XmlParseHelperTable::ScopeSharedData& sharedData = *userData.As<XmlParseHelperTable::ScopeSharedData>();
		if (!&sharedData || (name != "Action" && name != "Reaction" &&
			name != "Then" && name != "Else"))
			return false;

		Scope*& scope = sharedData.GetScope();
		scope = scope->GetParent();
		return true;
	}

	IXmlParseHelper * XmlParseHelperAction::Clone()
	{
		return new XmlParseHelperAction;
	}

#pragma region Specific Type Parse Helper Methods
	void XmlParseHelperAction::ParseActionAttribute(XmlParseHelperTable::ScopeSharedData & userData, StringMap & data)
	{
		std::string className = "default";
		std::string instanceName = "default";
		std::string targetName = "default";
		std::string prototypeName = "default";
		std::string subtype = "default";
		int value = -1;
		int delay = -1;

		for (auto& pair : data)
		{
			if (pair.first == "name")
			{
				instanceName = pair.second;
			}
			else if (pair.first == "class")
			{
				className = pair.second;
			}
			else if (pair.first == Action::targetKey)
			{
				targetName = pair.second;
			}
			else if (pair.first == Action::prototypeKey)
			{
				prototypeName = pair.second;
			}
			else if (pair.first == Action::valueKey)
			{
				value = std::stoi(pair.second);
			}
			else if (pair.first == ReactionAttributed::subtypeKey)
			{
				subtype = pair.second;
			}
			else if (pair.first == ActionEvent::delayKey)
			{
				delay = std::stoi(pair.second);
			}
		}

		Scope*& scope = userData.GetScope();
		
		Action* action = nullptr;
		Entity* entity = scope->As<Entity>();
		if (entity)
		{
			action = entity->CreateAction(className, instanceName);
		}
		else
		{
			ActionList* actionList = scope->As<ActionList>();
			if (actionList)
			{
				action = Factory<Action>::Create(className);
				actionList->Adopt(*action, Action::actionsKey);
				action->SetName(instanceName);
			}
			else throw std::exception("Scope is neither an entity or action list.");
		}
		scope = action;
		
		Datum* datum = action->Find(Action::targetKey);
		if (datum) { datum->Set(targetName); }

		datum = action->Find(Action::prototypeKey);
		if (datum) { datum->Set(prototypeName); }

		datum = action->Find(Action::valueKey);
		if (datum) { datum->Set(value); }

		datum = action->Find(ActionEvent::delayKey);
		if (datum) { datum->Set(delay); }

		datum = action->Find(ReactionAttributed::subtypeKey);
		if (datum) { datum->Set(subtype); }

	}

	void XmlParseHelperAction::ParseConditionAttribute(XmlParseHelperTable::ScopeSharedData & userData, StringMap & data)
	{
		int value = -1;
		for (auto& pair : data)
		{
			if (pair.first == "value")
			{
				value = std::stoi(pair.second);
			}
		}
		Scope*& scope = userData.GetScope();
		assert(scope->Is("ActionIf"));
		ActionIf* listIf = static_cast<ActionIf*>(scope);
		Datum* datum = listIf->Find(ActionIf::conditionKey);
		if (!datum)
			throw std::exception("Condition attribute not found.");

		datum->Set(value);
	}

	void XmlParseHelperAction::ParseReactionAttribute(XmlParseHelperTable::ScopeSharedData & userData, StringMap & data)
	{
		std::string className = "default";
		std::string instanceName = "default";
		std::string subtype = "default";

		for (auto& pair : data)
		{
			if (pair.first == "name")
			{
				instanceName = pair.second;
			}
			else if (pair.first == "class")
			{
				className = pair.second;
			}
			else if (pair.first == ReactionAttributed::subtypeKey)
			{
				subtype = pair.second;
			}
		}

		Scope*& scope = userData.GetScope();
		Reaction* reaction = Factory<Reaction>::Create(className);
		scope->Adopt(*reaction, instanceName);
		scope = reaction;

		Event<EventMessageAttributed>::Subscribe(*reaction);

		Datum* datum = reaction->Find(ReactionAttributed::subtypeKey);
		if (datum) { datum->Set(subtype); }
	}

	void XmlParseHelperAction::ParseThenAttribute(XmlParseHelperTable::ScopeSharedData & userData, StringMap & data)
	{
		UNREFERENCED_PARAMETER(data);
		Scope*& scope = userData.GetScope();
		assert(scope->Is("ActionIf"));
		ActionIf* listIf = static_cast<ActionIf*>(scope);
		scope = &listIf->ThenList();
	}

	void XmlParseHelperAction::ParseElseAttribute(XmlParseHelperTable::ScopeSharedData & userData, StringMap & data)
	{
		UNREFERENCED_PARAMETER(data);
		Scope*& scope = userData.GetScope();
		assert(scope->Is("ActionIf"));
		ActionIf* listIf = static_cast<ActionIf*>(scope);
		scope = &listIf->ElseList();
	}
#pragma endregion

}

