#include "GameState.h"

GameState::GameState(pyro::StateStack* stack, sf::RenderWindow* window)
	: State(stack, window)
	, player_(nullptr)
	, survivor_data_(std::move(data::initializeSurvivorData()))
	, background_(sf::Quads)
	, world_bounds_(0.f, 0.f, 2000.f, 2000.f)
{
	setupResources();
	initBackground();

	buildScene();
}

bool GameState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::Closed) {
		requestStateClear();
		return false;
	}

	scene_graph_.handleEvent(event);

	return true;
}

bool GameState::update(sf::Time dt)
{
	scene_graph_.update(dt);

	return true;
}

void GameState::draw()
{
	window_->draw(background_, &background_texture_);
	window_->draw(scene_graph_);
}

void GameState::buildScene()
{
	// Add Scene Layers to Scene
	for (unsigned short i = 0; i < SceneLayerCount; ++i) {
		auto scene_layer(std::make_unique<pyro::SceneNode>());

		scene_layers_.push_back(scene_layer.get());
		scene_graph_.attachChild(std::move(scene_layer));
	}

	// Add Survivor to Scene Layer: Entities
	auto player(std::make_unique<Player>(world_bounds_, &survivor_textures_.get(Survivor::Survivor1),
		                                 survivor_data_[Survivor::Survivor1].get(), window_));
	player_ = player.get();
	scene_layers_[Entities]->attachChild(std::move(player));
}

void GameState::initBackground()
{
	background_.append(sf::Vertex(sf::Vector2f(0.f,                 0.f),
		                          sf::Vector2f(0.f,                 0.f)));

	background_.append(sf::Vertex(sf::Vector2f(world_bounds_.width, 0.f),
		                          sf::Vector2f(world_bounds_.width, 0.f)));

	background_.append(sf::Vertex(sf::Vector2f(world_bounds_.width, world_bounds_.height),
		                          sf::Vector2f(world_bounds_.width, world_bounds_.height)));

	background_.append(sf::Vertex(sf::Vector2f(0.f,                 world_bounds_.height),
		                          sf::Vector2f(0.f,                 world_bounds_.height)));
}

void GameState::setupResources()
{
	background_texture_.loadFromFile("Assets/Textures/RockTerrain.jpg");
	background_texture_.setRepeated(true);

	survivor_textures_.load("Assets/Textures/Player_Pistol.png", Survivor::Survivor1);
}