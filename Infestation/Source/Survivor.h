#ifndef Survivor_H_
#define Survivor_H_

#include <PYRO/SpriteNode.h>

#include "Character.h"

class Survivor : public Character
{
public:
	enum SurvivorType { Survivor1, SurvivorTypeCount };

public:
	Survivor(const sf::Texture& muzzle_flash_texture, data::SurvivorData* survivor_data);
	virtual ~Survivor();
public:
	bool wasGunFired();
protected:
	void fireProjectile();
	virtual void updateCurrent(sf::Time dt) override;

protected:
	data::SurvivorData*             survivor_data_;
private:				            
	pyro::SpriteNode*               muzzle_flash_;
	const sf::Texture&              muzzle_flash_texture_;
	pyro::utils::Resource<sf::Time> muzzle_flash_lifetime_;
	bool                            gun_fired_;
};
#endif