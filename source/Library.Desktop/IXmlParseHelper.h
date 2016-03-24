#pragma once
#include "XmlParseMaster.h"
namespace Library 
{
	//! Abstract base class for XML Helper Handlers
	class IXmlParseHelper
	{
	public:
		typedef HashMap<std::string, std::string> StringMap;
		//! Initialize Helper Data
		/*! This gets called just before a file gets parsed. Will likely need to be implemented
		in derived helper handlers.*/
		virtual void Initialize() {};

		//! Handler for when the parser starts parsing an element
		/*! \param The SharedData owned by this helper's master.
		\param name The element name
		\param data A hash map of key, value pairs for each element
		\return If this helper can handle the parsed data or not.*/
		virtual bool StartElementHandler(XmlParseMaster::SharedData& userData, const std::string& name, StringMap& data) = 0;
		
		//! Handler for when the parser ends parsing an element
		/*! \param The SharedData owned by this helper's master.
		\param name The element name
		\return If this helper can handle the parsed data or not.*/
		virtual bool EndElementHandler(XmlParseMaster::SharedData& userData, const std::string& name) = 0;

		//! Handler for when the parser is trying to parse character data
		/*! \param The SharedData owned by this helper's master.
		\param buffer The parsed character data
		\param length The length of the parsed character data
		\return If this helper can handle the parsed data or not.*/
		virtual bool CharDataHandler(XmlParseMaster::SharedData& userData, const std::string& buffer);

		//! "Virtual" Constructor
		/*! \return A reference to a duplicate of this helper.*/
		virtual IXmlParseHelper* Clone() = 0;

		//! Default Destructor
		virtual ~IXmlParseHelper() = default;
	};
}

