#include "HashMap.h"
namespace Library
{	

#pragma region Hash Function Specializations
	template<typename TKey>
	inline bool DefaultEqualityFunctor<TKey>::operator()(const TKey& lhs, const TKey& rhs)
	{
		return lhs == rhs;
	}

	inline bool DefaultEqualityFunctor<char*>::operator()(const char* lhs, const char* rhs)
	{
		return strcmp(lhs, rhs) == 0;
	}

	template<typename TKey>
	inline std::uint32_t DefaultFunctor<TKey>::operator()(const TKey & key, std::uint32_t size)
	{
		const unsigned char* bytes = reinterpret_cast<const unsigned char*>(&key);
		int sum = 0;
		for (std::uint32_t i = 0; i < sizeof(key); ++i)
		{
			sum += bytes[i];
		}
		return sum % size;
	}

	inline std::uint32_t DefaultFunctor<std::string>::operator()(const std::string & key, std::uint32_t size)
	{
		int sum = 0;
		for (std::uint32_t i = 0; i < key.length(); ++i)
		{
			sum += key.at(i);
		}
		return sum % size;
	}

	inline std::uint32_t DefaultFunctor<char*>::operator()(const char* key, std::uint32_t size)
	{
		int sum = 0;
		for (std::uint32_t i = 0; i < strlen(key); ++i)
		{
			sum += key[i];
		}
		return sum % size;
	}
#pragma endregion

#pragma region Iterator
	template <typename TKey, typename TData, typename HashFunctor, typename Equality>
	HashMap<TKey, TData, HashFunctor, Equality>::Iterator::Iterator() :
		mIndex(0), mListIterator(ChainType::Iterator()), mMap(nullptr)
	{}

	template <typename TKey, typename TData, typename HashFunctor, typename Equality>
	HashMap<TKey, TData, HashFunctor, Equality>::Iterator::Iterator(std::uint32_t index, typename ChainType::Iterator listIterator, const HashMap* map) :
		mIndex(index), mListIterator(listIterator), mMap(map)
	{}

	template <typename TKey, typename TData, typename HashFunctor, typename Equality>
	HashMap<TKey, TData, HashFunctor, Equality>::Iterator::Iterator(Iterator&& rhs) :
		mIndex(rhs.mIndex), mListIterator(rhs.mListIterator), mMap(rhs.mMap)
	{
		rhs.mIndex = 0;
		rhs.mMap = nullptr;
	}

	template <typename TKey, typename TData, typename HashFunctor, typename Equality>
	HashMap<TKey, TData, HashFunctor, Equality>::Iterator::Iterator(const Iterator& rhs) :
		Iterator()
	{
		operator=(rhs);
	}

	template <typename TKey, typename TData, typename HashFunctor, typename Equality>
	typename HashMap<TKey, TData, HashFunctor, Equality>::Iterator& HashMap<TKey, TData, HashFunctor, Equality>::Iterator::operator=(const Iterator& rhs)
	{
		if (this != &rhs)
		{
			mIndex = rhs.mIndex;
			mListIterator = rhs.mListIterator;
			mMap = rhs.mMap;
		}
		return *this;
	}

	template <typename TKey, typename TData, typename HashFunctor, typename Equality>
	typename HashMap<TKey, TData, HashFunctor, Equality>::Iterator& HashMap<TKey, TData, HashFunctor, Equality>::Iterator::operator=(Iterator&& rhs)
	{
		if (this != &rhs)
		{
			mIndex = rhs.mIndex;
			mListIterator = rhs.mListIterator;
			mMap = rhs.mMap;

			rhs.mIndex = 0;
			rhs.mMap = nullptr;
		}
		return *this;
	}

	template <typename TKey, typename TData, typename HashFunctor, typename Equality>
	typename HashMap<TKey, TData, HashFunctor, Equality>::Iterator& HashMap<TKey, TData, HashFunctor, Equality>::Iterator::operator++()
	{
		if (mMap == nullptr || *this == mMap->end())
			throw std::exception("Reached out of bounds");

		if (mListIterator != mMap->mBuckets[mIndex].end())
		{
			++mListIterator;
		}
		
		if(mListIterator == mMap->mBuckets[mIndex].end())
		{
			do 
			{
				++mIndex;
			} 
			while (mIndex < mMap->mBuckets.Size() && mMap->mBuckets[mIndex].IsEmpty());

			if (mIndex >= mMap->mBuckets.Size())
				*this = mMap->end();
			else
				mListIterator = mMap->mBuckets[mIndex].begin();
		}
		return *this;
	}

	template <typename TKey, typename TData, typename HashFunctor, typename Equality>
	typename HashMap<TKey, TData, HashFunctor, Equality>::Iterator HashMap<TKey, TData, HashFunctor, Equality>::Iterator::operator++(int)
	{
		Iterator it = *this;
		operator++();
		return it;
	}

	template <typename TKey, typename TData, typename HashFunctor, typename Equality>
	typename HashMap<TKey, TData, HashFunctor, Equality>::PairType& HashMap<TKey, TData, HashFunctor, Equality>::Iterator::operator*() const
	{
		if (mMap == nullptr || *this == mMap->end())
		{
			throw std::exception("Invalid iterator");
		}
		return *mListIterator;
	}

	template <typename TKey, typename TData, typename HashFunctor, typename Equality>
	typename HashMap<TKey, TData, HashFunctor, Equality>::PairType* HashMap<TKey, TData, HashFunctor, Equality>::Iterator::operator->() const
	{
		return &operator*();
	}

	template <typename TKey, typename TData, typename HashFunctor, typename Equality>
	bool HashMap<TKey, TData, HashFunctor, Equality>::Iterator::operator==(const Iterator& rhs) const
	{
		return (mMap == rhs.mMap && mIndex == rhs.mIndex && mListIterator == rhs.mListIterator);
	}

	template <typename TKey, typename TData, typename HashFunctor, typename Equality>
	bool HashMap<TKey, TData, HashFunctor, Equality>::Iterator::operator!=(const Iterator& rhs) const
	{
		return !(operator==(rhs));
	}
#pragma endregion

#pragma region HashMap
	template <typename TKey, typename TData, typename HashFunctor, typename Equality>
	HashMap<TKey, TData, HashFunctor, Equality>::HashMap(std::uint32_t capacity) :
		mSize(0), mCapacity(capacity)
	{
		if (capacity == 0U)
			throw std::exception("Cannot have an initial size of zero.");

		mBuckets = BucketType(capacity, true);
	}

	template <typename TKey, typename TData, typename HashFunctor, typename Equality>
	HashMap<TKey, TData, HashFunctor, Equality>::HashMap(HashMap&& rhs) :
		mSize(rhs.mSize), mBuckets(rhs.mBuckets), mCapacity(rhs.mCapacity)
	{}

	template <typename TKey, typename TData, typename HashFunctor, typename Equality>
	HashMap<TKey, TData, HashFunctor, Equality>& HashMap<TKey, TData, HashFunctor, Equality>::operator=(HashMap&& rhs)
	{
		if (this != &rhs)
		{
			mSize = rhs.mSize;
			mBuckets = std::move(rhs.mBuckets);
			mCapacity = rhs.mCapacity;
		}
		return *this;
	}

	template <typename TKey, typename TData, typename HashFunctor, typename Equality>
	typename HashMap<TKey, TData, HashFunctor, Equality>::Iterator HashMap<TKey, TData, HashFunctor, Equality>::Find(const TKey& key) const
	{
		static HashFunctor hashFunction;
		static Equality equals;
		std::uint32_t index = hashFunction(key, mBuckets.Size());

		ChainType::Iterator it = mBuckets.At(index).begin();
		for (; it != mBuckets.At(index).end(); ++it)
		{
			if(equals((*it).first, key))
			{
				return Iterator(index, it, this);
			}
		}
		return end();
	}

	template <typename TKey, typename TData, typename HashFunctor, typename Equality>
	typename HashMap<TKey, TData, HashFunctor, Equality>::Iterator HashMap<TKey, TData, HashFunctor, Equality>::Insert(const PairType& item)
	{
		static HashFunctor hashFunction;
		std::uint32_t index = hashFunction(item.first, mBuckets.Size());
		Iterator it = Find(item.first);

		if (it == end())
		{
			++mSize;
			return Iterator(index, mBuckets.At(index).PushBack(item), this);
		}
		else return it;
	}

	template <typename TKey, typename TData, typename HashFunctor, typename Equality>
	typename HashMap<TKey, TData, HashFunctor, Equality>::Iterator HashMap<TKey, TData, HashFunctor, Equality>::Insert(const PairType& item, bool& existed)
	{
		static HashFunctor hashFunction;
		std::uint32_t index = hashFunction(item.first, mBuckets.Size());
		Iterator it = Find(item.first);

		if (it == end())
		{
			++mSize;
			existed = false;
			return Iterator(index, mBuckets.At(index).PushBack(item), this);
		}
		else
		{
			existed = true;
			return it;
		}
	}

	template <typename TKey, typename TData, typename HashFunctor, typename Equality>
	TData& HashMap<TKey, TData, HashFunctor, Equality>::operator[](const TKey& key)
	{
		Iterator it = Insert(PairType(key, TData()));
		return it->second;
	}

	template <typename TKey, typename TData, typename HashFunctor, typename Equality>
	void HashMap<TKey, TData, HashFunctor, Equality>::Remove(const TKey& key)
	{
		HashMap::Iterator it = Find(key);
		if (it != end())
		{
			mBuckets[it.mIndex].Remove(*it);
			--mSize;
		}
	}

	template <typename TKey, typename TData, typename HashFunctor, typename Equality>
	void HashMap<TKey, TData, HashFunctor, Equality>::Clear()
	{
		for(auto& bucket : mBuckets)
		{
			bucket.Clear();
		}
		mSize = 0;
	}

	template <typename TKey, typename TData, typename HashFunctor, typename Equality>
	std::uint32_t HashMap<TKey, TData, HashFunctor, Equality>::Size() const
	{
		return mSize;
	}

	template <typename TKey, typename TData, typename HashFunctor, typename Equality>
	std::uint32_t HashMap<TKey, TData, HashFunctor, Equality>::Capacity() const
	{
		return mCapacity;
	}

	template <typename TKey, typename TData, typename HashFunctor, typename Equality>
	bool HashMap<TKey, TData, HashFunctor, Equality>::ContainsKey(const TKey& key) const
	{
		return (Find(key) != end());
	}

	template <typename TKey, typename TData, typename HashFunctor, typename Equality>
	typename HashMap<TKey, TData, HashFunctor, Equality>::Iterator HashMap<TKey, TData, HashFunctor, Equality>::begin() const
	{
		std::uint32_t index = 0;
		while (index < mBuckets.Size() && mBuckets[index].IsEmpty())
			++index;

		if (index != mBuckets.Size())
			return Iterator(index, mBuckets.At(index).begin(), this);
		else
			return end();
	}

	template <typename TKey, typename TData, typename HashFunctor, typename Equality>
	typename HashMap<TKey, TData, HashFunctor, Equality>::Iterator HashMap<TKey, TData, HashFunctor, Equality>::end() const
	{
		return Iterator(mBuckets.Size(), ChainType::Iterator(), this);
	}

	template <typename TKey, typename TData, typename HashFunctor, typename Equality>
	float HashMap<TKey, TData, HashFunctor, Equality>::LoadFactor()
	{
		return static_cast<float>(mSize) / mBuckets.Capacity();
	}

	template <typename TKey, typename TData, typename HashFunctor, typename Equality>
	void HashMap<TKey, TData, HashFunctor, Equality>::Rehash(std::uint32_t capacity)
	{
		HashMap temp(capacity);
		for (const auto& pair : *this)
		{
			temp.Insert(pair);
		}
		*this = std::move(temp);
		mCapacity = capacity;
	}

#pragma endregion

}
