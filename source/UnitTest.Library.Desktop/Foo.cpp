#include "pch.h"

RTTI_DEFINITIONS(Foo)

Vector<std::string> Foo::mPrescribedAttributes;

Foo::Foo() 
{
	intVal = 100;
	floatVal = 5.6f;
	stringVal = "hello";
	vecVal = glm::vec4(1.0f);
	matVal = glm::mat4(2.0f);
	ptrVal = new RFoo();

	intVals = new int[3];
	floatVals = new float[3];
	stringVals = new std::string[3];
	vecVals = new glm::vec4[3];
	matVals = new glm::mat4[3];
	ptrVals = new RFoo*[3];

	tableVal = new Scope();
	(*tableVal)["a"];
	(*tableVal)["b"];
	(*tableVal)["c"];

	for (int i = 0; i < 10; i++)
	{
		(*tableVal).AppendScope("array1");
	}

	InitializeSignatures();
}

Vector<std::string>& Foo::PrescribedAttributes() const
{
	return mPrescribedAttributes;
}

Foo::Foo(const Foo& rhs) :
	Foo()
{
	operator=(rhs);
}

Foo::Foo(Foo&& rhs) :
	Foo()
{
	operator=(std::move(rhs));
}

Foo& Foo::operator=(Foo&& rhs)
{
	if (this != &rhs)
	{
		Attributed::operator=(std::move(rhs));

		delete ptrVal;
		delete[] intVals;
		delete[] floatVals;
		delete[] stringVals;
		delete[] vecVals;
		delete[] matVals;
		delete[] ptrVals;

		intVal = rhs.intVal;
		floatVal = rhs.floatVal;
		stringVal = rhs.stringVal;
		vecVal = rhs.vecVal;
		matVal = rhs.matVal;
		ptrVal = rhs.ptrVal;

		intVals = rhs.intVals;
		floatVals = rhs.floatVals;
		stringVals = rhs.stringVals;
		vecVals = rhs.vecVals;
		matVals = rhs.matVals;
		ptrVals = rhs.ptrVals;

		rhs.intVals = nullptr;
		rhs.floatVals = nullptr;
		rhs.stringVals = nullptr;
		rhs.vecVals = nullptr;
		rhs.matVals = nullptr;
		rhs.ptrVals = nullptr;
		rhs.ptrVal = nullptr;

	}

	return *this;
}

Foo& Foo::operator=(const Foo& rhs)
{
	if (this != &rhs)
	{
		Attributed::operator=(rhs);

		delete ptrVal;
		delete[] intVals;
		delete[] floatVals;
		delete[] stringVals;
		delete[] vecVals;
		delete[] matVals;
		delete[] ptrVals;

		intVal = rhs.intVal;
		floatVal = rhs.floatVal;
		stringVal = rhs.stringVal;
		vecVal = rhs.vecVal;
		matVal = rhs.matVal;
		ptrVal = new RFoo(*rhs.ptrVal);

		intVals = new int[3];
		for (std::uint32_t i = 0; i < 3; i++) intVals[i] = rhs.intVals[i];

		floatVals = new float[3];
		for (std::uint32_t i = 0; i < 3; i++) floatVals[i] = rhs.floatVals[i];

		stringVals = new std::string[3];
		for (std::uint32_t i = 0; i < 3; i++) stringVals[i] = rhs.stringVals[i];

		vecVals = new glm::vec4[3];
		for (std::uint32_t i = 0; i < 3; i++) vecVals[i] = rhs.vecVals[i];

		matVals = new glm::mat4[3];
		for (std::uint32_t i = 0; i < 3; i++) matVals[i] = rhs.matVals[i];

		ptrVals = new RFoo*[3];
		for (std::uint32_t i = 0; i < 3; i++) ptrVals[i] = rhs.ptrVals[i];

	}

	return *this;
}

bool Foo::operator==(const Foo& rhs) const
{
	return Scope::operator==(rhs);
}

Foo::~Foo()
{
	delete ptrVal;
	delete[] intVals;
	delete[] floatVals;
	delete[] stringVals;
	delete[] vecVals;
	delete[] matVals;
	delete[] ptrVals;
}

void Foo::InitializeSignatures()
{
	AddInternalAttribute("health", intVal, 1);

	AddInternalAttribute("hitChance", floatVal, 1);
	AddInternalAttribute("EnemyName", stringVal, 1);
	AddInternalAttribute("Position", vecVal, 1);
	AddInternalAttribute("Transform", matVal, 1);
	AddInternalAttribute("Object", ptrVal, 1);

	AddNestedScope("Scope", *tableVal);

	AddExternalAttribute("ints", 3, intVals);
	AddExternalAttribute("floats", 3, floatVals);
	AddExternalAttribute("strings", 3, stringVals);
	AddExternalAttribute("vectors", 3, vecVals);
	AddExternalAttribute("matrices", 3, matVals);
	AddExternalAttribute("pointers", 3, reinterpret_cast<RTTI**>(ptrVals));
}