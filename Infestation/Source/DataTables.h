#ifndef DataTables_H_
#define DataTables_H_

#include <vector>

#include <SFML/Graphics/Rect.hpp>

#include <PYRO/Utils.h>

namespace data
{
	using TextureData = std::vector<std::pair<sf::FloatRect, sf::Vector2f>>;

	struct EntityData
	{
		unsigned short entity_type;
		sf::Vector2f   size;
		unsigned short speed;
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

	using SurvivorDataHolder = std::vector<std::unique_ptr<SurvivorData>>;

	SurvivorDataHolder initializeSurvivorData();
}
#endif