#pragma once
namespace Library 
{
	template<typename T>
	//! Templated Vector Class
	/*! Class template for a Vector. Contains a dynamic array that supports
	all element types.*/
	class Vector
	{
	public:
		//! Iterator Class
		/*! Used to traverse the Vector.*/
		class Iterator
		{
			friend class Vector<T>;
		public:
			//! Default Constructor
			/* Creates an iterator object that has an index of zero and
			the reference to the vector that owns it is null*/
			Iterator();

			//! Copy Constructor
			/* \param rhs Iterator rvalue who's memory you want to copy*/
			Iterator(const Iterator& rhs);

			//! Move Constructor
			/* \param rhs Iterator rvalue who's memory you want to move*/
			Iterator(Iterator&& rhs);

			//! Copy Assignment Overload
			/* Creates a deep-copy of the iterator
			\param rhs The Iterator to copy
			\return A reference to the copied Iterator*/
			Iterator& operator=(const Iterator& rhs);

			//! Move Assignment Overload
			/* Performs move semantics on the provided Iterator
			\param rhs The Iterator that you want to move
			\return A reference to the moved Iterator*/
			Iterator& operator=(Iterator&& rhs);

			//! Equality Overload
			/* \param rhs Iterator to compare against
			\return boolean value that represents if iterators are owned by the
			same vector and refer to the same index */
			bool operator==(const Iterator& rhs) const;

			//! Inequality Overload
			/* \param rhs Iterator to compare against
			\return boolean value that represents if both iterators are not
			owned by the same vector or do not refer to the same index */
			bool operator!=(const Iterator& rhs) const;

			//! Pre-Increment Overload
			/* Immediately increments the Iterator to the next index in the vector
			\return A reference to the updated Iterator*/
			Iterator& operator++();

			//! Post-Increment Overload
			/* Copies the iterator and then increments it
			int is a dummy value used to remove ambiguity between pre and post fix
			\return a copy of the Iterator before it has been incremented*/
			Iterator operator++(int);

			//! Dereference Overload
			/* \return A reference to the item that this Iterator's index is referring
			to in the vector*/
			T& operator*();

			//! Dereference Overload for Const Vectors
			/* \return A constant reference to the item that this Iterator's index is referring
			to in the vector*/
			const T& operator*() const;
		private:
			//! Parameter List Constructor
			/* \param index value holding the location of the item in the vector
			\param vector Pointer to a vector that will become the owner of this iterator*/
			Iterator(std::uint32_t index, const Vector* vector);
			std::uint32_t mIndex;		//!< Value holding the location of the item in the vector
			const Vector* mVector;		//!< Pointer to the vector that owns this Iterator
		};

		//! Paramter List Constructor
		/*! Creates an empty vector with size zero and reserves with a capacity
		specified by the user.*/
		Vector(std::uint32_t capacity = 10, bool fixedSize = false);

		//! Copy Constructor
		/*! Creates a deep-copy
		\param rhs The vector to be copied*/
		Vector(const Vector& rhs);

		//! Move Constructor
		/* \param rhs Vector rvalue who's memory you want to move*/
		Vector(Vector&& rhs);

		//! Destructor
		/*! Calls Clear() on the vector*/
		~Vector();

		//! Subscript Operator Overload
		/*! \param index The index corresponding to the value stored in the vector
		\return A reference to the item stored at that index*/
		T& operator[](std::uint32_t index);

		//! Subscript Operator Overload for Const Vectors
		/*! \param index The index corresponding to the value stored in the vector
		\return A constant reference to the item stored at that index*/
		const T& operator[](std::uint32_t index) const;

		//! Insert to Back of the List
		/*! \param item Reference of the item to be added*/
		Iterator PushBack(const T& item);

		//! Remove from the Back of the Vector
		/*! Calls the destructor of the item at the back of the vector. Does
		not free memory*/
		void PopBack();

		//! Is Empty
		/*! \return Indicates whether or not the size of the vector is zero*/
		bool IsEmpty() const;

		//! Get Front of the Vector
		/*\return A reference to the front of the vector*/
		T& Front();

		//! Get Front of the Const Vector
		/*\return A constant reference to the front of the vector*/
		const T& Front() const;

		//! Get Back of the Vector
		/*\return A reference to the back of the vector*/
		T& Back();

		//! Get Back of the Const Vector
		/*\return A constant reference to the back of the vector*/
		const T& Back() const;

		//! Get Size
		/*! \return How many items are contained in the vector*/
		std::uint32_t Size() const;

		//! Get Capacity
		/*! \return How many items can this vector hold*/
		std::uint32_t Capacity() const;

		//! Begin
		/*! \return An Iterator pointing to the front of the list*/
		Iterator begin() const;

		//! End
		/*! \return An Iterator pointing past the end of the list*/
		Iterator end() const;

		//! Assignment Operator Overload
		/*Creates a deep-copy
		\param rhs The vector to be copied
		\return A reference to "this" vector */
		Vector& operator=(const Vector& rhs);

		//! Move Assignment Overload
		/* Performs move semantics on the provided vector
		\param rhs The vector that you want to move
		\return A reference to the moved vector*/
		Vector& operator=(Vector&& rhs);

		//! Get At Index
		/*! \param index The index corresponding to the value stored in the vector
		\return A reference to the item stored at that index*/
		T& At(std::uint32_t index) const;

		//! Reserve capacity
		/*! Expands the capacity of the vector to hold more elements. Preserves
		the current contents of the vector. Vector cannot be downsized.
		\param capacity The amount to expand the array*/
		void Reserve(std::uint32_t capacity, bool fixedSize = false);

		//! Search for Element
		/*! \param value The item that you are looking for in the vector
		\return An iterator pointing to that item. Will return end if
		the item was not found*/
		Iterator Find(const T& item) const;

		//! Clear
		/*! Removes all items in the list*/
		void Clear();

		//! Remove Item
		/*! Removes the item associated with the given data. Calling Remove on
		a non-existent item will not cause harm to the list.
		\param item The item to remove*/
		void Remove(const T& item);

	private:
		std::uint32_t mSize;			//!< Counter variable to keep track of how many elements are in the list
		std::uint32_t mCapacity;		//!< Tells us how many elements can be stored into the list
		T* mContainer;					//!< The array holding our data that this class wraps around
	};
}

#include "vector.inl"