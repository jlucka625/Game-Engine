#pragma once

using namespace Library;

class Bar : public Foo
{
	RTTI_DECLARATIONS(Bar, Foo)
public:
	Bar();
	Bar(const Bar& rhs);
	Bar(Bar&& rhs);

	Bar& operator=(const Bar& rhs);
	Bar& operator=(Bar&& rhs);

	bool operator==(const Bar& rhs) const;

	virtual ~Bar();

	void InitializeSignatures();
	virtual Vector<std::string>& PrescribedAttributes() const;

private:
	static Vector<std::string> mPrescribedAttributes;
	int intVal;

};

