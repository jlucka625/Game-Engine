#pragma once

using namespace Library;

class TestHelper : public IXmlParseHelper
{
public:
	typedef HashMap<std::string, std::string> StringMap;
	virtual bool StartElementHandler(XmlParseMaster::SharedData& userData, const std::string& name, StringMap& data) override;
	virtual bool EndElementHandler(XmlParseMaster::SharedData& userData, const std::string& name) override;
	virtual bool CharDataHandler(XmlParseMaster::SharedData& userData, const std::string& buffer) override;
	virtual IXmlParseHelper* Clone() override;
	virtual ~TestHelper();
};

