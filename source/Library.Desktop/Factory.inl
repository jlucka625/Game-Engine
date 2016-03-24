#include "Factory.h"
namespace Library
{
	template<typename AbstractProductT>
	typename Factory<AbstractProductT>::FactoryMap Factory<AbstractProductT>::sFactoryTable;

	template<typename AbstractProductT>
	const Factory<AbstractProductT>* Factory<AbstractProductT>::Find(const std::string & name)
	{
		typename FactoryMap::Iterator it = sFactoryTable.Find(name);
		if (it != sFactoryTable.end())
			return (*it).second;

		return nullptr;
	}

	template<typename AbstractProductT>
	typename Factory<AbstractProductT>::FactoryMap::Iterator Factory<AbstractProductT>::begin()
	{
		return sFactoryTable.begin();
	}

	template<typename AbstractProductT>
	typename Factory<AbstractProductT>::FactoryMap::Iterator Factory<AbstractProductT>::end()
	{
		return sFactoryTable.end();
	}

	template<typename AbstractProductT>
	AbstractProductT * Factory<AbstractProductT>::Create(const std::string & name)
	{
		const Factory<AbstractProductT>* factory = Find(name);
		if (factory == nullptr)
			throw std::exception("Factory for class does not exist");

		return factory->Create();
	}

	template<typename AbstractProductT>
	void Factory<AbstractProductT>::Add(Factory<AbstractProductT>& factory)
	{
		sFactoryTable.Insert(std::make_pair(factory.ClassName(), &factory));
	}

	template<typename AbstractProductT>
	void Factory<AbstractProductT>::Remove(Factory<AbstractProductT>& factory)
	{
		sFactoryTable.Remove(factory.ClassName());
	}
}
