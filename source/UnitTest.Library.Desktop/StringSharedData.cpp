#include "pch.h"

using namespace Library;

RTTI_DEFINITIONS(StringSharedData)

StringSharedData::StringSharedData() :
	mXmlString("")
{}

XmlParseMaster::SharedData* StringSharedData::Clone()
{
	return new StringSharedData();
}

void StringSharedData::AppendToString(std::string append)
{
	mXmlString += append;
}

void StringSharedData::SetString(std::string xmlString)
{
	mXmlString = xmlString;
}

std::string StringSharedData::GetString()
{
	return mXmlString;
}

void StringSharedData::Reset()
{
	SetString("");
}


