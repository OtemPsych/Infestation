#ifndef Projectile_H_
#define Projectile_H_

#include "Entity.h"

class Projectile : public Entity
{
public:
	enum ProjectileType { PistolRound, ProjectileTypeCount };

public:
	Projectile(const sf::Vector2u& window_size_, const sf::Vector2f& shooter_pos,
		       float shooter_rotation, data::EntityData* entity_data);
public:
	virtual bool isDestroyed() const override;
private:
	virtual void updateCurrent(sf::Time dt) override;

private:
	sf::Vector2f origin_position_;
	float        max_travel_distance_;
};
#endif