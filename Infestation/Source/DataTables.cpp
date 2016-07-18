#include "Survivor.h"

#include <iostream>

namespace data
{
	SurvivorDataHolder initializeSurvivorData()
	{
		SurvivorDataHolder data;
		auto survivor_data(std::make_unique<SurvivorData>());
		data.push_back(std::move(survivor_data));
		data[Survivor::Survivor1]->entity_type = Entity::Survivor;
		data[Survivor::Survivor1]->size        = sf::Vector2f();
		data[Survivor::Survivor1]->speed       = 150.f;
		data[Survivor::Survivor1]->name        = "Survivor";
		data[Survivor::Survivor1]->health      = 100;
		data[Survivor::Survivor1]->damage      = 25;
		data[Survivor::Survivor1]->survivor_type = Survivor::Survivor1;

		return std::move(data);
	}
}