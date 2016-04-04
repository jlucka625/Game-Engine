#pragma once

using namespace Library;

/*! \brief Test Class
Test class excercising that the Library is correctly linked to other projects
*/
class RFoo : public RTTI
{
	RTTI_DECLARATIONS(RFoo, RTTI)
public:
	//!Default Constructor
	/*! Initializes data to zero*/
	RFoo();

	//! Copy Constructor
	/*! Creates a deep-copy
	\param rhs The Foo to be copied*/
	RFoo(const RFoo& rhs);
	RFoo(RFoo&& rhs);

	/*! Operator "=" Overload
	Creates a deep-copy
	\param rhs The Foo to be copied
	\return A reference to "this" Foo */
	RFoo& operator=(const RFoo& rhs);
	RFoo& operator=(RFoo&& rhs);
	//! Equality Overload
	/*! \param The Foo object to compare against
	\return A boolean value based on if the two Foo objects share
	the same member values*/
	bool operator==(const RFoo& rhs) const;

	//!Destructor
	~RFoo();

	/*!Our test constructor
	\param data arbitrary data to be set to the private member variable, mData
	*/
	RFoo(int data);

	/*!
	Test accessor method
	\return A reference to the private member data
	*/
	int & Data();

	/*!
	Test accessor method
	\return A pointer to the private member data
	*/
	int * MoreData();

private:
	int mData;
	int* mMoreData;
};

