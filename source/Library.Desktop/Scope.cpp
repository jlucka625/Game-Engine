#include "pch.h"

namespace Library
{
	RTTI_DEFINITIONS(Scope)

	Scope::Scope(std::uint32_t capacity) :
		mData(capacity), mOrderList(), parent(nullptr)
	{}

	Scope::Scope(const Scope& rhs) :
		Scope(rhs.mData.Capacity())
	{
		operator=(rhs);
	}

	Scope::Scope(Scope&& rhs) :
		mData(rhs.mData), mOrderList(rhs.mOrderList), parent(nullptr)
	{
		Reparent(std::move(rhs));
	}

	Scope::~Scope()
	{
		Orphan();
		Clear();
	}

	Scope& Scope::operator=(Scope&& rhs)
	{
		if (this != &rhs)
		{
			Orphan();
			Clear();

			mData = std::move(rhs.mData);
			mOrderList = std::move(rhs.mOrderList);

			Reparent(std::move(rhs));

		}
		return *this;
	}

	void Scope::Reparent(Scope&& rhs)
	{
		if (parent != nullptr)
		{
			int index;
			Datum* datum = rhs.parent->FindContainedScope(rhs, index);
			datum->Set(this, index);
			rhs.parent = nullptr;
		}

		for (auto& pair : mOrderList)
		{
			Datum& datum = pair->second;
			if (datum.Type() == Datum::DatumType::Table)
			{
				for (std::uint32_t i = 0; i < datum.Size(); ++i)
				{
					datum[i].parent = this;
				}
			}
		}
	}

	Scope& Scope::operator=(const Scope& rhs)
	{
		if (this != &rhs)
		{
			Clear();
			for (auto& pair : rhs.mOrderList)
			{
				Datum& datum = pair->second;
				if (datum.Type() == Datum::DatumType::Table)
				{
					for (std::uint32_t i = 0; i < datum.Size(); ++i)
					{
						Scope& newScope = AppendScope(pair->first);
						newScope = datum[i];
					}
				}
				else
				{
					Datum& newDatum = Append(pair->first);
					newDatum = datum;
				}
			}
		}
		return *this;
	}

	Datum* Scope::Find(const std::string& name) const
	{
		Iterator it = mData.Find(name);
		if (it != mData.end())
			return &(*mData.Find(name)).second;
		else
			return nullptr;
	}

	Datum* Scope::Search(const std::string& name, Scope** foundScope) const
	{
		Datum* result = Find(name);
		if (result != nullptr && foundScope != nullptr)
		{
			*foundScope = const_cast<Scope*>(this);
		}
		else if(parent != nullptr)
		{
			return parent->Search(name, foundScope);
		}
		return result;
	}

	Datum& Scope::Append(const std::string& name)
	{
		if (name == "")
			throw std::exception("Cannot use default string as a key");

		bool existed = false;
		Iterator it = mData.Insert(PairType(name, Datum()), existed);
		if (!existed)
			mOrderList.PushBack(&(*it));

		return (*it).second;
	}

	Scope& Scope::AppendScope(const std::string& name)
	{
		Datum& datum = Append(name);
		datum.SetType(Datum::DatumType::Table);

		Scope* newScope = new Scope();
		newScope->parent = this;
		datum.PushBack(*newScope);
		
		return *newScope;
	}

	void Scope::Adopt(Scope& child, const std::string& name)
	{
		if (this == &child)
			throw std::exception("Scope cannot adopt itself.");

		Datum& result = Append(name);
		result.SetType(Datum::DatumType::Table);

		child.Orphan();
		result.PushBack(child);
		child.parent = this;
	}

	bool Scope::Orphan()
	{
		if (parent == nullptr)
			return false;
		else
		{
			int index = 0;
			Datum* foundDatum = parent->FindContainedScope(*this, index);
			if (foundDatum == nullptr)
				return false;
			else
			{
				foundDatum->RemoveAt(index);
				parent = nullptr;
				return true;
			}
		}
	}

	Datum* Scope::FindContainedScope(Scope& child, int& index) const
	{
		for (auto& pair : mOrderList)
		{
			Datum& datum = pair->second;
			if (datum.Type() == Datum::DatumType::Table)
			{
				for (std::uint32_t i = 0; i < datum.Size(); ++i)
				{
					if (datum[i] == child)
					{
						index = i;
						return &datum;
					}
				}
			}
		}
		return nullptr;
	}

	void Scope::Clear()
	{
		for (auto& pair : mOrderList)
		{
			Datum& datum = pair->second;
			if (datum.Type() == Datum::DatumType::Table)
			{
				for (std::uint32_t i = 0; i < datum.Size(); ++i)
				{
					datum[i].parent = nullptr;
					delete &datum[i];
				}
			}
		}
		mOrderList.Clear();
		mData.Clear();
	}

	Scope* Scope::GetParent() const
	{
		return parent;
	}

	Datum& Scope::operator[](const std::string& name)
	{
		return Append(name);
	}

	Datum& Scope::operator[](std::uint32_t index)
	{
		return mOrderList[index]->second;
	}

	std::uint32_t Scope::IndexOf(const std::string& name) const
	{
		std::uint32_t index = 0;
		for (auto& pair : mOrderList)
		{
			if (pair->first == name)
				break;

			index++;
		}
		return index;
	}

	bool Scope::operator==(const Scope& rhs) const
	{
		if (this == &rhs)
			return true;

		if (mOrderList.Size() != rhs.mOrderList.Size())
			return false;

		for (std::uint32_t i = 0; i < mOrderList.Size(); i++)
			if (*mOrderList[i] != *rhs.mOrderList[i])
				return false;
		
		return true;
	}

	bool Scope::operator!=(const Scope& rhs) const
	{
		return !(operator==(rhs));
	}

	std::string Scope::FindName(const Scope& scope) const
	{
		for (const auto& pair : mOrderList)
			if (pair->second.Type() == Datum::DatumType::Table)
				for (std::uint32_t i = 0; i < pair->second.Size(); ++i)
					if (pair->second[i] == scope)
						return pair->first;
		return "";
	}

	const Vector<std::pair<std::string, Datum>*>& Scope::GetData() const
	{
		return mOrderList;
		//return const_cast<Vector<std::pair<std::string, Datum>*>&>(mOrderList);
	}

	void Scope::Append(const std::string& name, const Datum& datum)
	{
		if (name == "")
			throw std::exception("Cannot use default string as a key");

		bool existed = false;
		Iterator it = mData.Insert(PairType(name, datum), existed);
		if (!existed)
			mOrderList.PushBack(&(*it));
	}
}
