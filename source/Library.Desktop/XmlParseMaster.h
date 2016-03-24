#pragma once
namespace Library
{
	class IXmlParseHelper;
	//! Master Handler Class
	/*! The master handler in this chain of responsibility pattern. Contains
	a list of helper handlers that process parsing tasks.*/
	class XmlParseMaster final
	{
	public:
		//! Shared Data Class
		/*! Represents the data that all helpers share with each other and with
		the master parser. This is a base class that will likely need to be extended
		for specific XML grammars.*/
		class SharedData : public RTTI
		{
			RTTI_DECLARATIONS(SharedData, RTTI)
		public:
			//! Default Constructor
			SharedData();

			//! "Virtual" Constructor
			/*! Creates a new SharedData object that has the same state
			as "this" Shared Data.
			\return Reference to the cloned SharedData.*/
			virtual SharedData* Clone();

			//! Set Parse Master
			/*! \param master Reference to an XmlParseMaster that owns this data.*/
			void SetXmlParseMaster(XmlParseMaster& master);

			//! Get Parse Master
			/*! \return A reference to an XmlParseMaster that owns this data.*/
			XmlParseMaster& GetXmlParseMaster();

			//! Increment Depth
			/*! Incremented on the start of each element. Used to keep track
			of what nested element you are at within the file.*/
			void IncrementDepth();

			//! Increment Depth
			/*! Decremented on the end of each element. Used to keep track
			of what nested element you are at within the file.*/
			void DecrementDepth();

			//! Get Nested Depth
			/*! \return A counter to the current nesting depth.*/
			std::uint32_t Depth() const;

			//! Resets any data members that this class uses.
			virtual void Reset() {};
		protected:
			std::uint32_t mDepth;		//!< Counter variable to keep track of nested depth.
			XmlParseMaster* mMaster;	//!< Pointer to the XmlParseMaster that owns this data.
		};

		//! Default Constructor
		/*! Initializes the parser.
		\param data The SharedData that this master owns.*/
		explicit XmlParseMaster(SharedData& data);

		XmlParseMaster(const XmlParseMaster& rhs) = delete;
		XmlParseMaster& operator=(const XmlParseMaster& rhs) = delete;

		//! Destructor
		/*! Will delete all helpers and data if this master is a clone. Also
		frees any memory associated with the parser.*/
		~XmlParseMaster();

		//! Clone Master Parser
		/*! \return A new XmlParseMaster object that contains clones of the helpers
		and shared data from the original. This returned object will be ready to
		parse a fresh file.*/
		XmlParseMaster* Clone();

		//! Add Helper Handler
		/*! Adds a reference to a helper handler for this chain of responsbility.*/
		void AddHelper(IXmlParseHelper& helper);

		//! Remove Helper Handler
		/*! Removes a helper handler from this chain of responsbility.*/
		void RemoveHelper(IXmlParseHelper& helper);

		//! Parse XML Data
		/*! \param buffer Character buffer for XML data
		\param size Number of characters in the buffer
		\param last Indicates if this is the last chunk for this file*/
		void Parse(const std::string& buffer, std::uint32_t size, bool last);

		//! Parse Entire File
		/*! Stringifies the contents of the XML file and then parses it. 
		\param filename The XML file to parse.*/
		void ParseFromFile(const std::string& filename);

		//! Resets the parser
		/*! This must be called either after calling parse with true, or after
		calling parse from file if you want to parse with this master again.*/
		void ResetParser();

		//! Get name of File being parsed
		/*! return name of the file being parsed.*/
		std::string GetFileName() const;

		//! Get the Shared Data that this master owns
		/*! \return A reference to the shared data.*/
		SharedData& GetSharedData();

		//! Set the Shared Data that this master owns
		/*! \param A reference to the shared data.*/
		void SetSharedData(SharedData& data);

	private:
		//! Callback function for Expat to handle XML start elements
		static void StartElementHandler(void* userData, const char* name, const char** atts);
		//! Callback function for Expat to handle XML end elements
		static void EndElementHandler(void* userData, const char* name);
		//! Callback function for Expat to handle XML character data
		static void CharDataHandler(void* userData, const XML_Char* buffer, int length);

		SharedData* mData;						//!< Pointer to the SharedData that this Parse Master owns
		XML_Parser mParser;						//!< Expat "object" to handle all of the parsing.
		Vector<IXmlParseHelper*> mHelpers;		//!< List of all helpers for this Parse Master's chain of responsibility.
		std::string mFilename;					//!< Name of the file currently being parsed by this handler
		bool mIsClone;							//!< Denotes if this Parse Master is a clone
	};
}

