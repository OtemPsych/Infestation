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

sf::FloatRect Character::getCollisionBounds() const
{
	const sf::FloatRect lbounds(getLocalBounds());
	const sf::Vector2f quarter_size(lbounds.width / 4.f, lbounds.height / 4.f);
	const sf::Vector2f sixth_size(lbounds.width / 6.f, lbounds.height / 6.f);
	
	sf::FloatRect new_lbounds(lbounds.left + lbounds.width / 6.f,
		                      lbounds.top + lbounds.height / 6.f,
		                      lbounds.width / 2.f,
		                      lbounds.height - quarter_size.y - sixth_size.y);
	return getWorldTransform().transformRect(new_lbounds);
}

void Character::dealDamage(Character* character)
{
	character->receiveDamage(character_data_->damage);
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