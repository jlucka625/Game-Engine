#pragma once
#include <utility>

namespace Library
{

#pragma region Functors
	template <typename TKey>
	class DefaultEqualityFunctor
	{
	public:
		inline bool operator()(const TKey& lhs, const TKey& rhs);
	};

	template<>
	class DefaultEqualityFunctor<char*>
	{
	public:
		inline bool operator()(const char* lhs, const char* rhs);
	};

	template <typename TKey>
	//! Default Function Object Class
	/*! Represents a function object that produces a hash value for
	any key type.*/
	class DefaultFunctor
	{
	public:
		//! Parenthesis Overload
		/*! Converts key data into an array of bytes and adds them together.
		\param key The key to be hashed.
		\param size How many buckets are in the hash map.
		\return A hash value to be used as an index into the map*/		
		inline std::uint32_t operator()(const TKey& key, std::uint32_t size);
	};

	template<>
	//! String Specialized Function Object Class
	/*! Represents a function object that produces a hash value for
	a string.*/
	class DefaultFunctor<std::string>
	{
	public:
		//! Parenthesis Overload
		/*! Adds the ASCII value of each character in the string together.
		\param key The key to be hashed.
		\param size How many buckets are in the hash map.
		\return A hash value to be used as an index into the map*/
		inline std::uint32_t operator()(const std::string& key, std::uint32_t size);
	};

	template<>
	//! Char Array Specialized Function Object Class
	/*! Represents a function object that produces a hash value for
	a char array.*/
	class DefaultFunctor<char*>
	{
	public:
		//! Parenthesis Overload
		/*! Adds the ASCII value of each character in the array together.
		\param key The key to be hashed.
		\param size How many buckets are in the hash map.
		\return A hash value to be used as an index into the map*/
		inline std::uint32_t operator()(const char* key, std::uint32_t size);
	};
#pragma endregion

#pragma region HashMapTemplate
	template <typename TKey, typename TData, typename HashFunctor = DefaultFunctor<TKey>, typename Equality = DefaultEqualityFunctor<TKey>>
	//! Hash Map Class
	/*! A class template for an unordered Hash Map using chaining. Constant time lookups and insertions
	are not guarunteed. Supports all element types for key, value pairs.*/
	class HashMap
	{
	public:
		typedef std::pair<TKey, TData> PairType;

	private:
		typedef SList<PairType> ChainType;
		typedef Vector<ChainType> BucketType;

	public:
		//! Hash Map Iterator Class
		/*! Used to traverse through an unordered hash map*/
		class Iterator final
		{
			friend class HashMap<TKey, TData, HashFunctor, Equality>;
		public:
			//! Default Constructor
			/*! Creates an iterator object that is not associate with
			any map instance.*/
			Iterator();

			//! Copy Constructor
			/*! Creates a deep-copy of the iterator.
			\param rhs The iterator to copy.*/
			Iterator(const Iterator& rhs);

			//! Move Constructor
			/*! Performs move semantics on the iterator
			\param rhs The iterator to move.*/
			Iterator(Iterator&& rhs);

			//! Copy Assignment Operator Overload
			/*! Creates a deep-copy of the iterator.
			\param rhs The iterator to copy.
			\return A reference to the copied iterator.*/
			Iterator& operator=(const Iterator& rhs);

			//! Move Assignment Operator Overload
			/*! Performs move semantics on the iterator
			\param rhs The iterator to move.
			\return A reference to the moved iterator.*/
			Iterator& operator=(Iterator&& rhs);

			//! Default Destructor
			~Iterator() = default;

			//! Pre-Increment Overload
			/* Immediately increments the Iterator to the available key, value
			pair in the map.
			\return A reference to the incremented iterator*/
			Iterator& operator++();

			//! Post-Increment Overload
			/* Copies the iterator and then increments it to the next available
			key, value pair. int is a dummy value used to remove ambiguity between pre 
			and post fix.
			\return a copy of the Iterator before it has been incremented*/
			Iterator operator++(int);

			//! Dereference Overload
			/* \return A reference to the pair that this Iterator's is pointing to*/
			typename PairType& operator*() const;

			//! Arrow Dereference Overload
			/* \return A pointer to the pair that this Iterator's is pointing to*/
			typename PairType* operator->() const;

			//! Equality Overload
			/* \param rhs Iterator to compare against
			\return boolean value that represents if iterators are owned by the
			same map and refer to the same key,value pair */
			bool operator==(const Iterator& rhs) const;

			//! Inequality Overload
			/* \param rhs Iterator to compare against
			\return boolean value that represents if both iterators are not
			owned by the same map or do not refer to the same key,valye pair */
			bool operator!=(const Iterator& rhs) const;
		private:
			//! Parameter List Constructor
			/*! Creates an iterator that refers to a specific owner map instance.
			\param index Which bucket in the map that the element is stored in
			\param listIterator Which pair in the bucket this iterator is referring to
			\param map The hash map that owns this iterator.*/
			Iterator(std::uint32_t index, typename ChainType::Iterator listIterator, const HashMap* map);	
			std::uint32_t mIndex;																			//!< Which bucket in the map that the element is stored in
			typename ChainType::Iterator mListIterator;														//!< listIterator Which pair in the bucket this iterator is referring to
			const HashMap* mMap;																			//!< The hash map that owns this iterator.
		};
		//! Parameter List Constructor
		/*! \param capacity The initial capacity of the Hash Map.*/
		explicit HashMap(std::uint32_t capacity = 11);

		//! Default Copy Constructor
		HashMap(const HashMap& rhs) = default;

		//! Move Constructor
		HashMap(HashMap&& rhs);

		//! Destructor
		/*! Clears out the HashMap*/
		~HashMap() = default;

		//! Default Copy Assignment Operator
		HashMap& operator=(const HashMap& rhs) = default;

		//! Move Assignment Operator
		HashMap& operator=(HashMap&& rhs);

		//! Find Key
		/*! \param key The key that we are searching for within the map.
		\return An iterator referring to the pair that contains the passed in key.*/
		Iterator Find(const TKey& key) const;

		//! Insert Pair
		/*! \param item The key, value pair to insert into the list.
		\return A Hash Map Iterator pointing to the newly inserted pair. This
		method will just return an Iterator pointing to a key, value pair if 
		you try to insert a key that already exists.*/
		Iterator Insert(const PairType& item);

		Iterator Insert(const PairType& item, bool& existed);

		//! Subscript Operator Overload
		/*! Creates new key entry if the passed in key does not exist in the map.
		\param key The key to find/insert.
		\return A reference to the data stored at that key.*/
		TData& operator[](const TKey& key);

		//! Remove Pair
		/*! \param key The key that identifies which pair you would like to remove.
		Calling remove on a key that is not in the map will do nothing.*/
		void Remove(const TKey& key);

		//! Clear Map
		/*! Empties all of the contents of the Hash Map. Hash Map will stay at
		it's current capacity.*/
		void Clear();

		//! Get Size
		/*! \return How many key,value pairs are stored in the map.*/
		std::uint32_t Size() const;

		std::uint32_t Capacity() const;

		//! Search for Key
		/*! \param key The key that we are searching for.
		\return A boolean representing if the provided key was found
		in the map or not.*/
		bool ContainsKey(const TKey& key) const;

		//! Iterator Begin
		/*! \return An Iterator pointing to the first available key, value
		pair in the map.*/
		Iterator begin() const;

		//! Iterator End
		/*! \return An Iterator pointing past the end of the map.*/
		Iterator end() const;

		//! Load Factor
		/*! \return A floating point value between 0.0 and 1.0 showing
		how many elements are stored within the map compared to the
		maps capacity.*/
		float LoadFactor();

		//! Resize the Map
		/*! Expands/Shrinks the hash map and re-hashes the keys currently
		stored in the map.
		\param capacity The new capacity of the hash map*/
		void Rehash(std::uint32_t capacity);

	private:
		BucketType mBuckets;		//!< Vector object that stores each chain
		std::uint32_t mSize;		//!< Keeps track of how many key, value pairs are in the map
		std::uint32_t mCapacity;
};
#pragma endregion

}

#include "HashMap.inl"