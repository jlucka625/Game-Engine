#include "pch.h"

namespace Library
{

#pragma region Shared Data

	RTTI_DEFINITIONS(XmlParseMaster::SharedData)

	XmlParseMaster::SharedData::SharedData() :
		mDepth(0), mMaster(nullptr)
	{}

	XmlParseMaster::SharedData* XmlParseMaster::SharedData::Clone()
	{
		SharedData* newData = new SharedData();
		return newData;
	}

	void XmlParseMaster::SharedData::SetXmlParseMaster(XmlParseMaster& master)
	{
		mMaster = &master;
	}

	XmlParseMaster& XmlParseMaster::SharedData::GetXmlParseMaster()
	{
		return *mMaster;
	}

	void XmlParseMaster::SharedData::IncrementDepth()
	{
		++mDepth;
	}

	void XmlParseMaster::SharedData::DecrementDepth()
	{
		if(mDepth > 0)
			--mDepth;
	}

	std::uint32_t XmlParseMaster::SharedData::Depth() const
	{
		return mDepth;
	}

#pragma endregion

#pragma region XML Parse Master
	XmlParseMaster::XmlParseMaster(SharedData& data) :
		mData(&data), mHelpers(), mIsClone(false), mFilename("")
	{
		mData = &data;
		mData->SetXmlParseMaster(*this);

		mParser = XML_ParserCreate(NULL);
		XML_SetUserData(mParser, mData);
		XML_SetElementHandler(mParser, StartElementHandler, EndElementHandler);
		XML_SetCharacterDataHandler(mParser, CharDataHandler);
	}


	XmlParseMaster::~XmlParseMaster()
	{
		XML_ParserFree(mParser);
		if (mIsClone)
		{
			for(auto& helper : mHelpers)
			{
				delete helper;
			}
			delete mData;
		}
	}

	XmlParseMaster* XmlParseMaster::Clone()
	{
		XmlParseMaster* newParseMaster = new XmlParseMaster(*mData->Clone());
		newParseMaster->mData->SetXmlParseMaster(*newParseMaster);
		newParseMaster->mIsClone = true;
		for (auto& helper : mHelpers)
		{
			newParseMaster->mHelpers.PushBack(helper->Clone());
		}
		return newParseMaster;
	}

	void XmlParseMaster::AddHelper(IXmlParseHelper& helper)
	{
		if (mIsClone)
			throw std::exception("Cannot add more helpers to a clone.");

		mHelpers.PushBack(&helper);
	}

	void XmlParseMaster::RemoveHelper(IXmlParseHelper& helper)
	{
		mHelpers.Remove(&helper);
	}

	void XmlParseMaster::Parse(const std::string& buffer, std::uint32_t size, bool last)
	{
		XML_Parse(mParser, buffer.c_str(), size, last);
	}

	void XmlParseMaster::ParseFromFile(const std::string& filename)
	{
		mFilename = filename;
		std::ifstream stream(filename);
		std::stringstream buffer;
		buffer << stream.rdbuf();
		Parse(buffer.str(), static_cast<std::uint32_t>(buffer.str().size()), true);
	}

	std::string XmlParseMaster::GetFileName() const
	{
		return mFilename;
	}

	XmlParseMaster::SharedData& XmlParseMaster::GetSharedData()
	{
		return *mData;
	}

	void XmlParseMaster::SetSharedData(SharedData& data)
	{
		if (mIsClone)
			delete mData;

		mData = &data;
		mData->SetXmlParseMaster(*this);
	}

	typedef HashMap<std::string, std::string> StringMap;
	typedef std::pair<std::string, std::string> StringPair;
	void XmlParseMaster::StartElementHandler(void* userData, const char* name, const char** atts)
	{
		SharedData* sharedData = reinterpret_cast<SharedData*>(userData);
		XmlParseMaster& master = sharedData->GetXmlParseMaster();

		StringMap map;
		while (*atts)
		{
			map.Insert(StringPair(atts[0], atts[1]));
			atts += 2;
		}

		sharedData->IncrementDepth();

		for (auto& helper : master.mHelpers)
		{
			if(helper->StartElementHandler(*sharedData, name, map))
			{
				break;
			}
		}
	}

	void XmlParseMaster::EndElementHandler(void* userData, const char* name)
	{
		SharedData* sharedData = reinterpret_cast<SharedData*>(userData);
		XmlParseMaster& master = sharedData->GetXmlParseMaster();

		for (auto& helper : master.mHelpers)
		{
			if (helper->EndElementHandler(*sharedData, name))
			{
				break;
			}
		}
		sharedData->DecrementDepth();
	}

	void XmlParseMaster::ResetParser()
	{
		for (auto& helper : mHelpers)
			helper->Initialize();

		XML_ParserReset(mParser, NULL);
		mData->Reset();
		XML_SetUserData(mParser, mData);
		XML_SetElementHandler(mParser, StartElementHandler, EndElementHandler);
		XML_SetCharacterDataHandler(mParser, CharDataHandler);
	}

	void XmlParseMaster::CharDataHandler(void* userData, const XML_Char* buffer, int length)
	{
		SharedData* sharedData = reinterpret_cast<SharedData*>(userData);
		XmlParseMaster& master = sharedData->GetXmlParseMaster();

		std::string bufferString(buffer, length);

		for (auto& helper : master.mHelpers)
		{
			if (helper->CharDataHandler(*sharedData, bufferString))
			{
				break;
			}
		}
	}
#pragma endregion

}

