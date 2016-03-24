#include "pch.h"

RTTI_DEFINITIONS(RFoo)

RFoo::RFoo() :
	mData(0), mMoreData(new int)
{

}

RFoo::RFoo(int data) :
	mData(data), mMoreData(new int)
{

}

RFoo::RFoo(const RFoo& rhs) :
	mData(rhs.mData), mMoreData(new int(*rhs.mMoreData))
{

}

RFoo& RFoo::operator=(const RFoo& rhs)
{
	if (this != &rhs)
	{
		delete mMoreData;

		mData = rhs.mData;
		mMoreData = new int(*rhs.mMoreData);
	}

	return *this;
}

bool RFoo::operator==(const RFoo& rhs) const
{
	return (mData == rhs.mData) && (*mMoreData == *rhs.mMoreData);
}

RFoo::~RFoo()
{
	delete mMoreData;
}
int & RFoo::Data()
{
	return mData;
}

int* RFoo::MoreData()
{
	return mMoreData;
}

