#ifndef Infected_H_
#define Infected_H_

#include "Survivor.h"
#include "Animation.h"

class Infected : public Character
{
public:
	enum InfectedType { Zombie, InfectedTypeCount };
	enum BloodSprayType { BloodSpray1, BloodSpray2, BloodSpray3, BloodSprayCount };
	enum BloodSplatterType { BloodSplatter1, BloodSplatter2, BloodSplatter3, BloodSplatterCount };

public:
	Infected(const sf::FloatRect& world_bounds, std::vector<Survivor*>* survivors,
		     data::InfectedData* infected_data, data::BloodSprayDataHolder* blood_spray_data_holder);
public:
	sf::FloatRect getCollisionBounds() const override;
private:
	void randomizeStartingPosition(const sf::FloatRect& world_bounds);
	virtual void updateCurrent(sf::Time dt) override;
	virtual void receiveDamage(unsigned short damage);

private:
	data::InfectedData*            infected_data_;
	std::vector<Survivor*>*        survivors_;
	const float                    speed_;

	data::BloodSprayDataHolder*    blood_spray_data_holder_;
	std::vector<pyro::SpriteNode*> blood_spray_sprites_;
	std::vector<Animation*>        blood_spray_animations_;
};
#endif