#pragma once
namespace Library
{
	class World;
	class Sector : public Attributed
	{
		RTTI_DECLARATIONS(Sector, Attributed)
	public:
		Sector();
		Sector(const Sector& rhs) = default;

		Sector& operator=(const Sector& rhs) = default;

		virtual ~Sector() = default;

		void InitializeAttributes();
		virtual Vector<std::string>& PrescribedAttributes() const;

		const std::string& Name() const;
		void SetName(const std::string& name);

		Datum& Entities() const;
		Entity* CreateEntity(std::string className, std::string instanceName);

		World* GetWorld() const;
		void SetWorld(World* world);

		void Update(WorldState& worldState);

	private:
		static Vector<std::string> sPerscribedAttributes;
		std::string mName;
		Scope mEntities;
	};
}


