#pragma once

using namespace Library;

/*! \brief Test Class
Test class excercising that the Library is correctly linked to other projects
*/
class Foo : public Attributed
{
	RTTI_DECLARATIONS(Foo, Attributed)
public:
	//!Default Constructor
	/*! Initializes data to zero*/
	Foo();

	//! Copy Constructor
	/*! Creates a deep-copy
	\param rhs The Foo to be copied*/
	Foo(const Foo& rhs);

	//! Move Constructor
	/*! Creates move semantics
	\param rhs The Foo to be moved*/
	Foo(Foo&& rhs);

	/*! Operator "=" Overload
	Creates a deep-copy
	\param rhs The Foo to be copied
	\return A reference to "this" Foo */
	Foo& operator=(const Foo& rhs);

	//! Move Assignment Operator Overload
	/*! Creates move semantics
	\param rhs The Foo to be moved
	\return The moved Foo*/
	Foo& operator=(Foo&& rhs);

	//! Equality Overload
	/*! \param The Foo object to compare against
	\return A boolean value based on if the two Foo objects share
	the same member values*/
	bool operator==(const Foo& rhs) const;

	//!Destructor
	virtual ~Foo();

	void InitializeSignatures();

	virtual Vector<std::string>& PrescribedAttributes() const;

private:

	static Vector<std::string> mPrescribedAttributes;

	int intVal;
	float floatVal;
	std::string stringVal;
	glm::vec4 vecVal;
	glm::mat4 matVal;
	RFoo* ptrVal;

	Scope* tableVal;

	int* intVals;
	float* floatVals;
	std::string* stringVals;
	glm::vec4* vecVals;
	glm::mat4* matVals;
	RFoo** ptrVals;

};