#pragma once
class StringSharedData : public XmlParseMaster::SharedData
{
	RTTI_DECLARATIONS(StringSharedData, XmlParseMaster::SharedData)
public:
	StringSharedData();
	virtual XmlParseMaster::SharedData* Clone() override;
	void AppendToString(std::string append);
	void SetString(std::string xmlString);
	std::string GetString();
	virtual void Reset() override;
protected:
	std::string mXmlString;
};

