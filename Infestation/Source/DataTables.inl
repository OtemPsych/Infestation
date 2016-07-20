namespace data
{
	template <typename ID>
	SurvivorDataHolder initializeSurvivorData(const pyro::TextureHolder<ID>& survivor_textures)
	{
		SurvivorDataHolder data;
		auto survivor_data(std::make_unique<SurvivorData>());
		data.push_back(std::move(survivor_data));
		data[Survivor::Survivor1]->entity_type   = static_cast<unsigned short>(Entity::EntityType::Survivor);
		data[Survivor::Survivor1]->texture       = &survivor_textures.get(Survivor::Survivor1);
		data[Survivor::Survivor1]->size          = sf::Vector2f();
		data[Survivor::Survivor1]->speed         = 250;
		data[Survivor::Survivor1]->name          = "Survivor";
		data[Survivor::Survivor1]->health        = 100;
		data[Survivor::Survivor1]->damage        = 25;
		data[Survivor::Survivor1]->survivor_type = Survivor::Survivor1;

		return std::move(data);
	}

	template <typename ID>
	ProjectileDataHolder initializeProjectileData(const pyro::TextureHolder<ID>& projectile_textures)
	{
		ProjectileDataHolder data;
		auto projectile_data(std::make_unique<EntityData>());
		data.push_back(std::move(projectile_data));
		data[Projectile::PistolRound]->entity_type = static_cast<unsigned short>(Entity::EntityType::Projectile);
		data[Projectile::PistolRound]->texture     = &projectile_textures.get(Projectile::PistolRound);
		data[Projectile::PistolRound]->size        = sf::Vector2f(20.f, 8.8f);
		data[Projectile::PistolRound]->speed       = 1500;

		return std::move(data);
	}
}