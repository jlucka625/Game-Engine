#pragma once
namespace Library
{
	//! Scope Class
	/*! Represents a dyanmic hierarchical database. Contains tables
	of Datums' that may contain Scopes. This creates a recursive structure
	resembling a tree.*/
	class Scope : public RTTI
	{
		RTTI_DECLARATIONS(Scope, RTTI)
	public:
		//! Default Constructor
		/*! Creates an empty scope with no parent.
		\param capacity The initial capacity of the table, default to 11.
		Throws exception if a zero capacity is passed in.*/
		explicit Scope(std::uint32_t capacity = 11);

		//! Copy Constructor
		/*! Creates a deep-copy of the provided scope.
		\param rhs The scope to copy.*/
		Scope(const Scope& rhs);

		//! Move Constructor
		/*! Performs move semantics the provided scope.
		\param rhs The scope to move.*/
		Scope(Scope&& rhs);

		//! Destructor
		/*! Orphans this scope from it's parent and clears the scope.*/
		virtual ~Scope();

		//! Clear
		/*! Frees any allocated data that this scope owns, and empties the
		table of all entries.*/
		void Clear();

		//! Assignment Operator Overload
		/*! Creates a deep-copy of the provided scope.
		\param rhs The scope to copy.
		\return A reference to the copied Scope.*/
		Scope& operator=(const Scope& rhs);

		//! Move Assignment Operator Overload
		/*! Performs move semantics on the provided scope.
		\param rhs The scope to move.
		\return A reference to the moved Scope.*/
		Scope& operator=(Scope&& rhs);

		//! Find Datum by Name
		/*! \param name The name used as a key for the Datum we are looking for.
		\return A pointer to the found Datum within this Scope. If
		no such name was found, nullptr is returned.*/
		Datum* Find(const std::string& name) const;

		//! Find Scope Contained Within a Datum
		/*! \param child The scope we are looking for.
		\param index A reference to the index in the Datum that
		the scope was found at.
		\return A pointer to the Datum that contains the Scope. If
		no such name was found, nullptr is returned.*/
		Datum* FindContainedScope(Scope& child, int& index) const;

		//! Search Scope Hierarchy
		/*! \param name The name used as a key for the Datum we are looking for.
		\param foundScope Address of a pointer to a scope that will be set to 
		the scope where the Datum resides.
		\return A pointer to the found Datum within this Scope. If
		no such name was found, nullptr is returned.*/
		Datum* Search(const std::string& name, Scope** foundScope = nullptr) const;

		//! Append Datum to Scope
		/*! Adds a new Datum to this scope if one at the provided name does not
		exist. Otherwise, it returns the existing Datum at that name.
		\param name The name that refers to the key where the Datum is stored.
		\return A reference to the found or newly created Datum.*/
		Datum& Append(const std::string& name);

		//! Nest a Scope within this Scope
		/*! Adds a new Datum of type table to this scope. If a Datum associated with
		the provided name exists, then the new scope will be added to the Datum.
		\param name The name that refers to the key where the Datum is stored.
		\return A reference to the newly added scope.*/
		Scope& AppendScope(const std::string& name);

		//! Adopt an existing Scope
		/*! Adds an existing scope as a child to this scope. The new child scope will
		be orphaned before being adopted.
		\param child The scope to be adopted
		\param name The name of the Datum where you want this Scope to be stored.*/
		void Adopt(Scope& child, const std::string& name);

		//! Orphan child from Parent
		/*! Forces this child's parent to remove it from its scope. Also
		makes the child no longer have a parent.
		\return True if the separation from the parent succeeded. False if the child
		already doesn't have a parent, or it's supposed parent doesn't have a reference
		to the child in its table.*/
		bool Orphan();

		//! Get the Parent of this Scope
		/*! \return A pointer to this Scope's parent.*/
		Scope* GetParent() const;

		//! Subscript Operator Overload by Name
		/*! Acts as a wrapper for Append.
		\param name The name that refers to the key where the Datum is stored.
		\return A reference to the found or newly created Datum.*/
		Datum& operator[](const std::string& name);

		//! Subscript Operator Overload by Index
		/*! \param index The index where the Datum you want is stored within this Scope/
		\return A reference to the found Datum.*/
		Datum& operator[](std::uint32_t index);

		//! Get Index of Datum
		/*! \param name Name of the Datum you are looking for
		\return Index representing what order the Datum was stored in.*/
		std::uint32_t IndexOf(const std::string& name) const;

		//! Equals Operator Overload
		/*! \param rhs The Scope to compare against
		\return If the two scopes contain the same number of entries and have all of
		the same Datum's.*/
		bool operator==(const Scope& rhs) const;

		//! Not Equals Operator Overload
		/*! \param rhs The Scope to compare against
		\return If the two scopes do not contain the same number of entries or do not 
		have all of the same Datum's.*/
		bool operator!=(const Scope& rhs) const;

		//! Find Name of Scope by Pointer
		/*! \param scope A pointer to the scope who's name we are looking for.
		\return The name of the found scope.*/
		std::string FindName(const Scope& scope) const;

	private:
		void Reparent(Scope&& rhs);

	protected:
		typedef std::pair<std::string, Datum> PairType;
		typedef HashMap<std::string, Datum> Dictionary;
		typedef Dictionary::Iterator Iterator;

		Vector<PairType*> mOrderList;	//!< A vector to keep track of the order that each pair was stored
		Dictionary mData;				//!< A hash map to store name, Datum pairs for each entry in this Scope
		Scope* parent;					//!< A pointer to the Scope that contains this Scope
	};
}

