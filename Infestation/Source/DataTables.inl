#include "Tools/JsonParser.h"

namespace data
{
	template <typename ID>
	SurvivorDataHolder initializeSurvivorData(const pyro::TextureHolder<ID>& survivor_textures)
	{
		SurvivorDataHolder data;
		auto survivor_data(std::make_unique<SurvivorData>());
		data.push_back(std::move(survivor_data));
		data[Survivor::Survivor1]->entity_type    = static_cast<unsigned short>(Entity::EntityType::Survivor);
		data[Survivor::Survivor1]->texture        = &survivor_textures.get(Survivor::Survivor1);
		data[Survivor::Survivor1]->size           = sf::Vector2f();
		data[Survivor::Survivor1]->speed          = 250;
		data[Survivor::Survivor1]->name           = "Survivor";
		data[Survivor::Survivor1]->health         = 100;
		data[Survivor::Survivor1]->damage         = 25;
		data[Survivor::Survivor1]->survivor_type  = Survivor::Survivor1;
		data[Survivor::Survivor1]->shoot_cooldown = sf::seconds(0.15f);

		return std::move(data);
	}

	template <typename ID>
	ProjectileDataHolder initializeProjectileData(const pyro::TextureHolder<ID>& projectile_textures)
	{
		ProjectileDataHolder data;
		auto projectile_data(std::make_unique<EntityData>());
		data.push_back(std::move(projectile_data));
		data[Projectile::PistolRound]->entity_type = static_cast<unsigned short>(Entity::EntityType::Projectile);
		data[Projectile::PistolRound]->texture     = &projectile_textures.get(Projectile::PistolRound);
		data[Projectile::PistolRound]->size        = sf::Vector2f(20.f, 8.8f);
		data[Projectile::PistolRound]->speed       = 1500;

		return std::move(data);
	}

	template <typename ID>
	InfectedDataHolder initializeInfectedData(const pyro::TextureHolder<ID>& infected_textures)
	{
		InfectedDataHolder data;
		auto infected_data(std::make_unique<InfectedData>());
		data.push_back(std::move(infected_data));
		data[Infected::Zombie]->entity_type   = static_cast<unsigned short>(Entity::EntityType::Infected);
		data[Infected::Zombie]->texture       = &infected_textures.get(Infected::Zombie);
		data[Infected::Zombie]->size          = sf::Vector2f();
		data[Infected::Zombie]->min_speed     = 80;
		data[Infected::Zombie]->speed         = 235;
		data[Infected::Zombie]->name          = "Zombie";
		data[Infected::Zombie]->health        = 75;
		data[Infected::Zombie]->damage        = 35;
		data[Infected::Zombie]->infected_type = Infected::Zombie;

		return std::move(data);
	}

	template <typename ID>
	BloodSprayDataHolder initializeBloodSprayData(const pyro::TextureHolder<ID>& blood_textures)
	{
		BloodSprayDataHolder data;
		auto blood_spray1(std::make_unique<BloodSprayData>());
		blood_spray1->blood_spray_type = Infected::BloodSprayType::BloodSpray1;
		blood_spray1->entity_type      = blood_spray1->blood_spray_type;
		blood_spray1->texture          = &blood_textures.get(static_cast<ID>(blood_spray1->blood_spray_type));
		blood_spray1->size             = sf::Vector2f();
		blood_spray1->speed            = 0;
		blood_spray1->name             = "Blood_Spray_1";
		blood_spray1->animation_rects  = std::make_unique<TextureData>(std::move(parseJsonFile(blood_spray1->name, "")));
		data.push_back(std::move(blood_spray1));

		auto blood_spray2(std::make_unique<BloodSprayData>());
		blood_spray2->blood_spray_type = Infected::BloodSprayType::BloodSpray2;
		blood_spray2->entity_type      = blood_spray2->blood_spray_type;
		blood_spray2->texture          = &blood_textures.get(static_cast<ID>(blood_spray2->blood_spray_type));
		blood_spray2->size             = sf::Vector2f();
		blood_spray2->speed            = 0;
		blood_spray2->name             = "Blood_Spray_2";
		blood_spray2->animation_rects  = std::make_unique<TextureData>(std::move(parseJsonFile(blood_spray2->name, "")));
		data.push_back(std::move(blood_spray2));

		auto blood_spray3(std::make_unique<BloodSprayData>());
		blood_spray3->blood_spray_type = Infected::BloodSprayType::BloodSpray3;
		blood_spray3->entity_type      = blood_spray3->blood_spray_type;
		blood_spray3->texture          = &blood_textures.get(static_cast<ID>(blood_spray3->blood_spray_type));
		blood_spray3->size             = sf::Vector2f();
		blood_spray3->speed            = 0;
		blood_spray3->name             = "Blood_Spray_3";
		blood_spray3->animation_rects  = std::make_unique<TextureData>(std::move(parseJsonFile(blood_spray3->name, "")));
		data.push_back(std::move(blood_spray3));

		return std::move(data);
	}

	template <typename ID>
	BloodSplatterDataHolder initializeBloodSplatterData(const pyro::TextureHolder<ID>& blood_textures)
	{
		BloodSplatterDataHolder data;
		auto blood_splatter1(std::make_unique<BloodSplatterData>());
		blood_splatter1->blood_splatter_type = Infected::BloodSplatterType::BloodSplatter1;
		blood_splatter1->entity_type         = blood_splatter1->blood_splatter_type;
		blood_splatter1->texture             = &blood_textures.get(static_cast<ID>(blood_splatter1->blood_splatter_type));
		blood_splatter1->size                = sf::Vector2f();
		blood_splatter1->speed               = 0;
		blood_splatter1->name                = "Blood_Splatter_1";
		blood_splatter1->animation_rects     = std::make_unique<TextureData>(std::move(parseJsonFile(blood_splatter1->name, "")));
		data.push_back(std::move(blood_splatter1));

		auto blood_splatter2(std::make_unique<BloodSplatterData>());
		blood_splatter2->blood_splatter_type = Infected::BloodSplatterType::BloodSplatter2;
		blood_splatter2->entity_type         = blood_splatter2->blood_splatter_type;
		blood_splatter2->texture             = &blood_textures.get(static_cast<ID>(blood_splatter2->blood_splatter_type));
		blood_splatter2->size                = sf::Vector2f();
		blood_splatter2->speed               = 0;
		blood_splatter2->name                = "Blood_Splatter_2";
		blood_splatter2->animation_rects     = nullptr;
		data.push_back(std::move(blood_splatter2));

		auto blood_splatter3(std::make_unique<BloodSplatterData>());
		blood_splatter3->blood_splatter_type = Infected::BloodSplatterType::BloodSplatter3;
		blood_splatter3->entity_type         = blood_splatter3->blood_splatter_type;
		blood_splatter3->texture             = &blood_textures.get(static_cast<ID>(blood_splatter3->blood_splatter_type));
		blood_splatter3->size                = sf::Vector2f();
		blood_splatter3->speed               = 0;
		blood_splatter3->name                = "Blood_Splatter_3";
		blood_splatter3->animation_rects     = nullptr;
		data.push_back(std::move(blood_splatter3));

		return std::move(data);
	}
}