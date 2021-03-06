#ifndef Character_H_
#define Character_H_

#include "Entity.h"

class Character : public Entity
{
public:
	explicit Character(data::CharacterData* character_data);
	virtual ~Character();
public:
	sf::FloatRect getCollisionBounds() const override;

	void dealDamage(Character* character);
	virtual void receiveDamage(unsigned short damage);
	virtual bool isDestroyed() const override;

private:
	data::CharacterData* character_data_;
	unsigned short       health_;
};
#endif