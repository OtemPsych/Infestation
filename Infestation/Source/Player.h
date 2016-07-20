#ifndef Player_H_
#define Player_H_

#include <SFML/Graphics/RenderWindow.hpp>

#include "Survivor.h"

class Player : public Survivor
{
private:
	struct Movement {
		Movement();

		bool up, down, left, right;
	};

public:
	Player(const sf::FloatRect& world_bounds, const sf::Texture& muzzle_flash_texture,
		   data::SurvivorData* survivor_data, sf::RenderWindow* window);
private:
	void handleWorldBoundsCollision();
	void handleRotation();
	void handleMovement(const sf::Event& event);

	virtual void updateCurrent(sf::Time dt) override;
	virtual void handleEventCurrent(const sf::Event& event) override;

private:
	const sf::FloatRect& world_bounds_;
	sf::RenderWindow*    window_;

	Movement             movement_;
};
#endif