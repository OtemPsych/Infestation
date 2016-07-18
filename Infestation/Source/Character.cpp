#include "Character.h"

Character::Character(const sf::Texture* texture, data::CharacterData* character_data)
	: Entity(texture, character_data)
	, character_data_(character_data)
	, health_(character_data->health)
{
	pyro::utils::setOriginFlags(pyro::utils::OriginFlag::Center, this);
}

Character::~Character()
{
}

void Character::receiveDamage(unsigned short damage)
{
	if (damage >= health_) {
		health_ = 0;
	} else {
		health_ -= damage;
	}
}

bool Character::isDestroyed() const
{
	return health_ == 0;
}