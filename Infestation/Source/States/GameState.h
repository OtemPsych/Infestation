#ifndef GameState_H_
#define GameState_H_

#include <SFML/Graphics/Texture.hpp>

#include <PYRO/State.h>
#include <PYRO/ResourceHolder.h>

#include "../Player.h"

class GameState : public pyro::State
{
private:
	enum SceneLayers { Background, Entities, SceneLayerCount };

public:
	GameState(pyro::StateStack* stack, sf::RenderWindow* window);
public:
	virtual bool handleEvent(const sf::Event& event) override;
	virtual bool update(sf::Time dt) override;
	virtual void draw() override;
private:
	void buildScene();
	void initBackground();
	virtual void setupResources() override;

private:
	pyro::SceneNode                             scene_graph_;
	std::vector<pyro::SceneNode*>               scene_layers_;

	Player*                                     player_;
	data::SurvivorDataHolder                    survivor_data_;
	pyro::TextureHolder<Survivor::SurvivorType> survivor_textures_;

	sf::VertexArray                             background_;
	sf::Texture                                 background_texture_;

	sf::FloatRect                               world_bounds_;
};
#endif