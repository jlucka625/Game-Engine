#pragma once
namespace Library
{
	typedef HashMap<std::string, std::string> StringMap;
	//! Helper parser for Scopes
	/*! Monolithic helper that parses XML and constructs a
	Scope from the grammar provided.*/
	class XmlParseHelperTable : public IXmlParseHelper
	{
	public:
		typedef std::pair<std::string, Scope*> PairType;
		//! Shared Data that contains a Scope
		/*! The data that the parse master and it's helpers
		use in order to construct the Scope object.*/
		class ScopeSharedData : public XmlParseMaster::SharedData
		{
			RTTI_DECLARATIONS(ScopeSharedData, XmlParseMaster::SharedData)
		public:
			//! Default Constructor
			ScopeSharedData();

			//! Destructor
			~ScopeSharedData();

			//! "Virtual" Constructor
			/*! Creates a new SharedData object that has the same state
			as "this" Shared Data.
			\return Reference to the cloned SharedData.*/
			virtual XmlParseMaster::SharedData* Clone() override;

			//! Resets the data to an empty Scope.
			virtual void Reset() override;

			//! Get the currently constructed Scope.
			/*! return A reference to the currently constructed Scope.*/
			Scope*& GetScope();

			//! Get the nested Scopes in order of depth
			/*! \return A stack containing all of the nested scopes that 
			will be placed within our base Scope.*/
			//Stack<PairType>& GetScopes();

			//! Push Down a nested Scope
			/*! Adds a nested Scope to let us know what Scope to currently
			add data to.*/
			//void PushDown(std::string& name, Scope* scope);
		protected:
			Scope* mScope;					//!< The Scope we are building from the XML
			//Stack<PairType> mScopes;		//!< The nested scopes that we are adding to the base Scope
		};

		typedef void(XmlParseHelperTable::*ParseFunction)(ScopeSharedData&, StringMap&);

		//Default Constructor
		XmlParseHelperTable();

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
		virtual ~XmlParseHelperTable() = default;

	private:
		//! Specific Element Parse Helper Method
		/* Parses based on specific element and adds it to its contained Scope.
		\param userData The shared data used to build the scope
		\param data A hashmap of any arguments provided to this element*/
		void ParseIntegerAttribute(ScopeSharedData& userData, StringMap& data);
		void ParseFloatAttribute(ScopeSharedData& userData, StringMap& data);
		void ParseStringAttribute(ScopeSharedData& userData, StringMap& data);
		void ParseVectorAttribute(ScopeSharedData& userData, StringMap& data);
		void ParseMatrixAttribute(ScopeSharedData& userData, StringMap& data);
		void ParseTableAttribute(ScopeSharedData& userData, StringMap& data);
		void ParseEntityAttribute(ScopeSharedData& userData, StringMap& data);
		void ParseSectorAttribute(ScopeSharedData& userData, StringMap& data);
		void ParseWorldAttribute(ScopeSharedData& userData, StringMap& data);

		typedef HashMap<std::string, ParseFunction> ParseMap;
		ParseMap mParseFunctions;			//!< A HashMap of function pointers to each Element Parse Helper
	};
}

