namespace Library
{
#pragma region Iterator
	template <typename T>
	Vector<T>::Iterator::Iterator() :
		mIndex(0), mVector(nullptr)
	{}

	template <typename T>
	Vector<T>::Iterator::Iterator(std::uint32_t index, const Vector<T>* vector) :
		mIndex(index), mVector(vector)
	{}

	template <typename T>
	Vector<T>::Iterator::Iterator(const Iterator& rhs)
	{
		operator=(rhs);
	}

	template <typename T>
	Vector<T>::Iterator::Iterator(Iterator&& rhs) :
		mIndex(rhs.mIndex), mVector(rhs.mVector)
	{
		rhs.mIndex = 0;
		rhs.mVector = nullptr;
	}

	template <typename T>
	typename Vector<T>::Iterator& Vector<T>::Iterator::operator=(const Iterator& rhs)
	{
		if (this != &rhs)
		{
			mIndex = rhs.mIndex;
			mVector = rhs.mVector;
		}

		return *this;
	}

	template <typename T>
	typename Vector<T>::Iterator& Vector<T>::Iterator::operator=(Iterator&& rhs)
	{
		if (this != &rhs)
		{
			mIndex = rhs.mIndex;
			mVector = rhs.mVector;

			rhs.mIndex = 0;
			rhs.mVector = nullptr;
		}

		return *this;
	}

	template <typename T>
	bool Vector<T>::Iterator::operator==(const Iterator& rhs) const
	{
		return ((mVector == rhs.mVector) && (mIndex == rhs.mIndex));
	}

	template <typename T>
	bool Vector<T>::Iterator::operator!=(const Iterator& rhs) const
	{
		return !(operator==(rhs));
	}

	template <typename T>
	T& Vector<T>::Iterator::operator*()
	{
		return mVector->At(mIndex);
	}

	template <typename T>
	const T& Vector<T>::Iterator::operator*() const
	{
		return mVector->At(mIndex);
	}

	template <typename T>
	typename Vector<T>::Iterator& Vector<T>::Iterator::operator++()
	{
		if (mVector != nullptr && *this != mVector->end())
		{
			mIndex++;
		}
		else
		{
			throw std::exception("Invalid iterator");
		}

		return *this;
	}

	template <typename T>
	typename Vector<T>::Iterator Vector<T>::Iterator::operator++(int)
	{
		Iterator it = *this;
		operator++();
		return it;
	}
#pragma endregion

#pragma region Vector
	template<typename T>
	Vector<T>::Vector(std::uint32_t capacity, bool fixedSize) :
		mSize(0), mCapacity(0), mContainer(nullptr)
	{
		Reserve(capacity, fixedSize);
	}

	template<typename T>
	Vector<T>::Vector(Vector&& rhs) :
		mSize(0), mCapacity(0), mContainer(nullptr)
	{
		operator=(rhs);
	}

	template<typename T>
	Vector<T>::Vector(const Vector& rhs) :
		mSize(0), mCapacity(0), mContainer(nullptr)
	{
		operator=(rhs);
	}

	template<typename T>
	Vector<T>& Vector<T>::operator=(const Vector& rhs)
	{
		if (this != &rhs)
		{
			Clear();
			Reserve(rhs.Capacity());
			for (auto& item : rhs)
			{
				PushBack(item);
			}
		}
		return *this;
	}

	template<typename T>
	Vector<T>& Vector<T>::operator=(Vector&& rhs)
	{
		if (this != &rhs)
		{
			Clear();

			mSize = rhs.mSize;
			mCapacity = rhs.mCapacity;
			mContainer = rhs.mContainer;

			rhs.mSize = 0;
			rhs.mCapacity = 0;
			rhs.mContainer = nullptr;
		}
		return *this;
	}

	template<typename T>
	Vector<T>::~Vector()
	{
		Clear();
	}

	template<typename T>
	bool Vector<T>::IsEmpty() const
	{
		return mSize == 0;
	}

	template<typename T>
	typename Vector<T>::Iterator Vector<T>::PushBack(const T& item)
	{
		if (mSize >= mCapacity)
		{
			Reserve(mCapacity + 10);
		}
		new (&mContainer[mSize]) T(item);

		return Iterator(mSize++, this);
	}

	template<typename T>
	void Vector<T>::PopBack()
	{
		if (IsEmpty())
			throw std::exception("The list is empty");

		mContainer[--mSize].~T();
	}

	template<typename T>
	T& Vector<T>::Front()
	{
		if (IsEmpty())
			throw std::exception("The Vector is Empty");

		return mContainer[0];
	}

	template<typename T>
	const T& Vector<T>::Front() const
	{
		if (IsEmpty())
			throw std::exception("The Vector is Empty");

		return mContainer[0];
	}

	template<typename T>
	T& Vector<T>::Back()
	{
		if (IsEmpty())
			throw std::exception("The Vector is Empty");

		return mContainer[mSize - 1];
	}

	template<typename T>
	const T& Vector<T>::Back() const
	{
		if (IsEmpty())
			throw std::exception("The Vector is Empty");

		return mContainer[mSize - 1];
	}

	template<typename T>
	std::uint32_t Vector<T>::Size() const
	{
		return mSize;
	}

	template<typename T>
	std::uint32_t Vector<T>::Capacity() const
	{
		return mCapacity;
	}

	template<typename T>
	T& Vector<T>::At(std::uint32_t index) const
	{
		if (index >= mSize)
			throw std::exception("Index out of bounds");

		return mContainer[index];
	}

	template<typename T>
	T& Vector<T>::operator[](std::uint32_t index)
	{
		return At(index);
	}

	template<typename T>
	const T& Vector<T>::operator[](std::uint32_t index) const
	{
		return At(index);
	}

	template <typename T>
	typename Vector<T>::Iterator Vector<T>::begin() const
	{
		if (IsEmpty())
			return end();

		return Iterator(0, this);
	}

	template <typename T>
	typename Vector<T>::Iterator Vector<T>::end() const
	{
		return Iterator(mSize, this);
	}

	template <typename T>
	void Vector<T>::Reserve(std::uint32_t capacity, bool fixedSize)
	{
		if (capacity <= mCapacity)
			return;

		T* newContainer = static_cast<T*>(std::malloc(capacity * sizeof(T)));

		if (mContainer != nullptr)
		{
			std::memcpy(newContainer, mContainer, mSize * sizeof(T));
			free(mContainer);
		}

		mContainer = newContainer;
		mCapacity = capacity;

		if (fixedSize)
		{
			for (std::uint32_t i = mSize; i < mCapacity; ++i)
			{
				new(&mContainer[i]) T();
			}
			mSize = mCapacity;
		}
	}

	template <typename T>
	typename Vector<T>::Iterator Vector<T>::Find(const T& value) const
	{
		Iterator it = begin();
		for (; it != end(); ++it)
		{
			if (*it == value)
			{
				break;
			}
		}
		return it;
	}

	template <typename T>
	void Vector<T>::Clear()
	{
		if (mContainer != nullptr)
		{
			while (!IsEmpty())
			{
				PopBack();
			}
			free(mContainer);
			mContainer = nullptr;
			mCapacity = 0;
		}
	}

	template <typename T>
	void Vector<T>::Remove(const T& item)
	{
		Iterator result = Find(item);
		if (result == end())
			return;

		std::uint32_t index = result.mIndex;
		mContainer[index].~T();
		std::memmove(&mContainer[index], &mContainer[index + 1], sizeof(T) * (mSize - index - 1));

		--mSize;
	}

#pragma endregion

}



