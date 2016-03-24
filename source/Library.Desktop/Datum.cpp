#include "pch.h"

namespace Library
{

#pragma region Helper Method Templates

	template <typename T>
	void Datum::copyContents(const Datum& rhs)
	{
		for (std::uint32_t i = 0; i < rhs.mSize; i++)
		{
			PushBack(rhs.Get<T>(i));
		}
	}

	void Datum::copyScopes(const Datum& rhs)
	{
		for (std::uint32_t i = 0; i < rhs.mSize; i++)
		{
			PushBack(*rhs.Get<Scope*>(i));
		}
	}

	template <typename T>
	bool Datum::contentsEqual(const Datum& rhs) const
	{
		for (std::uint32_t i = 0; i < rhs.mSize; i++)
		{
			if (Get<T>(i) != rhs.Get<T>(i))
			{
				return false;
			}
		}
		return true;
	}

	bool Datum::scopesEqual(const Datum& rhs) const
	{
		for (std::uint32_t i = 0; i < rhs.mSize; i++)
		{
			if (*Get<Scope*>(i) != *rhs.Get<Scope*>(i))
			{
				return false;
			}
		}
		return true;
	}

#pragma endregion

#pragma region Get Method Specializations
	template<>
	int& Datum::Get<int>(std::uint32_t index) const
	{
		if (mType != DatumType::Integer)
			throw std::exception("Datum is not of Integer type");

		if (index >= mSize)
			throw std::exception("Out of bounds");

		return mContainer.mIntValues[index];
	}

	template<>
	float& Datum::Get<float>(std::uint32_t index) const
	{
		if (mType != DatumType::Float)
			throw std::exception("Datum is not of Float type");

		if (index >= mSize)
			throw std::exception("Out of bounds");

		return mContainer.mFloatValues[index];
	}

	template<>
	std::string& Datum::Get<std::string>(std::uint32_t index) const
	{
		if (mType != DatumType::String)
			throw std::exception("Datum is not of String type");

		if (index >= mSize)
			throw std::exception("Out of bounds");

		return mContainer.mStringValues[index];
	}

	template<>
	glm::vec4& Datum::Get<glm::vec4>(std::uint32_t index) const
	{
		if (mType != DatumType::Vector)
			throw std::exception("Datum is not of Vector type");

		if (index >= mSize)
			throw std::exception("Out of bounds");

		return mContainer.mVectorValues[index];
	}

	template<>
	glm::mat4& Datum::Get<glm::mat4>(std::uint32_t index) const
	{
		if (mType != DatumType::Matrix)
			throw std::exception("Datum is not of Matrix type");

		if (index >= mSize)
			throw std::exception("Out of bounds");

		return mContainer.mMatrixValues[index];
	}

	template<>
	RTTI*& Datum::Get<RTTI*>(std::uint32_t index) const
	{
		if (mType != DatumType::Pointer)
			throw std::exception("Datum is not of Pointer type");

		if (index >= mSize)
			throw std::exception("Out of bounds");

		return mContainer.mPointerValues[index];
	}

	template<>
	Scope*& Datum::Get<Scope*>(std::uint32_t index) const
	{
		if (mType != DatumType::Table)
			throw std::exception("Datum is not of Scope type");

		if (index >= mSize)
			throw std::exception("Out of bounds");

		return mContainer.mScopeValues[index];
	}

#pragma endregion

#pragma region Constructors

	Datum::Datum() :
		mType(DatumType::Unknown), mCapacity(0), mSize(0), mIsExternal(false)
	{}

	Datum::Datum(const Datum & rhs)
	{
		operator=(rhs);
	}

	Datum::Datum(Datum&& rhs) :
		mType(rhs.mType), mCapacity(rhs.mCapacity), mSize(rhs.mSize), mIsExternal(rhs.mIsExternal), mContainer(rhs.mContainer)
	{
		rhs.mContainer.voidPointer = nullptr;
	}

#pragma endregion

#pragma region Deleting/Destroying Data

	Datum::~Datum()
	{
		if(!mIsExternal)
			Clear();
	}

	void Datum::Clear()
	{
		throwIfExternal();
		
		if (mContainer.voidPointer == nullptr)
			return;

		while (!IsEmpty())
		{
			PopBack();
		}

		free(mContainer.voidPointer);

		mCapacity = 0;
		mContainer.voidPointer = nullptr;
	}

	void Datum::PopBack()
	{
		throwIfUnknown();
		throwIfExternal();

		if (IsEmpty())
			return;

		switch (mType)
		{
			case DatumType::String:
				mContainer.mStringValues[mSize - 1].std::string::~string();
				break;
			case DatumType::Vector:
				mContainer.mVectorValues[mSize - 1].glm::vec4::~vec4();
				break;
			case DatumType::Matrix:
				mContainer.mMatrixValues[mSize - 1].glm::mat4::~mat4();
				break;
			}

		--mSize;
	}

#pragma endregion

#pragma region Assignment Operator Overloads

	Datum& Datum::operator=(Datum&& rhs)
	{
		if (this != &rhs)
		{
			if (!mIsExternal)
				Clear();
			
			mIsExternal = rhs.mIsExternal;
			mContainer = rhs.mContainer;
			mSize = rhs.mSize;
			mCapacity = rhs.mCapacity;
			mType = rhs.mType;

			rhs.mContainer.voidPointer = nullptr;
		}
		return *this;
	}

	Datum& Datum::operator=(const Datum& rhs)
	{
		if (this != &rhs)
		{
			if (!mIsExternal)
				Clear();

			mType = rhs.mType;

			if (!rhs.mIsExternal)
			{
				if (mIsExternal)
				{
					mContainer.voidPointer = nullptr;
					mSize = 0;
					mCapacity = 0;
				}

				mIsExternal = false;

				if (mType != DatumType::Unknown)
				{
					Reserve(rhs.Capacity());
					switch (mType)
					{
						case DatumType::Integer: copyContents<int>(rhs); break;
						case DatumType::Float: copyContents<float>(rhs); break;
						case DatumType::String: copyContents<std::string>(rhs); break;
						case DatumType::Vector: copyContents<glm::vec4>(rhs); break;
						case DatumType::Matrix: copyContents<glm::mat4>(rhs); break;
						case DatumType::Pointer: copyContents<RTTI*>(rhs); break;
						case DatumType::Table: copyScopes(rhs); break;
					}
				}
			}
			else
			{
				mContainer = rhs.mContainer;
				mSize = rhs.mSize;
				mCapacity = rhs.mCapacity;
				mIsExternal = true;
			}
		}
		return *this;
	}

	Datum& Datum::operator=(const int& rhs)
	{
		SetType(DatumType::Integer);
		if (mSize == 0)
			PushBack(rhs);
		else
			Set(rhs, 0);
		return *this;
	}

	Datum& Datum::operator=(const float& rhs)
	{
		SetType(DatumType::Float);
		if (mSize == 0)
			PushBack(rhs);
		else
			Set(rhs, 0);
		return *this;
	}

	Datum& Datum::operator=(const std::string& rhs)
	{
		SetType(DatumType::String);
		if (mSize == 0)
			PushBack(rhs);
		else
			Set(rhs, 0);
		return *this;
	}

	Datum& Datum::operator=(const glm::vec4& rhs)
	{
		SetType(DatumType::Vector);
		if (mSize == 0)
			PushBack(rhs);
		else
			Set(rhs, 0);
		return *this;
	}

	Datum& Datum::operator=(const glm::mat4& rhs)
	{
		SetType(DatumType::Matrix);
		if (mSize == 0)
			PushBack(rhs);
		else
			Set(rhs, 0);
		return *this;
	}

	Datum& Datum::operator=(RTTI* rhs)
	{
		SetType(DatumType::Pointer);
		if (mSize == 0)
			PushBack(rhs);
		else
			Set(rhs, 0);
		return *this;
	}

	Datum& Datum::operator=(Scope& rhs)
	{
		SetType(DatumType::Table);
		if (mSize == 0)
			PushBack(rhs);
		else
			Set(rhs, 0);
		return *this;
	}

#pragma endregion

#pragma region Type Methods

	Datum::DatumType Datum::Type() const
	{
		return mType;
	}

	void Datum::SetType(DatumType type)
	{
		if (type == DatumType::Unknown || type == DatumType::End)
			throw std::exception("Not setting to valid type.");

		if (mType == DatumType::Unknown)
			mType = type;

		if (mType != type)
			throw std::exception("Can't change type of Datum after it's already been set.");
	}

#pragma endregion

#pragma region Size/Capacity Methods

	std::uint32_t Datum::Size() const
	{
		return mSize;
	}

	std::uint32_t Datum::Capacity() const
	{
		return mCapacity;
	}

	bool Datum::IsEmpty() const
	{
		return mSize == 0U;
	}

	void Datum::Reserve(std::uint32_t capacity)
	{
		throwIfUnknown();
		throwIfExternal();

		if (capacity <= mCapacity)
			return;

		switch (mType)
		{
			case DatumType::Integer:
				mContainer.mIntValues = reinterpret_cast<int*>(std::realloc(mContainer.mIntValues, capacity * sizeof(int)));
				break;
			case DatumType::Float:
				mContainer.mFloatValues = reinterpret_cast<float*>(std::realloc(mContainer.mFloatValues, capacity * sizeof(float)));
				break;
			case DatumType::String:
				mContainer.mStringValues = reinterpret_cast<std::string*>(std::realloc(mContainer.mStringValues, capacity * sizeof(std::string)));
				break;
			case DatumType::Vector:
				mContainer.mVectorValues = reinterpret_cast<glm::vec4*>(std::realloc(mContainer.mVectorValues, capacity * sizeof(glm::vec4)));
				break;
			case DatumType::Matrix:
				mContainer.mMatrixValues = reinterpret_cast<glm::mat4*>(std::realloc(mContainer.mMatrixValues, capacity * sizeof(glm::mat4)));
				break;
			case DatumType::Pointer:
				mContainer.mPointerValues = reinterpret_cast<RTTI**>(std::realloc(mContainer.mPointerValues, capacity * sizeof(RTTI*)));
				break;
			case DatumType::Table:
				mContainer.mScopeValues = reinterpret_cast<Scope**>(std::realloc(mContainer.mScopeValues, capacity * sizeof(Scope*)));
				break;
		}
		mCapacity = capacity;
	}

#pragma endregion

#pragma region Set Storage

	bool Datum::IsExternal()
	{
		return mIsExternal;
	}

	void Datum::SetStorage(int* container, std::uint32_t size)
	{
		SetType(DatumType::Integer);

		if (mContainer.mIntValues != nullptr)
			throw std::exception("Memory has already been allocated for this Datum");

		mIsExternal = true;

		mContainer.mIntValues = container;
		mSize = mCapacity = size;	
	}

	void Datum::SetStorage(float* container, std::uint32_t size)
	{
		SetType(DatumType::Float);

		if (mContainer.mFloatValues != nullptr)
			throw std::exception("Memory has already been allocated for this Datum");

		mIsExternal = true;

		mContainer.mFloatValues = container;
		mSize = mCapacity = size;
	}

	void Datum::SetStorage(std::string* container, std::uint32_t size)
	{
		SetType(DatumType::String);

		if (mContainer.mStringValues != nullptr)
			throw std::exception("Memory has already been allocated for this Datum");

		mIsExternal = true;

		mContainer.mStringValues = container;
		mSize = mCapacity = size;
	}

	void Datum::SetStorage(glm::vec4* container, std::uint32_t size)
	{
		SetType(DatumType::Vector);

		if (mContainer.mVectorValues != nullptr)
			throw std::exception("Memory has already been allocated for this Datum");

		mIsExternal = true;

		mContainer.mVectorValues = container;
		mSize = mCapacity = size;
	}

	void Datum::SetStorage(glm::mat4* container, std::uint32_t size)
	{
		SetType(DatumType::Matrix);

		if (mContainer.mMatrixValues != nullptr)
			throw std::exception("Memory has already been allocated for this Datum");

		mIsExternal = true;

		mContainer.mMatrixValues = container;
		mSize = mCapacity = size;
	}

	void Datum::SetStorage(RTTI** container, std::uint32_t size)
	{
		SetType(DatumType::Pointer);

		if (mContainer.mPointerValues != nullptr)
			throw std::exception("Memory has already been allocated for this Datum");

		mIsExternal = true;

		mContainer.mPointerValues = container;
		mSize = mCapacity = size;
	}

#pragma endregion

#pragma region Equals Operator Overloads

	bool Datum::operator==(const Datum& rhs) const
	{
		if (mSize == rhs.mSize && mCapacity == rhs.mCapacity && mType == rhs.mType)
		{
			switch (mType)
			{
				case DatumType::Unknown: return true; break;
				case DatumType::Integer: return contentsEqual<int>(rhs); break;
				case DatumType::Float: return contentsEqual<float>(rhs); break;
				case DatumType::String: return contentsEqual<std::string>(rhs); break;
				case DatumType::Vector: return contentsEqual<glm::vec4>(rhs); break;
				case DatumType::Matrix: return contentsEqual<glm::mat4>(rhs); break;
				case DatumType::Pointer: return contentsEqual<RTTI*>(rhs); break;
				case DatumType::Table: return scopesEqual(rhs); break;
			}
		}
		return false;
	}

	bool Datum::operator==(const int& rhs) const
	{
		return Get<int>(0) == rhs;
	}

	bool Datum::operator==(const float& rhs) const
	{
		return Get<float>(0) == rhs;
	}

	bool Datum::operator==(const std::string& rhs) const
	{
		return Get<std::string>(0) == rhs;
	}

	bool Datum::operator==(const glm::vec4& rhs) const
	{
		return Get<glm::vec4>(0) == rhs;
	}

	bool Datum::operator==(const glm::mat4& rhs) const
	{
		return Get<glm::mat4>(0) == rhs;
	}

	bool Datum::operator==(RTTI* rhs) const
	{
		return Get<RTTI*>(0) == rhs;
	}

	bool Datum::operator==(Scope& rhs) const
	{
		return *Get<Scope*>(0) == rhs;
	}

#pragma endregion

#pragma region Not Equals Operator Overloads

	bool Datum::operator!=(const Datum& rhs) const
	{
		return !(operator==(rhs));
	}

	bool Datum::operator!=(const int& rhs) const
	{
		return !(operator==(rhs));
	}

	bool Datum::operator!=(const float& rhs) const
	{
		return !(operator==(rhs));
	}

	bool Datum::operator!=(const std::string& rhs) const
	{
		return !(operator==(rhs));
	}

	bool Datum::operator!=(const glm::vec4& rhs) const
	{
		return !(operator==(rhs));
	}

	bool Datum::operator!=(const glm::mat4& rhs) const
	{
		return !(operator==(rhs));
	}

	bool Datum::operator!=(RTTI* rhs) const
	{
		return !(operator==(rhs));
	}

	bool Datum::operator!=(Scope& rhs) const
	{
		return !(operator==(rhs));
	}

#pragma endregion

#pragma region Set Values

	void Datum::Set(const int& value, std::uint32_t index)
	{
		SetType(DatumType::Integer);

		if (index >= mSize)
			throw std::exception("Out of bounds");

		mContainer.mIntValues[index] = value;
	}

	void Datum::Set(const float& value, std::uint32_t index)
	{
		SetType(DatumType::Float);

		if (index >= mSize)
			throw std::exception("Out of bounds");

		mContainer.mFloatValues[index] = value;
	}

	void Datum::Set(const std::string& value, std::uint32_t index)
	{
		SetType(DatumType::String);

		if (index >= mSize)
			throw std::exception("Out of bounds");

		mContainer.mStringValues[index] = value;
	}

	void Datum::Set(const glm::vec4& value, std::uint32_t index)
	{
		SetType(DatumType::Vector);

		if (index >= mSize)
			throw std::exception("Out of bounds");

		mContainer.mVectorValues[index] = value;
	}

	void Datum::Set(const glm::mat4& value, std::uint32_t index)
	{
		SetType(DatumType::Matrix);

		if (index >= mSize)
			throw std::exception("Out of bounds");

		mContainer.mMatrixValues[index] = value;
	}

	void Datum::Set(RTTI* value, std::uint32_t index)
	{
		SetType(DatumType::Pointer);

		if (index >= mSize)
			throw std::exception("Out of bounds");

		mContainer.mPointerValues[index] = value;
	}

	void Datum::Set(Scope& value, std::uint32_t index)
	{
		SetType(DatumType::Table);

		if (index >= mSize)
			throw std::exception("Out of bounds");

		mContainer.mScopeValues[index] = &value;
	}

#pragma endregion 

#pragma region Push Back

	void Datum::PushBack(const int& value)
	{
		throwIfExternal();
		SetType(DatumType::Integer);
		if (mSize >= mCapacity)
		{
			Reserve(mCapacity + 1);
		}
		mContainer.mIntValues[mSize] = value;
		mSize++;
	}

	void Datum::PushBack(const float& value)
	{
		throwIfExternal();
		SetType(DatumType::Float);
		if (mSize >= mCapacity)
		{
			Reserve(mCapacity + 1);
		}
		mContainer.mFloatValues[mSize] = value;
		mSize++;
	}

	void Datum::PushBack(const std::string& value)
	{
		throwIfExternal();
		SetType(DatumType::String);
		if (mSize >= mCapacity)
		{
			Reserve(mCapacity + 1);
		}
		new (&mContainer.mStringValues[mSize]) std::string(value);
		mSize++;
	}

	void Datum::PushBack(const glm::vec4& value)
	{
		throwIfExternal();
		SetType(DatumType::Vector);
		if (mSize >= mCapacity)
		{
			Reserve(mCapacity + 1);
		}
		new (&mContainer.mVectorValues[mSize]) glm::vec4 (value);
		mSize++;
	}

	void Datum::PushBack(const glm::mat4& value)
	{
		throwIfExternal();
		SetType(DatumType::Matrix);
		if (mSize >= mCapacity)
		{
			Reserve(mCapacity + 1);
		}
		new (&mContainer.mMatrixValues[mSize]) glm::mat4(value);
		mSize++;
	}

	void Datum::PushBack(RTTI* value)
	{
		throwIfExternal();
		SetType(DatumType::Pointer);
		if (mSize >= mCapacity)
		{
			Reserve(mCapacity + 1);
		}
		mContainer.mPointerValues[mSize] = value;
		mSize++;
	}

	void Datum::PushBack(Scope& value)
	{
		throwIfExternal();
		SetType(DatumType::Table);
		if (mSize >= mCapacity)
		{
			Reserve(mCapacity + 1);
		}
		mContainer.mScopeValues[mSize] = &value;
		mSize++;
	}

#pragma endregion

#pragma region String Methods

	void Datum::SetFromString(char* value, DatumType type, std::uint32_t index)
	{
		glm::vec4 vec;
		glm::mat4 mat;
		switch (type)
		{
			case DatumType::Integer:
				int parsedInt;
				sscanf_s(value, "%i", &parsedInt);
				Set(parsedInt, index);
				break;
			case DatumType::Float:
				float parsedFloat;
				sscanf_s(value, "%f", &parsedFloat);
				Set(parsedFloat, index);
				break;
			case DatumType::String:
				Set(std::string(value), index);
				break;
			case DatumType::Vector:
				sscanf_s(value, "%f %f %f %f", &vec.x, &vec.y, &vec.z, &vec.w);
				Set(vec, index);
				break;
			case DatumType::Matrix:
				sscanf_s(value, "%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f",
					&mat[0][0], &mat[0][1], &mat[0][2], &mat[0][3], &mat[1][0], &mat[1][1], &mat[1][2], &mat[1][3],
					&mat[2][0], &mat[2][1], &mat[2][2], &mat[2][3], &mat[3][0], &mat[3][1], &mat[3][2], &mat[3][3]);
				Set(mat, index);
				break;
			case DatumType::Pointer:
				throw std::exception("Cannot use string representation of a pointer");
		}
	}

	std::string Datum::ToString(std::uint32_t index)
	{
		if (mType == DatumType::Unknown)
			throw std::exception("Type has not yet been set");

		char buffer[512] = { '\0' };
		glm::mat4 mat;
		glm::vec4 vec;
		switch (mType)
		{
		case DatumType::Integer:
			sprintf_s(buffer, sizeof(buffer), "%i", Get<int>(index));
			break;
		case DatumType::Float:
			sprintf_s(buffer, sizeof(buffer), "%f", Get<float>(index));
			break;
		case DatumType::Vector:
			vec = Get<glm::vec4>(index);
			sprintf_s(buffer, sizeof(buffer), "%f %f %f %f", vec[0], vec[1], vec[2], vec[3]);
			break;
		case DatumType::Matrix:
			mat = Get<glm::mat4>(index);
			sprintf_s(buffer, sizeof(buffer), "%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f",
				mat[0][0], mat[0][1], mat[0][2], mat[0][3], mat[1][0], mat[1][1], mat[1][2], mat[1][3],
				mat[2][0], mat[2][1], mat[2][2], mat[2][3], mat[3][0], mat[3][1], mat[3][2], mat[3][3]);
			break;
		case DatumType::String:
			return Get<std::string>(index);
		case DatumType::Pointer:
			return Get<RTTI*>(index)->ToString();
		}
		return std::string(buffer);
	}

#pragma endregion

#pragma region Index Operator
	Scope& Datum::operator[](std::uint32_t index) 
	{ 
		return *Get<Scope*>(index); 
	}
#pragma endregion

#pragma region Find

	std::uint32_t Datum::Find(const int& value)
	{
		throwIfUnknown();
		SetType(DatumType::Integer);

		for (std::uint32_t i = 0; i < mSize; ++i)
		{
			if (mContainer.mIntValues[i] == value)
				return i;
		}
		return mSize;
	}

	std::uint32_t Datum::Find(const float& value)
	{
		throwIfUnknown();
		SetType(DatumType::Float);

		for (std::uint32_t i = 0; i < mSize; ++i)
		{
			if (mContainer.mFloatValues[i] == value)
				return i;
		}
		return mSize;
	}

	std::uint32_t Datum::Find(const std::string& value)
	{
		throwIfUnknown();
		SetType(DatumType::String);

		for (std::uint32_t i = 0; i < mSize; ++i)
		{
			if (mContainer.mStringValues[i] == value)
				return i;
		}
		return mSize;
	}

	std::uint32_t Datum::Find(const glm::vec4& value)
	{
		throwIfUnknown();
		SetType(DatumType::Vector);

		for (std::uint32_t i = 0; i < mSize; ++i)
		{
			if (mContainer.mVectorValues[i] == value)
				return i;
		}
		return mSize;
	}

	std::uint32_t Datum::Find(const glm::mat4& value)
	{
		throwIfUnknown();
		SetType(DatumType::Matrix);

		for (std::uint32_t i = 0; i < mSize; ++i)
		{
			if (mContainer.mMatrixValues[i] == value)
				return i;
		}
		return mSize;
	}

	std::uint32_t Datum::Find(RTTI* value)
	{
		throwIfUnknown();
		SetType(DatumType::Pointer);

		for (std::uint32_t i = 0; i < mSize; ++i)
		{
			if (mContainer.mPointerValues[i]->Equals(value))
				return i;
		}
		return mSize;
	}

	std::uint32_t Datum::Find(Scope& value)
	{
		throwIfUnknown();
		SetType(DatumType::Table);

		for (std::uint32_t i = 0; i < mSize; ++i)
		{
			if (mContainer.mScopeValues[i]->Equals(&value))
				return i;
		}
		return mSize;
	}

#pragma endregion

#pragma region Remove

	void Datum::RemoveAt(std::uint32_t index)
	{
		throwIfExternal();
		throwIfUnknown();
		SetType(DatumType::Table);

		if (index >= mSize)
			return;
		else if (index < mSize - 1)
			std::memmove(&mContainer.mScopeValues[index], &mContainer.mScopeValues[index + 1], sizeof(Scope*) * (mSize - index - 1));

		--mSize;
	}

	void Datum::Remove(const int& value)
	{
		throwIfExternal();

		std::uint32_t index = Find(value);
		if (index >= mSize)
			return;
		else if(index < mSize - 1)
			std::memmove(&mContainer.mIntValues[index], &mContainer.mIntValues[index + 1], sizeof(int) * (mSize - index - 1));
		
		--mSize;
	}

	void Datum::Remove(const float& value)
	{
		throwIfExternal();

		std::uint32_t index = Find(value);
		if (index >= mSize)
			return;
		else if (index < mSize - 1)
			std::memmove(&mContainer.mFloatValues[index], &mContainer.mFloatValues[index + 1], sizeof(float) * (mSize - index - 1));

		--mSize;
	}

	void Datum::Remove(const std::string& value)
	{
		throwIfExternal();

		std::uint32_t index = Find(value);
		if (index >= mSize)
			return;

		mContainer.mStringValues[index].std::string::~string();
		if (index < mSize - 1)
			std::memmove(&mContainer.mStringValues[index], &mContainer.mStringValues[index + 1], sizeof(std::string) * (mSize - index - 1));

		--mSize;
	}

	void Datum::Remove(const glm::vec4& value)
	{
		throwIfExternal();

		std::uint32_t index = Find(value);
		if (index >= mSize)
			return;
		else if (index < mSize - 1)
			std::memmove(&mContainer.mVectorValues[index], &mContainer.mVectorValues[index + 1], sizeof(glm::vec4) * (mSize - index - 1));

		--mSize;
	}

	void Datum::Remove(const glm::mat4& value)
	{
		throwIfExternal();

		std::uint32_t index = Find(value);
		if (index >= mSize)
			return;
		else if (index < mSize - 1)
			std::memmove(&mContainer.mMatrixValues[index], &mContainer.mMatrixValues[index + 1], sizeof(glm::mat4) * (mSize - index - 1));

		--mSize;
	}

	void Datum::Remove(RTTI* value)
	{
		throwIfExternal();

		std::uint32_t index = Find(value);
		if (index >= mSize)
			return;
		else if (index < mSize - 1)
			std::memmove(&mContainer.mPointerValues[index], &mContainer.mPointerValues[index + 1], sizeof(RTTI*) * (mSize - index - 1));

		--mSize;
	}

	void Datum::Remove(Scope& value)
	{
		throwIfExternal();

		std::uint32_t index = Find(value);
		if (index >= mSize)
			return;
		else if (index < mSize - 1)
			std::memmove(&mContainer.mScopeValues[index], &mContainer.mScopeValues[index + 1], sizeof(Scope*) * (mSize - index - 1));

		--mSize;
	}

#pragma endregion 

}
