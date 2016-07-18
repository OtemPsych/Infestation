#include "Survivor.h"

Survivor::Survivor(const sf::Texture* texture, data::SurvivorData* survivor_data)
	: Character(texture, survivor_data)
	, survivor_data_(survivor_data)
{
}

Survivor::~Survivor()
{
}