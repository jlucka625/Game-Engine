#pragma once
namespace Library
{
	class World : public Attributed
	{
		RTTI_DECLARATIONS(World, Attributed)
	public:
		World();
		World(const World& rhs) = delete;

		World& operator=(const World& rhs) = delete;

		virtual ~World() = default;

		void InitializeAttributes();
		virtual Vector<std::string>& PrescribedAttributes() const;

		const std::string& Name() const;
		void SetName(const std::string& name);

		Datum& Sectors() const;
		Sector* CreateSector(const std::string& instanceName);
		void Update();

	private:
		static Vector<std::string> sPerscribedAttributes;
		std::string mName;
		Scope mSectors;
		WorldState mWorldState;
	};
}

