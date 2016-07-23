#ifndef Entity_H_
#define Entity_H_

#include <SFML/Graphics/VertexArray.hpp>

#include <PYRO/SceneNode.h>

#include "DataTables.h"

class Entity : public pyro::SceneNode
{
public:
	enum class EntityType { Survivor, Infected, Projectile, EntityTypeCount };

public:
	explicit Entity(data::EntityData* entity_data);
	virtual ~Entity();
public:
	bool checkCollision(const Entity& entity);

	void setSize(const sf::Vector2f& size);
	void setTextureRect(const sf::FloatRect& texture_rect);

	virtual sf::FloatRect getCollisionBounds() const;
	virtual sf::FloatRect getLocalBounds() const override;
protected:
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
	void initVertices();

protected:			   
	data::EntityData*       entity_data_;
	bool                    collided_;
	sf::VertexArray         vertices_;
};
#endif