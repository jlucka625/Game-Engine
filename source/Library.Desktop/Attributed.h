#pragma once
namespace Library
{
	//! Attributed Class
	/*! Serves as glue code for binding together the scripting language to native
	classes/objects. Helps provide a way for us to get C++ data via game engine
	configuration scripts.*/
	class Attributed : public Scope
	{
		RTTI_DECLARATIONS(Attributed, Scope)
	public:
		//! Default Constructor
		/*! Sets up a this pointer in the scope for "this" attribute.*/
		Attributed();

		//! Copy Constructor
		/*! Performs a deep-copy on the attributed class. This will also
		deep-copy the scope related contents.
		\param rhs The attributed object to copy*/
		Attributed(const Attributed& rhs);

		//! Move Constructor
		/*! Performs move semantics on the attributed class. This 
		will also move the scope related contents.
		\param rhs The attributed object to move*/
		Attributed(Attributed&& rhs);

		//! Destructor
		/*! Clears out all perscribed and auxiliary attributes.*/
		virtual ~Attributed();

		//! Copy Assignment Operator
		/*! Performs a deep-copy on the attributed class. This will also
		deep-copy the scope related contents.
		\param rhs The attributed object to copy
		\return The copied attributed object.*/
		Attributed& operator=(const Attributed& rhs);

		//! Move Constructor
		/*! Performs move semantics on the attributed class. This
		will also move the scope related contents.
		\param rhs The attributed object to move
		\return The moved attributed object.*/
		Attributed& operator=(Attributed&& rhs);

		//! Checks if the attribute is perscribed
		/*! \param name The name of the attribute
		\return If the attribute is perscribed.*/
		bool IsPrescribedAttribute(const std::string& name) const;

		//! Checks if the attribute is auxiliary
		/*! \param name The name of the attribute
		\return If the attribute is auxiliary.*/
		bool IsAuxiliaryAttribute(const std::string& name) const;

		//! Checks if the name referes to an attribute of this class
		/*! \param name The name of the attribute
		\return If the attribute is contained within this scope.*/
		bool IsAttributed(const std::string& name) const;

		//! Appends Auxiliary Attribute
		/*! \param name The name of the attribute to append.
		\return A reference to the Datum that contains the appended
		attribute.*/
		Datum& AppendAuxiliaryAttribute(const std::string& name);
		
		//! Adds an Attribute with Internal Storage
		/*! Constructs a name, Datum pair to add to this scope
		\param name The name of the attribute
		\param type The type of the datum for this attribute
		\param initialVale An initial scalar value to store within the Datum
		\param size The size of the Datum.*/
		void AddInternalAttribute(const std::string& name, int& initialValue, std::uint32_t size);
		void AddInternalAttribute(const std::string& name, float& initialValue, std::uint32_t size);
		void AddInternalAttribute(const std::string& name, std::string& initialValue, std::uint32_t size);
		void AddInternalAttribute(const std::string& name, glm::vec4& initialValue, std::uint32_t size);
		void AddInternalAttribute(const std::string& name, glm::mat4& initialValue, std::uint32_t size);
		void AddInternalAttribute(const std::string& name, RTTI* initialValue, std::uint32_t size);

		//! Adds an Attribute with External Storage
		/*! Constructs a name, Datum pair to add to this scope
		\param name The name of the attribute
		\param type The type of the datum for this attribute
		\param size The size of the Datum.
		\param storage An array of values that this Datum will store externally.*/
		void AddExternalAttribute(const std::string& name, std::uint32_t size, int* storage);
		void AddExternalAttribute(const std::string& name, std::uint32_t size, float* storage);
		void AddExternalAttribute(const std::string& name, std::uint32_t size, std::string* storage);
		void AddExternalAttribute(const std::string& name, std::uint32_t size, glm::vec4* storage);
		void AddExternalAttribute(const std::string& name, std::uint32_t size, glm::mat4* storage);
		void AddExternalAttribute(const std::string& name, std::uint32_t size, RTTI** storage);

		//! Adds a Scope Attribute
		/*! \param name The name of the attribute
		\param child A pointer to the scope to nest within this scope.*/
		void AddNestedScope(std::string name, Scope& child);

	protected:
		template <typename T>
		void InitializeInternalAttribute(const std::string& name, Datum::DatumType type, T& initialValue, std::uint32_t size);
		
		template <typename T>
		void InitializeExternalAttribute(const std::string& name, Datum::DatumType type, T* storage, std::uint32_t size);

		virtual Vector<std::string>& PrescribedAttributes() const = 0;
	};

	template <typename T>
	void Attributed::InitializeInternalAttribute(const std::string& name, Datum::DatumType type, T& initialValue, std::uint32_t size)
	{
		Datum& datum = Append(name);
		datum.SetType(type);
		PrescribedAttributes().PushBack(name);
		for (std::uint32_t i = 0; i < size; i++)
		{
			datum.PushBack(initialValue);
		}
	}

	template <typename T>
	void Attributed::InitializeExternalAttribute(const std::string& name, Datum::DatumType type, T* storage, std::uint32_t size)
	{
		Datum& datum = Append(name);
		datum.SetType(type);
		PrescribedAttributes().PushBack(name);
		datum.SetStorage(storage, size);
	}
}

