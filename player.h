#pragma once

#include "entity.h"

class Player: public Entity
{
public:
	float velocity = 0.f;
	float attackDamage = 0.f;
	int walkSpriteNumber = 0;
	int direction = 0; // 1 = up, 2 = down, 3 = left, 4 = right
	int hp = 10;

	Player(int width, int height);
	~Player();

	void update();
	void move();
};

