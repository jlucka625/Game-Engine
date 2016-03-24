#pragma once
namespace Library
{
	template <typename AbstractProductT>
	//! Factory Interface Template
	/*! Allows the Game Engine to create objects by their
	type name.*/
	class Factory
	{
		//Interface
	public:
		typedef std::pair<std::string, const Factory* const> PairType;
		typedef HashMap<std::string, const Factory* const> FactoryMap;
		typedef typename FactoryMap::Iterator Iterator;

		//! Get Class Name
		/*! \return A string representing the name of the class
		the factory instantiates.*/
		virtual const std::string ClassName() const = 0;

	private:
		//! Create Concrete Product
		/*! \return The address of a concrete product associated
		with the concrete factory class.*/
		virtual AbstractProductT* Create() const = 0;

		//Singleton Manager
	public:
		//! Find Concrete Factory
		/*! \param name The name of the class this factory instantiates
		\return The associated concrete factory.*/
		static const Factory<AbstractProductT>* Find(const std::string& name);

		//! Begin Iterator
		/*! \return An iterator for the container of factories.*/
		static typename FactoryMap::Iterator begin();

		//! End Iterator
		/*! \return An iterator one past the last factory in
		the container.*/
		static typename FactoryMap::Iterator end();

		//! Create Concrete Product
		/*! You are responsible for deleting the memory
		associated with any objects that you get from the
		Create method.
		\param name The name of the class that you want to
		instantiate from a factory.
		\return The address of a concrete product associated
		with the concrete factory class.*/
		static AbstractProductT* Create(const std::string& name);

	protected:
		//! Add Factory
		/*! This will not Add a factory to the container of factories
		if one for a particular class type is already contained.
		\param factory A reference to a concrete factory to add 
		to the container of factories for this abstract factory.*/
		static void Add(Factory<AbstractProductT>& factory);

		//! Remove Factory
		/*! \param factory A reference to a concrete factory to 
		remove from the container of factories for this abstract
		factory.*/
		static void Remove(Factory<AbstractProductT>& factory);

	private:
		static FactoryMap sFactoryTable;		//!< A table of factories organized by the name of the concrete product class that they create
	};

	//Concrete Factory Macro
#define ConcreteFactory(AbstractProductT, ConcreteProductT)				\
	class ConcreteProductT ## Factory : Factory<AbstractProductT>		\
	{																	\
	public:																\
		ConcreteProductT ## Factory() { Add(*this); }					\
		~ConcreteProductT ## Factory() { Remove(*this); }				\
		virtual const std::string ClassName() const						\
		{																\
			return # ConcreteProductT;									\
		}																\
		virtual AbstractProductT* Create() const						\
		{																\
			AbstractProductT* product = new ConcreteProductT();			\
			assert(product != nullptr);									\
			return product;												\
		}																\
	};
}

#include "Factory.inl"

