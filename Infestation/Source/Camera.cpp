#include "Camera.h"

Camera::Camera(const sf::FloatRect& world_bounds, const std::vector<Survivor*>& survivors,
	           sf::RenderWindow* window)
	: world_bounds_(world_bounds)
	, survivors_(survivors)
	, window_(window)
	, view_(window_->getDefaultView())
	, spectated_survivor_index_(0)
{
}

void Camera::updateCurrent(sf::Time dt)
{
	// Change Survivor Being Spectated if Needed
	if (survivors_[spectated_survivor_index_]->isDestroyed()) {
		for (size_t i = 1; i < survivors_.size(); ++i) {
			if (!survivors_[i]->isDestroyed()) {
				spectated_survivor_index_ = i;
			}
		}
	}

	// Update Camera Position
	const sf::Vector2f view_center(survivors_[spectated_survivor_index_]->getPosition());
	const sf::Vector2f half_view(view_.getSize() / 2.f);
	const sf::FloatRect visible_area(view_center - half_view, view_center + half_view);

	if (visible_area.left >= world_bounds_.left && visible_area.width <= world_bounds_.width)
		view_.setCenter(view_center.x, view_.getCenter().y);

	if (visible_area.top >= world_bounds_.top && visible_area.height <= world_bounds_.height)
		view_.setCenter(view_.getCenter().x, view_center.y);

	window_->setView(view_);
}

void Camera::handleEventCurrent(const sf::Event& event)
{
	if (survivors_.front()->isDestroyed() && event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::Left && spectated_survivor_index_ - 1 >= 1) {
			spectated_survivor_index_ -= 1;
		}
		else if (event.key.code == sf::Keyboard::Right && 
			     spectated_survivor_index_ + 1 < survivors_.size()) {
			spectated_survivor_index_ += 1;
		}
	}
}