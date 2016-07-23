#ifndef GameState_H_
#define GameState_H_

#include <SFML/Graphics/Texture.hpp>

#include <PYRO/State.h>
#include <PYRO/ResourceHolder.h>
#include <PYRO/SpriteNode.h>
#include <PYRO/Audio/SoundPlayer.h>
#include <PYRO/Audio/MusicPlayer.h>

#include "../Camera.h"
#include "../Player.h"
#include "../Projectile.h"
#include "../Infected.h"

class GameState : public pyro::State
{
private:
	using Projectiles = std::vector<Projectile*>;
	enum SceneLayers { Background, BloodDecals, Bullets, Characters, GUI, SceneLayerCount };
	enum SoundID { PistolFire, BloodSquirt1, BloodSquirt2 };
	enum MusicID { Track1, TotalTracks };

public:
	GameState(pyro::StateStack* stack, sf::RenderWindow* window);
public:
	virtual bool handleEvent(const sf::Event& event) override;
	virtual bool update(sf::Time dt) override;
	virtual void draw() override;
private:
	void setRandomMusicTrack();

	void addBloodSplatter(const Infected& dead_infected);
	void cleanupBloodSplatterAnimations();

	void handleProjectiles();
	void handleInfected();
	void handleSurvivors();
	void buildScene();
	virtual void setupResources() override;

private:
	pyro::SceneNode                                  scene_graph_;
	std::vector<pyro::SceneNode*>                    scene_layers_;
												     
	Camera*                                          camera_;
													 
	std::vector<Survivor*>                           survivors_;
	data::SurvivorDataHolder                         survivor_data_;
	pyro::TextureHolder<Survivor::SurvivorType>      survivor_textures_;
													 
	std::map<Survivor*, std::vector<Projectile*>>    projectiles_;
	data::ProjectileDataHolder                       projectile_data_;
	pyro::TextureHolder<Projectile::ProjectileType>  projectile_textures_;
													 
	std::vector<Infected*>                           infected_;
	data::InfectedDataHolder                         infected_data_;
	pyro::TextureHolder<Infected::InfectedType>      infected_textures_;
													 
	data::BloodSprayDataHolder                       blood_spray_data_;
	pyro::TextureHolder<Infected::BloodSprayType>    blood_spray_textures_;

	data::BloodSplatterDataHolder                    blood_splatter_data_;
	pyro::TextureHolder<Infected::BloodSplatterType> blood_splatter_textures_;
	std::vector<Animation*>                          blood_splatter_animations_;
													 
	pyro::SoundPlayer<SoundID>                       sound_player_;
	pyro::MusicPlayer<MusicID>                       music_player_;
													 
	pyro::SpriteNode*                                crosshair_;
	sf::Texture                                      crosshair_texture_;
	sf::Texture                                      background_texture_;
	sf::Texture                                      muzzle_flash_texture_;
												     
	sf::FloatRect                                    world_bounds_;
};
#endif