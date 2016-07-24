#ifndef Camera_H_
#define Camera_H_

#include <SFML/Graphics/RenderWindow.hpp>

#include <PYRO/Text.h>

#include "Survivor.h"

class Camera : public pyro::SceneNode, private sf::NonCopyable
{
public:
	Camera(const sf::FloatRect& world_bounds, const std::vector<Survivor*>& survivors,
		   sf::RenderWindow* window);
private:
	virtual void updateCurrent(sf::Time dt) override;
	virtual void handleEventCurrent(const sf::Event& event) override;

private:
	const sf::FloatRect&          world_bounds_;
	const std::vector<Survivor*>& survivors_;
	sf::RenderWindow*             window_;

	sf::View                      view_;
	const sf::Vector2f            half_view_size_;
	unsigned short                spectated_survivor_index_;
};
#endif