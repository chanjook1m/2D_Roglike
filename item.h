#pragma once

#include "entity.h"

class Item: public Entity
{
public:
	Item(int x, int y, int width, int height, int type);
	~Item();

	enum types
	{
		COIN = 1,
		ETC
	};

	int animateSpriteNumber = 0;
	int delayCounter = 0;
	int animateDelay = 5;
	int type = 0;

	void update();
	void animate();
};

