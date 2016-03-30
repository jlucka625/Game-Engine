#pragma once
namespace Library
{
	typedef HashMap<std::string, std::string> StringMap;
	//! Helper parser for Actions
	/*! Monolithic helper that parses XML and constructs an
	Action from the grammar provided.*/
	class XmlParseHelperAction : public IXmlParseHelper
	{
	public:
		typedef std::pair<std::string, Scope*> PairType;
		typedef void(XmlParseHelperAction::*ParseFunction)(XmlParseHelperTable::ScopeSharedData&, StringMap&);

		//Default Constructor
		XmlParseHelperAction();

		//! Start Element Handler
		/*! Determines the type of data to add to the Scope based on the name
		of the tag, then parses that data and adds to it's containing Scope.
		\param userData The shared data used to build the scope
		\param name The name of the tag for this start element
		\param data A hashmap of any arguments provided to this element
		\return If the handler can parse this data.*/
		virtual bool StartElementHandler(XmlParseMaster::SharedData& userData, const std::string& name, StringMap& data) override;

		//! End Element Handler
		/*! This only applies when an end element was reached and the tag
		represents type table. It takes nested Scope that was just parsed
		and appends it to the base Scope object.
		\param userData The shared data used to build the scope
		\param name The name of the tag for this end element
		\return If the handler can parse this data.*/
		virtual bool EndElementHandler(XmlParseMaster::SharedData& userData, const std::string& name) override;

		//! "Virtual" Constructor
		/*! \return A reference to a duplicate of this helper.*/
		virtual IXmlParseHelper* Clone() override;

		//Default Destructor
		virtual ~XmlParseHelperAction() = default;

	private:
		//! Specific Element Parse Helper Method
		/* Parses based on specific element and adds it to its contained Scope.
		\param userData The shared data used to build the scope
		\param data A hashmap of any arguments provided to this element*/
		void ParseActionAttribute(XmlParseHelperTable::ScopeSharedData& userData, StringMap& data);
		void ParseConditionAttribute(XmlParseHelperTable::ScopeSharedData& userData, StringMap& data);
		void ParseThenAttribute(XmlParseHelperTable::ScopeSharedData& userData, StringMap& data);
		void ParseElseAttribute(XmlParseHelperTable::ScopeSharedData& userData, StringMap& data);

		typedef HashMap<std::string, ParseFunction> ParseMap;
		ParseMap mParseFunctions;			//!< A HashMap of function pointers to each Element Parse Helper
	};
}


