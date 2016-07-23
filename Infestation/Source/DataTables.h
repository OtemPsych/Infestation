#ifndef DataTables_H_
#define DataTables_H_

#include <vector>
#include <memory>

#include <SFML/Graphics/Rect.hpp>

#include <PYRO/Utils.h>
#include <PYRO/ResourceHolder.h>

namespace data
{
	using TextureData = std::vector<std::pair<sf::FloatRect, sf::Vector2f>>;

	struct EntityData
	{
		unsigned short     entity_type;
		const sf::Texture* texture;
		sf::Vector2f       size;
		unsigned short     speed;
	};

	struct BloodSprayData : public EntityData
	{
		unsigned short               blood_spray_type;
		std::string                  name;
		std::unique_ptr<TextureData> animation_rects;
	};

	struct BloodSplatterData : public EntityData
	{
		unsigned short               blood_splatter_type;
		std::string                  name;
		std::unique_ptr<TextureData> animation_rects;
	};

	struct CharacterData : public EntityData
	{
		std::string    name;
		unsigned short health;
		unsigned short damage;
	};

	struct SurvivorData : public CharacterData
	{
		unsigned short survivor_type;
		sf::Time       shoot_cooldown;
	};

	struct InfectedData : public CharacterData
	{
		unsigned short infected_type;
		unsigned short min_speed;
	};

	using SurvivorDataHolder = std::vector<std::unique_ptr<SurvivorData>>;
	template <typename ID>
	SurvivorDataHolder initializeSurvivorData(const pyro::TextureHolder<ID>& survivor_textures);

	using ProjectileDataHolder = std::vector<std::unique_ptr<EntityData>>;
	template <typename ID>
	ProjectileDataHolder initializeProjectileData(const pyro::TextureHolder<ID>& projectile_textures);

	using InfectedDataHolder = std::vector<std::unique_ptr<InfectedData>>;
	template <typename ID>
	InfectedDataHolder initializeInfectedData(const pyro::TextureHolder<ID>& infected_textures);

	using BloodSprayDataHolder = std::vector<std::unique_ptr<BloodSprayData>>;
	template <typename ID>
	BloodSprayDataHolder initializeBloodSprayData(const pyro::TextureHolder<ID>& blood_textures);

	using BloodSplatterDataHolder = std::vector<std::unique_ptr<BloodSplatterData>>;
	template <typename ID>
	BloodSplatterDataHolder initializeBloodSplatterData(const pyro::TextureHolder<ID>& blood_textures);
}
#include "DataTables.inl"
#endif