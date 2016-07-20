#include <PYRO/Math.h>

#include "Player.h"

Player::Movement::Movement()
	: up(false)
	, down(false)
	, left(false)
	, right(false)
{
}

Player::Player(const sf::FloatRect& world_bounds, const sf::Texture& muzzle_flash_texture,
	           data::SurvivorData* survivor_data, sf::RenderWindow* window)
	: Survivor(muzzle_flash_texture, survivor_data)
	, world_bounds_(world_bounds)
	, window_(window)
{
}

void Player::handleWorldBoundsCollision()
{
	sf::FloatRect gbounds(getGlobalBounds());

	if (gbounds.left <= world_bounds_.left)
		movement_.left = false;
	else if (gbounds.left + gbounds.width >= world_bounds_.left + world_bounds_.width)
		movement_.right = false;

	if (gbounds.top <= world_bounds_.top)
		movement_.up = false;
	else if (gbounds.top + gbounds.height >= world_bounds_.top + world_bounds_.height)
		movement_.down = false;
}

void Player::handleRotation()
{
	if (window_->hasFocus()) {
		const sf::Vector2f mouse_pos(window_->mapPixelToCoords(sf::Mouse::getPosition(*window_)));
		const sf::Vector2f distance_vec(mouse_pos - getPosition());

		setRotation(pyro::math::toDegrees(atan2f(distance_vec.y, distance_vec.x)));
	}
}

void Player::handleMovement(const sf::Event& event)
{
	bool state;
	if (event.type == sf::Event::KeyPressed) {
		state = true;
	} else if (event.type == sf::Event::KeyReleased) {
		state = false;
	} else {
		return;
	}

	switch (event.key.code) 
	{
	case sf::Keyboard::W:
		movement_.up = state;
		break;
	case sf::Keyboard::S:
		movement_.down = state;
		break;
	case sf::Keyboard::A:
		movement_.left = state;
		break;
	case sf::Keyboard::D:
		movement_.right = state;
	}
}

void Player::updateCurrent(sf::Time dt)
{
	Survivor::updateCurrent(dt);

	handleWorldBoundsCollision();
	handleRotation();

	sf::Vector2i movement(0, 0);

	if (movement_.up)
		movement.y -= survivor_data_->speed;
	if (movement_.down)
		movement.y += survivor_data_->speed;

	if (movement_.left)
		movement.x -= survivor_data_->speed;
	if (movement_.right)
		movement.x += survivor_data_->speed;

	move(movement.x * dt.asSeconds(), movement.y * dt.asSeconds());
}

void Player::handleEventCurrent(const sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
		fireProjectile();
	else
		handleMovement(event);
}