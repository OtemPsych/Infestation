#include <PYRO/Math.h>

#include "Infected.h"

Infected::Infected(const sf::FloatRect& world_bounds, std::vector<Survivor*>* survivors,
	               data::InfectedData* infected_data, data::BloodSprayDataHolder* blood_spray_data_holder)
	: Character(infected_data)
	, infected_data_(infected_data)
	, survivors_(survivors)
	, speed_(static_cast<float>(rand() % (infected_data->speed - infected_data->min_speed) + infected_data->min_speed))
	, blood_spray_data_holder_(blood_spray_data_holder)
{
	randomizeStartingPosition(world_bounds);
}

sf::FloatRect Infected::getCollisionBounds() const
{
	const sf::FloatRect lbounds(getLocalBounds());
	const sf::Vector2f quarter_size(lbounds.width / 4.f, lbounds.height / 4.f);
	const sf::Vector2f sixth_size(lbounds.width / 6.f, lbounds.height / 6.f);

	sf::FloatRect new_lbounds(lbounds.left + quarter_size.x,
		                      lbounds.top + quarter_size.y,
						      quarter_size.x,
							  lbounds.height / 2.f);
	return getWorldTransform().transformRect(new_lbounds);
}

void Infected::randomizeStartingPosition(const sf::FloatRect& world_bounds)
{
	sf::FloatRect gbounds(getGlobalBounds());
	enum Pos { Up, Down, Left, Right };

	Pos pos = static_cast<Pos>(rand() % 4);

	switch (pos)
	{
	case Up:
		setPosition(rand() / (RAND_MAX / world_bounds.width), world_bounds.top - gbounds.height);
		break;
	case Down:
		setPosition(rand() / (RAND_MAX / world_bounds.width), world_bounds.height + gbounds.height);
		break;
	case Left:
		setPosition(world_bounds.left - gbounds.width, rand() / (RAND_MAX / world_bounds.height));
		break;
	case Right:
		setPosition(world_bounds.width + gbounds.width, rand() / (RAND_MAX / world_bounds.height));
	}
}

void Infected::updateCurrent(sf::Time dt)
{
	// Remove Dead Animations
	for (size_t i = 0; i < blood_spray_animations_.size(); ++i) {
		if (blood_spray_animations_[i]->isDestroyed()) {
			detachChild(*blood_spray_animations_[i]);
			blood_spray_animations_.erase(blood_spray_animations_.begin() + i);

			detachChild(*blood_spray_sprites_[i]);
			blood_spray_sprites_.erase(blood_spray_sprites_.begin() + i);
		}
	}

	// Movement
	const sf::Vector2f& infected_pos = getPosition();

	sf::Vector2f distance_vec;
	short starting_survivor = -1;
	for (size_t i = 0; i < survivors_->size(); ++i) {
		if (!survivors_->at(i)->isDestroyed()) {
			distance_vec = survivors_->at(i)->getPosition() - infected_pos;
			starting_survivor = i;
		}
	}
	if (starting_survivor == -1)
		return;

	float magnitude = pyro::math::getHypotenuse(distance_vec);
	for (size_t i = starting_survivor; i < survivors_->size(); ++i) {
		if (!survivors_->at(i)->isDestroyed()) {
			const sf::Vector2f new_distance_vec(survivors_->at(i)->getPosition() - infected_pos);
			const float new_magnitude = pyro::math::getHypotenuse(new_distance_vec);

			if (std::abs(new_magnitude) < std::abs(magnitude)) {
				distance_vec = new_distance_vec;
				magnitude = new_magnitude;
			}
		}
	}

	distance_vec = pyro::math::normalizeVector(distance_vec);
	setRotation(pyro::math::toDegrees(std::atan2f(distance_vec.y, distance_vec.x)));

	move(distance_vec * speed_ * dt.asSeconds());
}

void Infected::receiveDamage(unsigned short damage)
{
	Character::receiveDamage(damage);

	const data::BloodSprayData* blood_spray_data = blood_spray_data_holder_->at(rand() % BloodSprayCount).get();
	auto blood_spray_sprite(std::make_unique<pyro::SpriteNode>(*blood_spray_data->texture,
		                                                       static_cast<sf::IntRect>(blood_spray_data->animation_rects->front().first)));
	blood_spray_sprite->setOriginFlags(pyro::utils::OriginFlag::Center);
	pyro::utils::setRelativeAlignment(*this, pyro::utils::OriginFlag::Center, 0.f, blood_spray_sprite.get());
	blood_spray_sprite->scale(-2.8f, 2.8f);

	auto blood_spray(std::make_unique<Animation>(*blood_spray_data->animation_rects.get(), sf::seconds(0.47f),
		                                         blood_spray_sprite.get()));

	blood_spray_sprites_.push_back(blood_spray_sprite.get());
	blood_spray_animations_.push_back(blood_spray.get());
	attachChild(std::move(blood_spray_sprite));
	attachChild(std::move(blood_spray));
}