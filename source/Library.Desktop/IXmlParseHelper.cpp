#include "pch.h"

namespace Library
{
	bool IXmlParseHelper::CharDataHandler(XmlParseMaster::SharedData& userData, const std::string& buffer)
	{
		UNREFERENCED_PARAMETER(userData);
		UNREFERENCED_PARAMETER(buffer);
		return false;
	}
}
