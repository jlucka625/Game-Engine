namespace Library {

#pragma region Node

	template<typename T>
	SList<T>::Node::Node(const T & item, Node* next) :
		mItem(item), mNext(next)
	{}

#pragma endregion

#pragma region Iterator
	template <typename T>
	SList<T>::Iterator::Iterator() :
		mCurrentNode(nullptr), mList(nullptr)
	{}

	template <typename T>
	SList<T>::Iterator::Iterator(Node* currentNode, const SList<T>* list) :
		mCurrentNode(currentNode), mList(list)
	{}

	template <typename T>
	SList<T>::Iterator::Iterator(const Iterator& rhs)
	{
		operator=(rhs);
	}

	template <typename T>
	SList<T>::Iterator::Iterator(Iterator&& rhs) :
		mCurrentNode(rhs.mCurrentNode), mList(rhs.mList)
	{
		rhs.mCurrentNode = nullptr;
		rhs.mList = nullptr;
	}

	template <typename T>
	typename SList<T>::Iterator& SList<T>::Iterator::operator=(const Iterator& rhs)
	{
		if (this != &rhs)
		{
			mCurrentNode = rhs.mCurrentNode;
			mList = rhs.mList;
		}

		return *this;
	}

	template <typename T>
	typename SList<T>::Iterator& SList<T>::Iterator::operator=(Iterator&& rhs)
	{
		if (this != &rhs)
		{
			mCurrentNode = rhs.mCurrentNode;
			mList = rhs.mList;

			rhs.mCurrentNode = nullptr;
			rhs.mList = nullptr;
		}

		return *this;
	}

	template <typename T>
	bool SList<T>::Iterator::operator==(const Iterator& rhs) const
	{
		return ((mList == rhs.mList) && (mCurrentNode == rhs.mCurrentNode));
	}

	template <typename T>
	bool SList<T>::Iterator::operator!=(const Iterator& rhs) const
	{
		return !(operator==(rhs));
	}

	template <typename T>
	T& SList<T>::Iterator::operator*() const
	{
		if (mCurrentNode == nullptr)
			throw std::exception("Attempted to dereference end()");

		return mCurrentNode->mItem;
	}

	template <typename T>
	typename SList<T>::Iterator& SList<T>::Iterator::operator++()
	{
		if (mList != nullptr && *this != mList->end())
		{
			mCurrentNode = mCurrentNode->mNext;
		}
		else
		{
			throw std::exception("Invalid iterator");
		}

		return *this;
	}

	template <typename T>
	typename SList<T>::Iterator SList<T>::Iterator::operator++(int)
	{
		Iterator it = *this;
		operator++();
		return it;
	}

#pragma endregion

#pragma region SList

	template <typename T>
	SList<T>::SList() :
		mSize(0), mFront(nullptr), mBack(nullptr)
	{
	}

	template <typename T>
	SList<T>::SList(const SList<T>& rhs) :
		SList()
	{
		operator=(rhs);
	}

	template <typename T>
	SList<T>::SList(SList<T>&& rhs) :
		SList()
	{
		operator=(rhs);
	}

	template <typename T>
	SList<T> & SList<T>::operator=(SList<T>&& rhs)
	{
		if (this != &rhs)
		{
			Clear();

			mSize = rhs.mSize;
			mFront = rhs.mFront;
			mBack = rhs.mBack;

			rhs.mSize = 0;
			rhs.mFront = nullptr;
			rhs.mBack = nullptr;
		}

		return *this;
	}

	template <typename T>
	SList<T> & SList<T>::operator=(const SList<T>& rhs)
	{
		if (this != &rhs)
		{
			Clear();

			if (!rhs.IsEmpty())
			{
				for (Iterator it = rhs.begin(); it != rhs.end(); ++it)
				{
					PushBack(*it);
				}
			}
		}

		return *this;
	}

	template <typename T>
	typename SList<T>::Iterator SList<T>::PushFront(const T& item)
	{
		Node* nodeToPush = new Node(item, nullptr);
		nodeToPush->mNext = nullptr;

		if (!IsEmpty())
		{
			nodeToPush->mNext = mFront;
		}
		else
		{
			mBack = nodeToPush;
		}
		mFront = nodeToPush;

		mSize++;

		return Iterator(mFront, this);
	}

	template <typename T>
	void SList<T>::PopFront()
	{
		if (IsEmpty())
		{
			return;
		}

		Node* newFront = mFront->mNext;
		delete mFront;

		if (mSize == 1U)
		{
			mFront = nullptr;
			mBack = nullptr;
		}
		else
		{
			mFront = newFront;
		}

		mSize--;
	}

	template <typename T>
	typename SList<T>::Iterator SList<T>::PushBack(const T& item)
	{
		Node* nodeToPush = new Node(item);
		nodeToPush->mNext = nullptr;

		if (!IsEmpty())
		{
			mBack->mNext = nodeToPush;
		}
		else
		{
			mFront = nodeToPush;
		}
		mBack = nodeToPush;

		mSize++;

		return Iterator(mBack, this);
	}

	template <typename T>
	SList<T>::~SList()
	{
		Clear();
	}

	template <typename T>
	T & SList<T>::Front()
	{
		if (mFront == nullptr)
			throw std::exception("Front of the list is null");

		return mFront->mItem;
	}

	template <typename T>
	const T & SList<T>::Front() const
	{
		if (mFront == nullptr)
			throw std::exception("Front of the list is null");

		return mFront->mItem;
	}

	template <typename T>
	T & SList<T>::Back()
	{
		if (mBack == nullptr)
			throw std::exception("Back of the list is null");

		return mBack->mItem;
	}

	template <typename T>
	std::uint32_t SList<T>::Size() const
	{
		return mSize;
	}

	template <typename T>
	bool SList<T>::IsEmpty() const
	{
		return mSize == 0U;
	}

	template <typename T>
	void SList<T>::Clear()
	{
		while (!IsEmpty())
		{
			PopFront();
		}
	}

	template <typename T>
	typename SList<T>::Iterator SList<T>::begin() const
	{
		if (IsEmpty())
			return Iterator(nullptr, this);

		return Iterator(mFront, this);
	}

	template <typename T>
	typename SList<T>::Iterator SList<T>::end() const
	{
		return Iterator(nullptr, this);
	}

	template <typename T>
	void SList<T>::InsertAfter(const T& item, const Iterator& it)
	{
		if (it.mList != this)
			throw std::exception("Invalid iterator");

		if (it == end() || it.mCurrentNode == mBack)
		{
			PushBack(item);
		}
		else
		{
			Node* nextNode = it.mCurrentNode->mNext;
			it.mCurrentNode->mNext = new Node(item, nextNode);
		}
	}

	template <typename T>
	typename SList<T>::Iterator SList<T>::Find(const T& value) const
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
	void SList<T>::Remove(const T& item)
	{
		Iterator previous = begin();
		Iterator current = begin();

		for (; current != end(); ++current)
		{
			if (*current == item)
			{
				if (current == begin())
				{
					PopFront();
					return;
				}

				Node* nextNode = current.mCurrentNode->mNext;
				if (current.mCurrentNode == mBack)
				{
					mBack = previous.mCurrentNode;
				}
				delete current.mCurrentNode;
				previous.mCurrentNode->mNext = nextNode;
				mSize--;
				return;
			}
			previous = current;
		}
	}

#pragma endregion

}