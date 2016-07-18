#ifndef Entity_H_
#define Entity_H_

#include <SFML/Graphics/VertexArray.hpp>

#include <PYRO/SceneNode.h>

#include "DataTables.h"

class Entity : public pyro::SceneNode
{
public:
	enum EntityType { Survivor, Infected, Projectile, EntityTypeCount };

public:
	Entity(const sf::Texture* texture, data::EntityData* entity_data);
	virtual ~Entity();
public:
	void setSize(const sf::Vector2f& size);
	void setTextureRect(const sf::FloatRect& texture_rect);

	virtual sf::FloatRect getLocalBounds() const override;
protected:
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
	void initVertices();

private:			   
	data::EntityData*  entity_data_;
	sf::VertexArray    vertices_;
	const sf::Texture* texture_;
};
#endif