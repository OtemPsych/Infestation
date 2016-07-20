#include "Character.h"

Character::Character(data::CharacterData* character_data)
	: Entity(character_data)
	, character_data_(character_data)
	, health_(character_data->health)
{
	setOriginFlags(pyro::utils::OriginFlag::Center);
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