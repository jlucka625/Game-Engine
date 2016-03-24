#include "pch.h"
namespace Library
{

#pragma region Scope Shared Data
	RTTI_DEFINITIONS(XmlParseHelperTable::ScopeSharedData)
	XmlParseHelperTable::ScopeSharedData::ScopeSharedData() :
		mScope(new Scope())/*, mScopes()*/
	{
		//PushDown(std::string("Base"), &mScope);
	}
	
	XmlParseHelperTable::ScopeSharedData::~ScopeSharedData()
	{
		delete mScope;
	}

	XmlParseMaster::SharedData* XmlParseHelperTable::ScopeSharedData::Clone()
	{
		return new ScopeSharedData();
	}

	Scope*& XmlParseHelperTable::ScopeSharedData::GetScope()
	{
		return mScope;
	}

	/*Stack<XmlParseHelperTable::PairType>& XmlParseHelperTable::ScopeSharedData::GetScopes()
	{
		return mScopes;
	}*/

	/*void XmlParseHelperTable::ScopeSharedData::PushDown(std::string& name, Scope* scope)
	{
		mScopes.Push(std::make_pair(name, scope));
	}*/

	void XmlParseHelperTable::ScopeSharedData::Reset()
	{
		mScope->Clear();
		//mScopes.Clear();
		//PushDown(std::string("Base"), &mScope);
	}

#pragma endregion

#pragma region Specific Type Parse Helper Methods
	void XmlParseHelperTable::ParseIntegerAttribute(ScopeSharedData& userData, StringMap & data)
	{
		int value = 0;
		std::string attributeName = "default";
		std::uint32_t size = 1;

		for (auto& pair : data)
		{
			if (pair.first == "name")
			{
				attributeName = pair.second;
			}
			else if (pair.first == "value")
			{
				value = std::stoi(pair.second);
			}
			else if (pair.first == "size")
			{
				size = std::stoi(pair.second);
			}
		}

		//Scope* currentScope = userData.GetScopes().Top().second;
		Scope* scope = userData.GetScope();
		Datum& datum = scope->Append(attributeName);
		for (std::uint32_t i = 0; i < size; ++i)
		{
			datum.PushBack(value);
		}
	}

	void XmlParseHelperTable::ParseFloatAttribute(ScopeSharedData& userData, StringMap & data)
	{
		float value = 0.0f;
		std::string attributeName = "default";
		std::uint32_t size = 1;

		for (auto& pair : data)
		{
			if (pair.first == "name")
			{
				attributeName = pair.second;
			}
			else if (pair.first == "value")
			{
				value = std::stof(pair.second);
			}
			else if (pair.first == "size")
			{
				size = std::stoi(pair.second);
			}
		}

		Scope* scope = userData.GetScope();
		Datum& datum = scope->Append(attributeName);
		for (std::uint32_t i = 0; i < size; ++i)
		{
			datum.PushBack(value);
		}
	}

	void XmlParseHelperTable::ParseStringAttribute(ScopeSharedData& userData, StringMap & data)
	{
		std::string value = "";
		std::string attributeName = "default";
		std::uint32_t size = 1;

		for (auto& pair : data)
		{
			if (pair.first == "name")
			{
				attributeName = pair.second;
			}
			else if (pair.first == "value")
			{
				value = pair.second;
			}
			else if (pair.first == "size")
			{
				size = std::stoi(pair.second);
			}
		}

		Scope* scope = userData.GetScope();
		Datum& datum = scope->Append(attributeName);
		for (std::uint32_t i = 0; i < size; ++i)
		{
			datum.PushBack(value);
		}
	}

	void XmlParseHelperTable::ParseVectorAttribute(ScopeSharedData& userData, StringMap & data)
	{
		std::string vectorAsString = "";
		std::string attributeName = "default";
		std::uint32_t size = 1;

		for (auto& pair : data)
		{
			if (pair.first == "name")
			{
				attributeName = pair.second;
			}
			else if (pair.first == "value")
			{
				vectorAsString = pair.second;
			}
			else if (pair.first == "size")
			{
				size = std::stoi(pair.second);
			}
		}

		Scope* scope = userData.GetScope();
		Datum& datum = scope->Append(attributeName);
		for (std::uint32_t i = 0; i < size; ++i)
		{
			datum.PushBack(glm::vec4(0.0f));
			datum.SetFromString(const_cast<char*>(vectorAsString.c_str()), Datum::DatumType::Vector, i);
		}
	}

	void XmlParseHelperTable::ParseMatrixAttribute(ScopeSharedData& userData, StringMap & data)
	{
		std::string matrixAsString = "";
		std::string attributeName = "default";
		std::uint32_t size = 1;

		for (auto& pair : data)
		{
			if (pair.first == "name")
			{
				attributeName = pair.second;
			}
			else if (pair.first == "value")
			{
				matrixAsString = pair.second;
			}
			else if (pair.first == "size")
			{
				size = std::stoi(pair.second);
			}
		}

		Scope* scope = userData.GetScope();
		Datum& datum = scope->Append(attributeName);
		for (std::uint32_t i = 0; i < size; ++i)
		{
			datum.PushBack(glm::mat4(0.0f));
			datum.SetFromString(const_cast<char*>(matrixAsString.c_str()), Datum::DatumType::Matrix, i);
		}
	}

	void XmlParseHelperTable::ParseTableAttribute(ScopeSharedData& userData, StringMap & data)
	{
		std::string attributeName = "default";

		for (auto& pair : data)
		{
			if (pair.first == "name")
			{
				attributeName = pair.second;
			}
		}

		Scope*& scope = userData.GetScope();
		Scope& childScope = scope->AppendScope(attributeName);
		scope = &childScope;

		//Scope* scope = new Scope();
		//userData.PushDown(attributeName, scope);
	}

	void XmlParseHelperTable::ParseEntityAttribute(ScopeSharedData& userData, StringMap & data)
	{
		std::string className = "default";
		std::string instanceName = "default";

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
		}

		Scope*& scope = userData.GetScope();
		Sector* sector = scope->As<Sector>();
		if (!sector)
			throw std::exception("Scope is not a Sector");

		Entity* entity = sector->CreateEntity(className, instanceName);
		scope = entity;
	}

	void XmlParseHelperTable::ParseSectorAttribute(ScopeSharedData& userData, StringMap & data)
	{
		std::string attributeName = "default";

		for (auto& pair : data)
		{
			if (pair.first == "name")
			{
				attributeName = pair.second;
			}
		}

		Scope*& scope = userData.GetScope();
		World* world = scope->As<World>();
		if (!world)
			throw std::exception("Scope is not a World");

		world->CreateSector(attributeName);
		scope = world;
	}

	void XmlParseHelperTable::ParseWorldAttribute(ScopeSharedData& userData, StringMap & data)
	{
		std::string attributeName = "default";

		for (auto& pair : data)
		{
			if (pair.first == "name")
			{
				attributeName = pair.second;
			}
		}

		Scope*& scope = userData.GetScope();
		World* world = new World();
		scope->Adopt(*world, attributeName);
		scope = world;
	}
#pragma endregion

#pragma region XML Parse Helper Table
	XmlParseHelperTable::XmlParseHelperTable() :
		mParseFunctions()
	{
		mParseFunctions["Integer"] = &XmlParseHelperTable::ParseIntegerAttribute;
		mParseFunctions["Float"] = &XmlParseHelperTable::ParseFloatAttribute;
		mParseFunctions["String"] = &XmlParseHelperTable::ParseStringAttribute;
		mParseFunctions["Vector"] = &XmlParseHelperTable::ParseVectorAttribute;
		mParseFunctions["Matrix"] = &XmlParseHelperTable::ParseMatrixAttribute;
		mParseFunctions["Table"] = &XmlParseHelperTable::ParseTableAttribute;
		mParseFunctions["Entity"] = &XmlParseHelperTable::ParseEntityAttribute;
		mParseFunctions["Sector"] = &XmlParseHelperTable::ParseSectorAttribute;
		mParseFunctions["World"] = &XmlParseHelperTable::ParseWorldAttribute;
	}

	bool XmlParseHelperTable::StartElementHandler(XmlParseMaster::SharedData & userData, const std::string & name, StringMap & data)
	{
		ScopeSharedData& sharedData = *userData.As<ScopeSharedData>();
		if (!&sharedData)
				return false;

		ParseMap::Iterator it = mParseFunctions.Find(name);
		(this->*(it->second))(sharedData, data);
		return true;
	}

	bool XmlParseHelperTable::EndElementHandler(XmlParseMaster::SharedData & userData, const std::string & name)
	{
		ScopeSharedData& sharedData = *userData.As<ScopeSharedData>();
		if (!&sharedData || (name != "Table" && name != "Entity" &&
			name != "Sector" && name != "World"))
				return false;

		Scope*& scope = sharedData.GetScope();
		scope = scope->GetParent();
		//PairType pair = sharedData.GetScopes().Top();
		//sharedData.GetScopes().Pop();
		//sharedData.GetScopes().Top().second->Adopt(*(pair.second), pair.first);
		return true;
	}

	IXmlParseHelper * XmlParseHelperTable::Clone()
	{
		return new XmlParseHelperTable();
	}

#pragma endregion

}

