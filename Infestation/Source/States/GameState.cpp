#include "GameState.h"

GameState::GameState(pyro::StateStack* stack, sf::RenderWindow* window)
	: State(stack, window)
	, camera_(nullptr)
	, crosshair_(nullptr)
	, world_bounds_(0.f, 0.f, 2000.f, 2000.f)
{
	window_->setMouseCursorVisible(false);

	setupResources();
	projectile_data_ = std::move(data::initializeProjectileData(projectile_textures_));
	survivor_data_ = std::move(data::initializeSurvivorData(survivor_textures_));

	buildScene();
}

bool GameState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed &&
		                                    event.key.code == sf::Keyboard::Escape)) {
		requestStateClear();
		return false;
	}

	scene_graph_.handleEvent(event);

	return true;
}

bool GameState::update(sf::Time dt)
{
	// Add/Remove Projectiles
	for (size_t i = 0; i < survivors_.size(); ++i) {
		for (size_t j = 0; j < projectiles_[survivors_[i]].size(); ++j) {
			if (projectiles_[survivors_[i]][j]->isDestroyed()) {
				scene_layers_[Bullets]->detachChild(*projectiles_[survivors_[i]][j]);
				projectiles_[survivors_[i]].erase(projectiles_[survivors_[i]].begin() + j);
			}
		}

		if (survivors_[i]->wasGunFired()) {
			sound_player_.play(SoundID::PistolFire, survivors_[i]->getPosition(), 35.f);
			auto projectile(std::make_unique<Projectile>(window_->getSize(),
				survivors_[i]->getPosition(), survivors_[i]->getRotation(),
				projectile_data_[Projectile::PistolRound].get()));

			projectiles_[survivors_[i]].push_back(projectile.get());
			scene_layers_[Bullets]->attachChild(std::move(projectile));
		}
	}

	// Update Crosshair's Position
	crosshair_->setPosition(window_->mapPixelToCoords(sf::Mouse::getPosition(*window_)));

	scene_graph_.update(dt);

	return true;
}

void GameState::draw()
{
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

	// Add Background to Scene Layer: Background
	auto background(std::make_unique<pyro::SpriteNode>(background_texture_, static_cast<sf::IntRect>(world_bounds_)));
	scene_layers_[Background]->attachChild(std::move(background));

	// Add Camera to Scene Layer: Background
	auto camera(std::make_unique<Camera>(world_bounds_, survivors_, window_));
	camera_ = camera.get();
	scene_layers_[Background]->attachChild(std::move(camera));

	// Add Player to Scene Layer: Characters
	auto player(std::make_unique<Player>(world_bounds_, muzzle_flash_texture_,
		                                 survivor_data_[Survivor::Survivor1].get(), window_));
	player->setPosition(world_bounds_.width / 2.f, world_bounds_.height / 2.f);
	survivors_.push_back(player.get());
	projectiles_.insert(std::make_pair(player.get(), std::vector<Projectile*>()));
	scene_layers_[Characters]->attachChild(std::move(player));

	// Add Crosshair to Scene Layer: GUI
	auto crosshair(std::make_unique<pyro::SpriteNode>(crosshair_texture_));
	crosshair->setOriginFlags(pyro::utils::OriginFlag::Center);
	crosshair->scale(0.2f, 0.2f);
	crosshair_ = crosshair.get();
	scene_layers_[GUI]->attachChild(std::move(crosshair));
}

void GameState::setupResources()
{
	// Textures
	crosshair_texture_.loadFromFile("Assets/Textures/Crosshair.png");
	background_texture_.loadFromFile("Assets/Textures/RockTerrain.jpg");
	background_texture_.setRepeated(true);
	muzzle_flash_texture_.loadFromFile("Assets/Textures/MuzzleFlash.png");

	projectile_textures_.load("Assets/Textures/Bullet_Pistol.png", Projectile::PistolRound);

	survivor_textures_.load("Assets/Textures/Player_Pistol.png", Survivor::Survivor1);


	// Sounds
	sound_player_.loadEffect(SoundID::PistolFire, "Assets/Sounds/Pistol_Fire.flac");
}