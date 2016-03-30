#include "pch.h"

namespace Library
{
	RTTI_DEFINITIONS(Action)

	Vector<std::string> Action::sPerscribedAttributes;
	const std::string Action::actionsKey = "Actions";
	const std::string Action::targetKey = "Target";
	const std::string Action::valueKey = "Value";
	const std::string Action::prototypeKey = "Prototype";

	Action::Action() :
		mName("")
	{
		InitializeAttributes();
	}

	const std::string & Action::Name() const
	{
		return mName;
	}

	void Action::SetName(const std::string & name)
	{
		mName = name;
	}

	void Action::Clear()
	{
		sPerscribedAttributes.Clear();
	}

	Vector<std::string>& Action::PrescribedAttributes() const
	{
		return sPerscribedAttributes;
	}

	void Action::InitializeAttributes()
	{
		AddExternalAttribute("Name", 1, &mName);
	}
}


