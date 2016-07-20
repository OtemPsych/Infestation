#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "Entity.h"

Entity::Entity(data::EntityData* entity_data)
	: entity_data_(entity_data)
	, vertices_(sf::Quads)
{
	initVertices();
}

Entity::~Entity()
{
}

void Entity::setSize(const sf::Vector2f& size)
{
	vertices_[1].position.x = size.x;
	vertices_[2].position   = size;
	vertices_[3].position.y = size.y;
}

void Entity::setTextureRect(const sf::FloatRect& texture_rect)
{
	const float texture_rect_right  = texture_rect.left + texture_rect.width;
	const float texture_rect_bottom = texture_rect.top + texture_rect.height;

	vertices_[0].texCoords = sf::Vector2f(texture_rect.left, texture_rect.top);
	vertices_[1].texCoords = sf::Vector2f(texture_rect_right, texture_rect.top);
	vertices_[2].texCoords = sf::Vector2f(texture_rect_right, texture_rect_bottom);
	vertices_[3].texCoords = sf::Vector2f(texture_rect.left, texture_rect_bottom);
}

sf::FloatRect Entity::getLocalBounds() const
{
	return sf::FloatRect(sf::Vector2f(), vertices_[2].position);
}

void Entity::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.texture = entity_data_->texture;
	target.draw(vertices_, states);
}

void Entity::initVertices()
{
	if (entity_data_->size == sf::Vector2f() && entity_data_->texture)
		entity_data_->size = static_cast<sf::Vector2f>(entity_data_->texture->getSize());

	vertices_.append(sf::Vertex(sf::Vector2f(0.f,                  0.f)));
	vertices_.append(sf::Vertex(sf::Vector2f(entity_data_->size.x, 0.f)));
	vertices_.append(sf::Vertex(sf::Vector2f(entity_data_->size.x, entity_data_->size.y)));
	vertices_.append(sf::Vertex(sf::Vector2f(0.f,                  entity_data_->size.y)));

	if (entity_data_->texture) {
		const sf::Vector2f texture_size(entity_data_->texture->getSize());
		vertices_[1].texCoords = sf::Vector2f(texture_size.x, 0.f);
		vertices_[2].texCoords = sf::Vector2f(texture_size.x, texture_size.y);
		vertices_[3].texCoords = sf::Vector2f(0.f,            texture_size.y);
	}
}