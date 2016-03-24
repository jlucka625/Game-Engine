#include "pch.h"

RTTI_DEFINITIONS(Bar)

Vector<std::string> Bar::mPrescribedAttributes;

Bar::Bar()
{
	intVal = 100;

	InitializeSignatures();
}

Bar::Bar(const Bar& rhs) :
	Bar()
{
	operator=(rhs);
}

Bar::Bar(Bar&& rhs) :
	Bar()
{
	operator=(std::move(rhs));
}

Bar& Bar::operator=(Bar&& rhs)
{
	if (this != &rhs)
	{
		Foo::operator=(std::move(rhs));
		intVal = rhs.intVal;
	}

	return *this;
}

Bar& Bar::operator=(const Bar& rhs)
{
	if (this != &rhs)
	{
		Foo::operator=(rhs);
	}

	return *this;
}

bool Bar::operator==(const Bar& rhs) const
{
	return Scope::operator==(rhs);
}

Bar::~Bar()
{
	//Foo::~Foo();
}

void Bar::InitializeSignatures()
{
	AddInternalAttribute("childAttr", intVal, 1);
}

Vector<std::string>& Bar::PrescribedAttributes() const
{
	return mPrescribedAttributes;
}