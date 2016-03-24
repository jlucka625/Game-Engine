#pragma once
namespace Library {
	template <typename T>
	//! Stack Adaptor Class
	/*! Class template for a Stack. Contains operations for insertion/removal from
	the top of the structure only.*/
	class Stack
	{
	public:
		//! Default Constructor
		/*! Creates an empty stack */
		Stack();

		//! Copy Constructor
		/*! Creates a deep-copy
		\param rhs The stack to be copied*/
		Stack(const Stack& rhs);

		/*! Operator "=" Overload
		Creates a deep-copy
		\param rhs The stack to be copied
		\return A reference to "this" stack */
		Stack& operator=(const Stack& rhs);

		//! Destructor
		/*! Pops all elements off of the stack before deleting itself*/
		~Stack() = default;

		//! Push to Top of the Stack
		/*! \param item The element to insert.*/
		void Push(const T& item);

		//! Pop from the Top of the Stack
		/*! Removes the item sitting at the top of the stack.*/
		void Pop();

		//! Peek
		/*! \return A reference to the item at the top of the stack.*/
		T& Top();

		//! Peek for Const Stacks
		/*! \return A constant reference to the item at the top of the stack.*/
		const T& Top() const;

		//! Get Size
		/*! \return How many items are contained in the stack*/
		std::uint32_t Size() const;

		//! Is Empty
		/*! \return Indicates whether or not the size of the stack is zero*/
		bool IsEmpty() const;

		//! Clear
		/*! Removes all items in the stack*/
		void Clear();
	private:
		SList<T> mList;	//!< Underlying linked-list container that holds our elements
	};
}

#include "Stack.inl"