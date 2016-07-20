#ifndef DataTables_H_
#define DataTables_H_

#include <vector>

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

	struct CharacterData : public EntityData
	{
		std::string    name;
		unsigned short health;
		unsigned short damage;
	};

	struct SurvivorData : public CharacterData
	{
		unsigned short survivor_type;
	};

	struct InfectedData : public CharacterData
	{
		unsigned short infected_type;
	};

	using SurvivorDataHolder = std::vector<std::unique_ptr<SurvivorData>>;
	template <typename ID>
	SurvivorDataHolder initializeSurvivorData(const pyro::TextureHolder<ID>& survivor_textures);

	using ProjectileDataHolder = std::vector<std::unique_ptr<EntityData>>;
	template <typename ID>
	ProjectileDataHolder initializeProjectileData(const pyro::TextureHolder<ID>& projectile_textures);
}
#include "DataTables.inl"
#endif