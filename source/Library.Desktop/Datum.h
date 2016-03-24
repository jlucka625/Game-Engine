#pragma once
namespace Library {
	class Scope;
	//! Datum Class
	/*! Represents a runtime polymorphic Data Structure.*/
	class Datum
	{
	private:
		//! Array Values Union
		/*! Contains an array for each possible type that can be
		stored within the Datum.*/
		union DatumArray
		{
			void* voidPointer = nullptr;
			int* mIntValues;
			float* mFloatValues;
			std::string* mStringValues;
			glm::vec4* mVectorValues;
			glm::mat4* mMatrixValues;
			RTTI** mPointerValues;
			Scope** mScopeValues;
		};

	public:
		//! Datum Types
		/*! Enum for all of the possible types that this Datum can be.*/
		enum class DatumType
		{
			Unknown = -1,
			Integer = 0,
			Float = 1,
			Vector = 2,
			Matrix = 3,
			Table = 4,
			String = 5,
			Pointer = 6,
			End = 7
		};

		//! Default Constructor
		/*! Creates an empty datum with no data allocated.*/
		Datum();

		//! Copy Constructor
		/*! Performs a deep copy only on provided Datum that has internal storage, or
		makes this Datum an alias to the provided Datum if it has external storage. This
		can alter the Datum from internal/external storage and the Datum's type.
		\param rhs The Datum to be copied over*/
		Datum(const Datum& rhs);

		//! Move Constructor
		/*! Performs move semantics only on provided Datum. This
		\param rhs The Datum to be moved*/
		Datum(Datum&& rhs);

		//! Assignment Operator Overload
		/*! Performs a deep copy only on provided Datum that has internal storage, or
		makes this Datum an alias to the provided Datum if it has external storage. This
		can alter the Datum from internal/external storage and the Datum's type.
		\param rhs The Datum to be copied over*/
		Datum& operator=(const Datum& rhs);

		//! Move Assignment Operator
		/*! Performs move semantics only on provided Datum. This
		\param rhs The Datum to be moved
		\return The moved Datum*/
		Datum& operator=(Datum&& rhs);

		//! Assignment Operator Overload - Scalar
		/*! Assigns a scalar value to the Datum's first entry. If the Datum is empty and
		typeless, this will automatically increase the Datum's capacity and set it's type.
		\param rhs The scalar value to be copied over*/
		Datum& operator=(const int& rhs);
		Datum& operator=(const float& rhs);
		Datum& operator=(const std::string& rhs);
		Datum& operator=(const glm::vec4& rhs);
		Datum& operator=(const glm::mat4& rhs);
		Datum& operator=(RTTI* rhs);
		Datum& operator=(Scope& rhs);

		//! Subscript Operator Overload
		/*! \param index The index into the Datum that contains the scope you are looking for.
		\return A reference to the scope at the found index.*/
		Scope& operator[](std::uint32_t index);

		//! Desctructor
		/*! Wipes all allocated data if this Datum stores internal memory.*/
		~Datum();

		//! Get Type
		/*! \return The type of this Datum.*/
		DatumType Type() const;

		//! Set Type
		/*! Set the type of this Datum only if the Datum's current type
		is Unknown.
		\param type The type to set this Datum to.*/
		void SetType(DatumType type);

		//! Get Is External
		/*! \return If this Datum is using external storage.*/
		bool IsExternal();

		//! Get Size
		/*! \return How many values are stored in this Datum.*/
		std::uint32_t Size() const;

		//! Get Capacity
		/*! \return How many values can this Datum hold.*/
		std::uint32_t Capacity() const;

		//! Get Is Empty
		/*! \return If this Datum has a size of zero/no elements stored within it.*/
		bool IsEmpty() const;

		//! Reserve More Memory
		/* Re-allocates more memory to expand the capacity of how many values the 
		Datum can hold. This will not shrink the Datum's capacity.
		\param capacity How many elements you want this Datum to hold.*/
		void Reserve(std::uint32_t capacity);

		//! Clear
		/*! Free memory and destructs all values within this Datum. This will
		only work on Datum's that store internal data.*/
		void Clear();

		//! Set External Storage
		/*! Enforces that this Datum must wrap around external storage of a 
		particular type and size. If this Datum's type is unknown, SetStorage
		will automatically change the type, otherwise, the type of the Datum
		must match the type of the storage being passed in.
		\param container The external storage that this Datum will wrap.
		\param size How many values are contained in this external storage.*/
		void SetStorage(int* container, std::uint32_t size);
		void SetStorage(float* container, std::uint32_t size);
		void SetStorage(std::string* container, std::uint32_t size);
		void SetStorage(glm::vec4* container, std::uint32_t size);
		void SetStorage(glm::mat4* container, std::uint32_t size);
		void SetStorage(RTTI** container, std::uint32_t size);

		//! Equals Comparison Operator
		/*! \param rhs The Datum to compare against.
		\return if two Datum's have the same size, capacity, type, and contain
		the same values.*/
		bool operator==(const Datum& rhs) const;

		//! Equals Comparison Operator - Scalar
		/*! \param rhs The scalar value to compare against.
		\return If the scalar representation of this Datum is equal to the
		provided scalar value.*/
		bool operator==(const int& rhs) const;
		bool operator==(const float& rhs) const;
		bool operator==(const std::string& rhs) const;
		bool operator==(const glm::vec4& rhs) const;
		bool operator==(const glm::mat4& rhs) const;
		bool operator==(RTTI* rhs) const;
		bool operator==(Scope& rhs) const;

		//! Not Equals Comparison Operator
		/*! \param rhs The Datum to compare against.
		\return if two Datum's do not have the same size, capacity, type, and 
		do not contain the same values.*/
		bool operator!=(const Datum& rhs) const;

		//! Not Equals Comparison Operator - Scalar
		/*! \param rhs The scalar value to compare against.
		\return If the scalar representation of this Datum is not equal to the
		provided scalar value.*/
		bool operator!=(const int& rhs) const;
		bool operator!=(const float& rhs) const;
		bool operator!=(const std::string& rhs) const;
		bool operator!=(const glm::vec4& rhs) const;
		bool operator!=(const glm::mat4& rhs) const;
		bool operator!=(RTTI* rhs) const;
		bool operator!=(Scope& rhs) const;

		//! Set Value At Index
		/* \param value The value to store within this Datum.
		\param index The location in this Datum that you want to store
		this value at.*/
		void Set(const int& value, std::uint32_t index = 0);
		void Set(const float& value, std::uint32_t index = 0);
		void Set(const std::string& value, std::uint32_t index = 0);
		void Set(const glm::vec4& value, std::uint32_t index = 0);
		void Set(const glm::mat4& value, std::uint32_t index = 0);
		void Set(RTTI* value, std::uint32_t index = 0);
		void Set(Scope& value, std::uint32_t index = 0);

		//! Push Back
		/* \param value The value to add to the end of this Datum.
		This operation only works on Datum's with internal storage.*/
		void PushBack(const int& value);
		void PushBack(const float& value);
		void PushBack(const std::string& value);
		void PushBack(const glm::vec4& value);
		void PushBack(const glm::mat4& value);
		void PushBack(RTTI* value);
		void PushBack(Scope& value);

		//! Pop Back
		/*! Removes a value from the end of the Datum. This operation
		only works on Datum's with internal storage.*/
		void PopBack();

		//! Find Value
		/*! \param value The value within the Datum that you are searching for.
		\return The index location of the value.*/
		std::uint32_t Find(const int& value);
		std::uint32_t Find(const float& value);
		std::uint32_t Find(const std::string& value);
		std::uint32_t Find(const glm::vec4& value);
		std::uint32_t Find(const glm::mat4& value);
		std::uint32_t Find(RTTI* value);
		std::uint32_t Find(Scope& value);

		//! Remove Value
		/*! \param value The value within the Datum that you are removing.*/
		void Remove(const int& value);
		void Remove(const float& value);
		void Remove(const std::string& value);
		void Remove(const glm::vec4& value);
		void Remove(const glm::mat4& value);
		void Remove(RTTI* value);
		void Remove(Scope& value);

		//! Remove At
		/*! \param index The index to remove from*/
		void RemoveAt(std::uint32_t index);

		template <typename T>
		//! Get Value At Index
		/*! \param index The location of the value you want to retrieve.
		\return A reference to the value.*/
		T& Get(std::uint32_t index = 0) const;
		template<>
		int& Get<int>(std::uint32_t index) const;
		template <>
		float& Get<float>(std::uint32_t index) const;
		template <>
		std::string& Get<std::string>(std::uint32_t index) const;
		template <>
		glm::vec4& Get<glm::vec4>(std::uint32_t index) const;
		template <>
		glm::mat4& Get<glm::mat4>(std::uint32_t index) const;
		template <>
		RTTI*& Get<RTTI*>(std::uint32_t index) const;
		template <>
		Scope*& Get<Scope*>(std::uint32_t index) const;

		//! Set Value From String
		/*! Sets a value in a Datum based on a string representation of that value.
		For vectors, you will need four float values separated by a space, for example:
		"2.0 2.0 2.0 2.0", and for a matrix, you will need a similar format but for
		sixteen values "2.0 2.0 2.0 2.0 2.0 2.0 ..."
		\param value The string representation of the value
		\param type The type of this value
		\param index The optional location of where to store this value.*/
		void SetFromString(char* value, DatumType type, std::uint32_t index = 0);

		//! Get Value As String
		/*! \param index The location of the value that you want to retrieve.
		\return A string representation of the retrieved value.*/
		std::string ToString(std::uint32_t index);

	private:

		template <typename T>
		//! Copy Contents of one Datum to this Datum
		void copyContents(const Datum& rhs);

		void copyScopes(const Datum& rhs);

		template <typename T>
		//! Check if the contents of one Datum are the same as this Datum
		bool contentsEqual(const Datum& rhs) const;

		bool scopesEqual(const Datum& rhs) const;

		//! Throws exception if this Datum has an Unknown Type
		inline void throwIfUnknown()
		{
			if (mType == DatumType::Unknown)
				throw std::exception("Type has not yet been set.");
		}
		
		//! Throws an exception if this Datum has external storage
		inline void throwIfExternal()
		{
			if (mIsExternal)
				throw std::exception("Cannot change external data");
		}

		DatumArray mContainer;		//!< Union member that contains pointers of various types
		DatumType mType;			//!< The type of this Datum
		std::uint32_t mCapacity;	//!< How many elements this Datum can contain
		std::uint32_t mSize;		//!< How many elements this Datum is holding
		bool mIsExternal;			//!< Keeps track of what kind of storage this Datum has
	};
}
