#pragma once

#include "entity.h"
#include "random.h"

class Enemy: public Entity
{
public:
	float velocity = 0.f;
	float attackDamage = 0.f;
	int walkSpriteNumber = 0;
	int direction = 0;
	int delayCounter = 0;
	int movementDelay = 100;
	int hp = 3;


	Enemy(int x, int y, int width, int height);
	~Enemy();

	void update();
	void move();

};

