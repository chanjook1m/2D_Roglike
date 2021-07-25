#pragma once

#include "entity.h"

class Wall: public Entity
{
public:
	Wall();
	~Wall();

	bool destructible = false;
	int hp = 3;
};

