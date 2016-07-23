#include "Animation.h"
#include "Entity.h"

Animation::Animation(const data::TextureData& texture_data, const sf::Time& duration,
	                 pyro::SpriteNode* sprite_node, bool repeat)
	: texture_data_(texture_data)
	, repeat_(repeat)
	, current_rect_(0)
	, time_per_rect_(duration / static_cast<float>(texture_data.size()))
	, elapsed_time_(sf::Time::Zero)
	, animation_ongoing_(true)
	, sprite_node_(sprite_node)
{
}

void Animation::restart()
{
	current_rect_ = 0;
	animation_ongoing_ = true;
}

void Animation::updateCurrent(sf::Time dt)
{
	if (animation_ongoing_ && (elapsed_time_ += dt) >= time_per_rect_) {
		if (current_rect_ < texture_data_.size() - 1 || repeat_) {
			sprite_node_->setTextureRect(texture_data_[current_rect_].first);

			sf::FloatRect lbounds(sprite_node_->getLocalBounds());
			sf::Vector2f origin(texture_data_[current_rect_].second);
			sprite_node_->setOrigin(origin.x * lbounds.width, origin.y * lbounds.height);

			elapsed_time_ = sf::Time::Zero;

			if (repeat_) {
				if (current_rect_ < texture_data_.size() - 1) {
					current_rect_++;
				} else {
					current_rect_ = 0;
				}
			} else {
				current_rect_++;
			}
		}
		else {
			animation_ongoing_ = false;
		}
	}
}

bool Animation::isDestroyed() const
{
	return !animation_ongoing_;
}