#include "GameState.h"

GameState::GameState(pyro::StateStack* stack, sf::RenderWindow* window)
	: State(stack, window)
	, camera_(nullptr)
	, blood_splatter_lifetime_(sf::seconds(30.f))
	, crosshair_(nullptr)
	, wave_(0)
	, infected_per_wave_(6)
	, world_bounds_(0.f, 0.f, 2500.f, 2500.f)
{
	window_->setMouseCursorVisible(false);
	
	setupResources();
	projectile_data_     = std::move(data::initializeProjectileData(projectile_textures_));
	survivor_data_       = std::move(data::initializeSurvivorData(survivor_textures_));
	infected_data_       = std::move(data::initializeInfectedData(infected_textures_));
	blood_spray_data_    = std::move(data::initializeBloodSprayData(blood_spray_textures_));
	blood_splatter_data_ = std::move(data::initializeBloodSplatterData(blood_splatter_textures_));

	buildScene();

	auto wave_text(std::make_unique<pyro::Text>());
	wave_text->loadFontFromFile("Assets/Fonts/Waves.ttf");
	wave_text->setOriginFlags(pyro::utils::OriginFlag::Right | pyro::utils::OriginFlag::Top);
	wave_text->setTextColor(sf::Color::White);
	wave_text->activateShadow(true);
	wave_text->setShadowColor(sf::Color::Black);
	wave_text->setShadowOffset(2.f, 2.f);
	wave_text->setPosition(window_->getSize().x - 5.f, 5.f);
	wave_text_ = wave_text.get();
	camera_->attachChild(std::move(wave_text));

	music_player_.setVolume(70.f);
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
	handleProjectiles();
	handleInfected();
	handleSurvivors();

	cleanupBloodSplatterAnimations();
	cleanupBloodSplatters(dt);
	setRandomMusicTrack();

	updateWave();

	if (survivors_.empty()) {
		requestStateClear();
		return false;
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

void GameState::updateWave()
{
	if (infected_.empty()) {
		wave_text_->setString("Wave " + std::to_string(++wave_));
		for (size_t i = 0; i < wave_ * infected_per_wave_; ++i) {
			auto infected(std::make_unique<Infected>(world_bounds_, &survivors_,
				                                     infected_data_[Infected::Zombie].get(),
				                                     &blood_spray_data_));
			infected_.push_back(infected.get());
			scene_layers_[Characters]->attachChild(std::move(infected));
		}
	}
}

void GameState::setRandomMusicTrack()
{
	if (music_player_.isTrackOver()) {
		music_player_.play(static_cast<MusicID>(rand() % TotalTracks), false);
	}
}

void GameState::addBloodSplatter(const Infected& dead_infected)
{
	const data::BloodSplatterData* blood_splatter_data = blood_splatter_data_[rand() % Infected::BloodSplatterCount].get();
	std::unique_ptr<pyro::SpriteNode> blood_splatter_sprite;
	if (blood_splatter_data->animation_rects) {
		blood_splatter_sprite = std::make_unique<pyro::SpriteNode>(*blood_splatter_data->texture,
			static_cast<sf::IntRect>(blood_splatter_data->animation_rects->front().first));
	}
	else {
		blood_splatter_sprite = std::make_unique<pyro::SpriteNode>(*blood_splatter_data->texture);
	}
	blood_splatter_sprite->setPosition(dead_infected.getWorldPosition());
	blood_splatter_sprite->setRotation(dead_infected.getRotation() - 45.f);

	if (blood_splatter_data->animation_rects) {
		auto blood_splatter(std::make_unique<Animation>(*blood_splatter_data->animation_rects.get(), sf::seconds(0.35f),
		                                                 blood_splatter_sprite.get()));
		blood_splatter_animations_.push_back(blood_splatter.get());
		scene_layers_[BloodDecals]->attachChild(std::move(blood_splatter));
	}
	else {
		blood_splatter_sprite->setOriginFlags(pyro::utils::OriginFlag::Center);
		blood_splatter_sprite->scale(0.3f, 0.3f);
	}
	blood_splatters_.push_back(std::make_pair(blood_splatter_sprite.get(), blood_splatter_lifetime_));
	scene_layers_[BloodDecals]->attachChild(std::move(blood_splatter_sprite));

	std::cout << blood_splatters_.size() << std::endl;
}

void GameState::cleanupBloodSplatters(sf::Time dt)
{
	for (size_t i = 0; i < blood_splatters_.size(); ++i) {
		if ((blood_splatters_[i].second -= dt) <= sf::Time::Zero) {
			scene_layers_[BloodDecals]->detachChild(*blood_splatters_[i].first);
			blood_splatters_.erase(blood_splatters_.begin() + i);

			std::cout << blood_splatters_.size() << std::endl;
		}
	}
}

void GameState::cleanupBloodSplatterAnimations()
{
	for (size_t i = 0; i < blood_splatter_animations_.size(); ++i) {
		if (blood_splatter_animations_[i]->isDestroyed()) {
			scene_layers_[BloodDecals]->detachChild(*blood_splatter_animations_[i]);
			blood_splatter_animations_.erase(blood_splatter_animations_.begin() + i);
		}
	}
}

void GameState::handleProjectiles()
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
			sound_player_.play(SoundID::PistolFire, survivors_[i]->getPosition(), 26.f);
			auto projectile(std::make_unique<Projectile>(window_->getSize(),
				survivors_[i]->getPosition(), survivors_[i]->getRotation(),
				projectile_data_[Projectile::PistolRound].get()));

			projectiles_[survivors_[i]].push_back(projectile.get());
			scene_layers_[Bullets]->attachChild(std::move(projectile));
		}
	}

	for (const auto& survivor : projectiles_) {
		for (auto& itr = survivor.second.rbegin(); itr != survivor.second.rend(); ++itr) {
			for (auto& infected : infected_) {
				if ((*itr)->checkCollision(*infected)) {
					sound_player_.play(rand() % 2 == 0 ? SoundID::BloodSquirt1 : SoundID::BloodSquirt2,
						               survivor.first->getWorldPosition(), 70.f);
					survivor.first->dealDamage(infected);
					break;
				}
			}
		}
	}
}
void GameState::handleInfected()
{
	// Delete Infected if Necessary
	for (size_t i = 0; i < infected_.size(); ++i) {
		if (infected_[i]->isDestroyed()) {
			addBloodSplatter(*infected_[i]);

			scene_layers_[Characters]->detachChild(*infected_[i]);
			infected_.erase(infected_.begin() + i);
		}
	}
}

void GameState::handleSurvivors()
{
	// Check Infected-Survivor Collision
	for (size_t i = 0; i < survivors_.size(); ++i) {
		for (const auto& infected : infected_) {
			if (survivors_[i]->checkCollision(*infected)) {
				infected->dealDamage(survivors_[i]);
				if (survivors_[i]->isDestroyed()) {
					scene_layers_[Characters]->detachChild(*survivors_[i]);
					survivors_.erase(survivors_.begin() + i);
					break;
				}
			}
		}
	}
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

	// Add Camera to Scene Layer: GUI
	auto camera(std::make_unique<Camera>(world_bounds_, survivors_, window_));
	camera_ = camera.get();
	scene_layers_[GUI]->attachChild(std::move(camera));

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
		// General
	crosshair_texture_.loadFromFile("Assets/Textures/Crosshair.png");
	background_texture_.loadFromFile("Assets/Textures/RockTerrain.jpg");
	background_texture_.setRepeated(true);
	muzzle_flash_texture_.loadFromFile("Assets/Textures/MuzzleFlash.png");
		// Projectiles
	projectile_textures_.load("Assets/Textures/Bullet_Pistol.png", Projectile::PistolRound);
		// Survivors
	survivor_textures_.load("Assets/Textures/Player_Pistol.png", Survivor::Survivor1);
		// Infected
	infected_textures_.load("Assets/Textures/Zombie.png", Infected::Zombie);
		// Blood
			// Blood Spray
	blood_spray_textures_.load("Assets/Textures/Blood_Spray_1.png", Infected::BloodSprayType::BloodSpray1);
	blood_spray_textures_.load("Assets/Textures/Blood_Spray_2.png", Infected::BloodSprayType::BloodSpray2);
	blood_spray_textures_.load("Assets/Textures/Blood_Spray_3.png", Infected::BloodSprayType::BloodSpray3);
			// Blood Splatter
	blood_splatter_textures_.load("Assets/Textures/Blood_Splatter_1.png", Infected::BloodSplatterType::BloodSplatter1);
	blood_splatter_textures_.get(Infected::BloodSplatterType::BloodSplatter1).setSmooth(true);
	blood_splatter_textures_.load("Assets/Textures/Blood_Splatter_2.png", Infected::BloodSplatterType::BloodSplatter2);
	blood_splatter_textures_.get(Infected::BloodSplatterType::BloodSplatter2).setSmooth(true);
	blood_splatter_textures_.load("Assets/Textures/Blood_Splatter_3.png", Infected::BloodSplatterType::BloodSplatter3);
	blood_splatter_textures_.get(Infected::BloodSplatterType::BloodSplatter3).setSmooth(true);

	// Sounds
	sound_player_.loadEffect(SoundID::PistolFire, "Assets/Sounds/Pistol_Fire.flac");
	sound_player_.loadEffect(SoundID::BloodSquirt1, "Assets/Sounds/Blood_Squirt1.flac");
	sound_player_.loadEffect(SoundID::BloodSquirt2, "Assets/Sounds/Blood_Squirt2.flac");

	// Music
	music_player_.loadTheme(MusicID::Track1, "Assets/Music/Track1_TheWolf.ogg");

	// Fonts
	wave_font_.loadFromFile("Assets/Fonts/Waves.ttf");
}