#pragma once
namespace Library 
{
	class Sector;
	class Entity : public Attributed
	{
		RTTI_DECLARATIONS(Entity, Attributed)
	public:
		Entity();
		Entity(const Entity& rhs) = default;

		Entity& operator=(const Entity& rhs) = default;

		virtual ~Entity() = default;

		void InitializeAttributes();
		virtual Vector<std::string>& PrescribedAttributes() const;

		const std::string& Name() const;
		void SetName(const std::string& name);

		Sector* GetSector();
		void SetSector(Sector* sector);

		void Update(WorldState& worldState);

	private:
		static Vector<std::string> sPerscribedAttributes;
		std::string mName;
	};
}


