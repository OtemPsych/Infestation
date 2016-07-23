#include <PYRO/Math.h>

#include "Projectile.h"

Projectile::Projectile(const sf::Vector2u& window_size, const sf::Vector2f& shooter_pos,
	                   float shooter_rotation, data::EntityData* entity_data)
	: Entity(entity_data)
	, origin_position_(shooter_pos)
	, max_travel_distance_(pyro::math::getHypotenuse(static_cast<sf::Vector2f>(window_size)))
{
	using namespace pyro::utils;
	setOriginFlags(OriginFlag::Center);

	setPosition(shooter_pos);
	setRotation(shooter_rotation);
}

bool Projectile::isDestroyed() const
{
	return pyro::math::getHypotenuse(origin_position_ - getWorldPosition()) > max_travel_distance_ 
		   || collided_;
}

void Projectile::updateCurrent(sf::Time dt)
{
	float angle_radians = pyro::math::toRadians(getRotation());
	move(cos(angle_radians) * entity_data_->speed * dt.asSeconds(),
		 sin(angle_radians) * entity_data_->speed * dt.asSeconds());
}