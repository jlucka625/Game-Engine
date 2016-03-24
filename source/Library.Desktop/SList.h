#pragma once

namespace Library {

	template <typename T>
	//! Singly-Linked List Class
	/*! Class template for a Singly-Linked List. Contains operations for insertion/removal from
	the front and back only. Supports all element types. */
	class SList
	{
	private:
		//! List Node Class
		/*! Used to store items in the list and make the list
		traversable through next pointers*/
		class Node
		{
		public:
			//! Parameter List Constructor
			/*! Creates a new node from the given item.
			\param item The item to be stored within the node
			\param next Pointer to the next node in the list, null by default*/
			Node(const T& item, Node* next = nullptr);
			Node(const Node& rhs) = delete;
			Node & operator=(const Node& rhs) = delete;
			T mItem;		//!< The item being stored in this node
			Node* mNext;	//!< A pointer to the next node in the list
		};
		std::uint32_t mSize;	//!< Counter variable to keep track of how many elements are in the list
		Node* mFront;		//!< Pointer to the first node in the list
		Node* mBack;		//!< Pointer to the last node in the list

	public:
		//! Iterator Class
		/* Used to traverse the singly linked list.*/
		class Iterator
		{
			friend class SList<T>;
		public:
			//! Default Constructor
			/* Creates an iterator object that points to a node that is null and
			the reference to the list that owns it is null*/
			Iterator();

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
			same list and point to the same node in that list */
			bool operator==(const Iterator& rhs) const;

			//! Inequality Overload
			/* \param rhs Iterator to compare against
			\return boolean value that represents if both iterators are not 
			owned by the same list or do not point to the same node in their list */
			bool operator!=(const Iterator& rhs) const;

			//! Pre-Increment Overload
			/* Immediately increments the Iterator to the next node in the list
			\return A reference to the updated Iterator*/
			Iterator& operator++();

			//! Post-Increment Overload
			/* Copies the iterator and then increments it
			int is a dummy value used to remove ambiguity between pre and post fix
			\return a copy of the Iterator before it has been incremented*/
			Iterator operator++(int);

			//! Dereference Overload
			/* \return A reference to the item that this Iterator's node is pointing to*/
			T& operator*() const;
		private:
			//! Parameter List Constructor
			/* \param currentNode Pointer to a node that you want this iterator to start at
			\param list Pointer to a list that will become the owner of this iterator*/
			Iterator(Node* currentNode, const SList* list);
			Node* mCurrentNode;			//!< Pointer to a node in the list
			const SList* mList;			//!< Pointer to the list that owns this Iterator
		};

		//! Default Constructor
		/*! Creates an empty list */
		SList();

		//! Copy Constructor
		/*! Creates a deep-copy
		\param rhs The list to be copied*/
		SList(const SList& rhs);

		SList(SList&& rhs);

		//! Destructor
		/*! Clears the list of all elements before freeing data*/
		~SList();

		/*! Operator "=" Overload
		Creates a deep-copy
		\param rhs The list to be copied
		\return A reference to "this" list */
		SList & operator=(const SList& rhs);

		SList & operator=(SList&& rhs);

		/*! Adds item to the front of the list
		\param item Reference of the item to be added*/
		Iterator PushFront(const T& item);

		/*! Removes the item at the front of the list*/
		void PopFront();

		/*! Adds item to the back of the list
		\param item Reference of the item to be added*/
		Iterator PushBack(const T& item);

		/*! \return Indicates whether or not the list is empty*/
		bool IsEmpty() const;

		/*! \return A reference to the first item in the list*/
		T & Front();

		const T & Front() const;

		/*! \return A reference to the last item in the list*/
		T & Back();

		/*! \return How many items are contained in the list*/
		std::uint32_t Size() const;

		/*! Removes all items in the list*/
		void Clear();

		/*! \return An Iterator pointing to the front of the list*/
		Iterator begin() const;

		/*! \return An Iterator pointing past the back of the list*/
		Iterator end() const;

		/*! Appends the given item after the item the given iterator points to
		\param item The item to append
		\param An iterator pointing to where you want to append*/
		void InsertAfter(const T& item, const Iterator& it);

		/*! \param value The item that you are looking for in the list
		\return An iterator pointing to that item. Will return end if
		the item was not found*/
		Iterator Find(const T& value) const;

		/*! Removes the item associated with the given data. Calling Remove on
		a non-existent item will not cause harm to the list.
		\param item The item to remove*/
		void Remove(const T& item);
	};
}

#include "SList.inl"