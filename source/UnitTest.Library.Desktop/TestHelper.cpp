#include "pch.h"

bool TestHelper::StartElementHandler(XmlParseMaster::SharedData& userData, const std::string & name, StringMap & data)
{
	if (!userData.Is("StringSharedData"))
		return false;
	
	StringSharedData& outputData = *userData.As<StringSharedData>();

	outputData.AppendToString("<" + name);

	for (auto& pair : data)
	{
		outputData.AppendToString(" " + pair.first + "=\"" + pair.second + "\"");
	}

	outputData.AppendToString(">");
	return true;
}

bool TestHelper::EndElementHandler(XmlParseMaster::SharedData& userData, const std::string & name)
{
	if (!userData.Is("StringSharedData"))
		return false;

	StringSharedData& outputData = *userData.As<StringSharedData>();

	outputData.AppendToString("</" + name + ">");
	return true;
}

bool TestHelper::CharDataHandler(XmlParseMaster::SharedData& userData, const std::string & buffer)
{
	if (!userData.Is("StringSharedData"))
		return false;

	StringSharedData& outputData = *userData.As<StringSharedData>();

	outputData.AppendToString(buffer);
	return true;
}

IXmlParseHelper* TestHelper::Clone()
{
	return new TestHelper();
}

TestHelper::~TestHelper()
{
}


