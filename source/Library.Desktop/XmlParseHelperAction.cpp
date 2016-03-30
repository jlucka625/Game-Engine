#include "pch.h"

namespace Library
{
	XmlParseHelperAction::XmlParseHelperAction()
	{
		mParseFunctions["Action"] = &XmlParseHelperAction::ParseActionAttribute;
		mParseFunctions["Condition"] = &XmlParseHelperAction::ParseConditionAttribute;
		mParseFunctions["Then"] = &XmlParseHelperAction::ParseThenAttribute;
		mParseFunctions["Else"] = &XmlParseHelperAction::ParseElseAttribute;
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
		if (!&sharedData || (name != "Action" &&
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
		int value = 0;

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
		}

		Scope*& scope = userData.GetScope();
		Entity* entity = static_cast<Entity*>(scope);
		Action* action = entity->CreateAction(className, instanceName);
		scope = action;
		
		Datum* datum = action->Find(Action::targetKey);
		if (datum) { datum->Set(targetName); }

		datum = action->Find(Action::prototypeKey);
		if (datum) { datum->Set(prototypeName); }

		datum = action->Find(Action::valueKey);
		if (datum) { datum->Set(value); }

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
		ActionIf* listIf = static_cast<ActionIf*>(scope);
		Datum* datum = listIf->Find(ActionIf::conditionKey);
		if (!datum)
			throw std::exception("Condition attribute not found.");

		datum->Set(value);
	}

	void XmlParseHelperAction::ParseThenAttribute(XmlParseHelperTable::ScopeSharedData & userData, StringMap & data)
	{
		UNREFERENCED_PARAMETER(data);
		Scope*& scope = userData.GetScope();
		ActionIf* listIf = static_cast<ActionIf*>(scope);
		scope = &listIf->ThenList();
	}

	void XmlParseHelperAction::ParseElseAttribute(XmlParseHelperTable::ScopeSharedData & userData, StringMap & data)
	{
		UNREFERENCED_PARAMETER(data);
		Scope*& scope = userData.GetScope();
		ActionIf* listIf = static_cast<ActionIf*>(scope);
		scope = &listIf->ElseList();
	}
#pragma endregion

}

