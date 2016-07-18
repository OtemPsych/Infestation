#ifndef Survivor_H_
#define Survivor_H_

#include "Character.h"

class Survivor : public Character
{
public:
	enum SurvivorType { Survivor1, SurvivorTypeCount };

public:
	Survivor(const sf::Texture* texture, data::SurvivorData* survivor_data);
	virtual ~Survivor();

protected:
	data::SurvivorData* survivor_data_;
};
#endif