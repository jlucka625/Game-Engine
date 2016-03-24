namespace Library
{
	template <typename T>
	Stack<T>::Stack() :
		mList(SList<T>())
	{}

	template <typename T>
	Stack<T>::Stack(const Stack<T>& rhs) :
		Stack()
	{
		operator=(rhs);
	}

	template <typename T>
	Stack<T>& Stack<T>::operator=(const Stack<T>& rhs)
	{
		if (this != &rhs)
		{
			mList = rhs.mList;
		}
		return *this;
	}

	template <typename T>
	void Stack<T>::Push(const T& item)
	{
		mList.PushFront(item);
	}

	template <typename T>
	void Stack<T>::Pop()
	{
		mList.PopFront();
	}

	template <typename T>
	T& Stack<T>::Top()
	{
		return mList.Front();
	}

	template <typename T>
	const T& Stack<T>::Top() const
	{
		return mList.Front();
	}

	template <typename T>
	std::uint32_t Stack<T>::Size() const
	{
		return mList.Size();
	}

	template <typename T>
	bool Stack<T>::IsEmpty() const
	{
		return mList.IsEmpty();
	}

	template <typename T>
	void Stack<T>::Clear()
	{
		mList.Clear();
	}
}