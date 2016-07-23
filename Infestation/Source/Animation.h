#ifndef Animation_H_
#define Animation_H_

#include <PYRO/SpriteNode.h>

#include "DataTables.h"

class Animation : public pyro::SceneNode, private sf::NonCopyable
{
public:
	Animation(const data::TextureData& texture_data, const sf::Time& duration,
		      pyro::SpriteNode* sprite_node, bool repeat = false);
public:
	void restart();
	virtual void updateCurrent(sf::Time dt) override;
	virtual bool isDestroyed() const override;

	inline bool isAnimationOngoing() const { return animation_ongoing_; }

private:
	const data::TextureData& texture_data_;
	const bool               repeat_;
	unsigned short           current_rect_;
	sf::Time                 time_per_rect_;
	sf::Time                 elapsed_time_;
	bool                     animation_ongoing_;
	pyro::SpriteNode*        sprite_node_;
};
#endif