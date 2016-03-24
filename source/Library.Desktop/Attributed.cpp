#include "pch.h"

namespace Library 
{
	RTTI_DEFINITIONS(Attributed)

	Attributed::Attributed()
	{
		(*this)["this"] = this;
	}

	Attributed::Attributed(const Attributed & rhs) :
		Scope(rhs)
	{
		(*this)["this"] = this;
	}

	Attributed::Attributed(Attributed && rhs) :
		Scope(std::move(rhs))
	{
		(*this)["this"] = this;
	}

	Attributed::~Attributed()
	{

	}

	Attributed& Attributed::operator=(const Attributed & rhs)
	{
		if (this != &rhs)
		{
			Scope::operator=(rhs);
		}
		return *this;
	}

	Attributed& Attributed::operator=(Attributed && rhs)
	{
		if (this != &rhs)
		{
			Scope::operator=(std::move(rhs));
		}
		return *this;
	}

	bool Attributed::IsPrescribedAttribute(const std::string& name) const
	{
		return PrescribedAttributes().Find(name) != PrescribedAttributes().end();
	}

	bool Attributed::IsAuxiliaryAttribute(const std::string& name) const
	{
		return IsAttributed(name) && !IsPrescribedAttribute(name);
	}

	bool Attributed::IsAttributed(const std::string& name) const
	{
		return Find(name) != nullptr;
	}

	Datum& Attributed::AppendAuxiliaryAttribute(const std::string& name)
	{
		if (IsPrescribedAttribute(name))
			throw std::exception("Perscribed Attribute with this name already exists");

		return Append(name);
	}

	void Attributed::AddNestedScope(std::string name, Scope& child)
	{
		Adopt(child, name);
		PrescribedAttributes().PushBack(name);
	}

#pragma region Add Internal Attributes
	void Attributed::AddInternalAttribute(const std::string& name, int& initialValue, std::uint32_t size)
	{
		InitializeInternalAttribute(name, Datum::DatumType::Integer, initialValue, size);
	}

	void Attributed::AddInternalAttribute(const std::string& name, float& initialValue, std::uint32_t size)
	{
		InitializeInternalAttribute(name, Datum::DatumType::Float, initialValue, size);
	}

	void Attributed::AddInternalAttribute(const std::string& name, std::string& initialValue, std::uint32_t size)
	{
		InitializeInternalAttribute(name, Datum::DatumType::String, initialValue, size);
	}

	void Attributed::AddInternalAttribute(const std::string& name, glm::vec4& initialValue, std::uint32_t size)
	{
		InitializeInternalAttribute(name, Datum::DatumType::Vector, initialValue, size);
	}

	void Attributed::AddInternalAttribute(const std::string& name, glm::mat4& initialValue, std::uint32_t size)
	{
		InitializeInternalAttribute(name, Datum::DatumType::Matrix, initialValue, size);
	}

	void Attributed::AddInternalAttribute(const std::string& name, RTTI* initialValue, std::uint32_t size)
	{
		InitializeInternalAttribute(name, Datum::DatumType::Pointer, initialValue, size);
	}
#pragma endregion

#pragma region Add External Attributes
	void Attributed::AddExternalAttribute(const std::string& name, std::uint32_t size, int* storage)
	{
		InitializeExternalAttribute(name, Datum::DatumType::Integer, storage, size);
	}

	void Attributed::AddExternalAttribute(const std::string& name, std::uint32_t size, float* storage)
	{
		InitializeExternalAttribute(name, Datum::DatumType::Float, storage, size);
	}

	void Attributed::AddExternalAttribute(const std::string& name, std::uint32_t size, std::string* storage)
	{
		InitializeExternalAttribute(name, Datum::DatumType::String, storage, size);
	}

	void Attributed::AddExternalAttribute(const std::string& name, std::uint32_t size, glm::vec4* storage)
	{
		InitializeExternalAttribute(name, Datum::DatumType::Vector, storage, size);
	}

	void Attributed::AddExternalAttribute(const std::string& name, std::uint32_t size, glm::mat4* storage)
	{
		InitializeExternalAttribute(name, Datum::DatumType::Matrix, storage, size);
	}

	void Attributed::AddExternalAttribute(const std::string& name, std::uint32_t size, RTTI** storage)
	{
		InitializeExternalAttribute(name, Datum::DatumType::Pointer, storage, size);
	}
#pragma endregion

}



