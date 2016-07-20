#include "Survivor.h"

Survivor::Survivor(const sf::Texture& muzzle_flash_texture, data::SurvivorData* survivor_data)
	: Character(survivor_data)
	, survivor_data_(survivor_data)
	, muzzle_flash_(nullptr)
	, muzzle_flash_texture_(muzzle_flash_texture)
	, muzzle_flash_lifetime_(sf::seconds(0.18f))
	, gun_fired_(false)
{
}

Survivor::~Survivor()
{
}

bool Survivor::wasGunFired()
{
	bool gun_state = gun_fired_;
	gun_fired_ = false;

	return gun_state;
}

void Survivor::fireProjectile()
{
	gun_fired_ = true;

	if (!muzzle_flash_) {
		auto muzzle_flash(std::make_unique<pyro::SpriteNode>(muzzle_flash_texture_));
		muzzle_flash->scale(0.4f, 0.4f);
		muzzle_flash->setOriginFlags(pyro::utils::OriginFlag::Left | pyro::utils::OriginFlag::CenterY);
		muzzle_flash->setPosition(getLocalBounds().width - 2.f, getLocalBounds().height / 2.f);
		muzzle_flash_ = muzzle_flash.get();
		attachChild(std::move(muzzle_flash));
	}
	else {
		muzzle_flash_lifetime_.current = muzzle_flash_lifetime_.original;
	}
}

void Survivor::updateCurrent(sf::Time dt)
{
	if (muzzle_flash_ && (muzzle_flash_lifetime_.current -= dt) <= sf::Time::Zero) {
		muzzle_flash_lifetime_.current = muzzle_flash_lifetime_.original;

		detachChild(*muzzle_flash_);
		muzzle_flash_ = nullptr;
	}
}