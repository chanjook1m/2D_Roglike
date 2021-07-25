#include "enemy.h"

Enemy::Enemy(int x, int y, int width, int height)
{
    spriteWidth = width;
    spriteHeight = height;
    collisionRect.setSize(sf::Vector2f(spriteWidth, spriteHeight));
    collisionRect.setPosition(400, 200);
    collisionRect.setFillColor(sf::Color::Blue);    
    sprite.setTextureRect(sf::IntRect(x, y, spriteWidth, spriteHeight));
}

Enemy::~Enemy()
{

}

void Enemy::update()
{
    move();
    sprite.setPosition(collisionRect.getPosition());
}

void Enemy::move()
{
    if (direction == 1 && canMoveUp)
    {
        collisionRect.move(0.f, -velocity);
        sprite.setTextureRect(sf::IntRect(walkSpriteNumber * spriteWidth, spriteHeight * 2, spriteWidth, spriteHeight));
    }
    else if (direction == 2 && canMoveDown)
    {
        collisionRect.move(0.f, velocity);
        sprite.setTextureRect(sf::IntRect(walkSpriteNumber * spriteWidth, 0, spriteWidth, spriteHeight));
    }
    else if (direction == 3 && canMoveLeft)
    {
        collisionRect.move(-velocity, 0.f);
        sprite.setTextureRect(sf::IntRect(walkSpriteNumber * spriteWidth, spriteHeight * 1, spriteWidth, spriteHeight));
    }
    else if (direction == 4 && canMoveRight)
    {
        collisionRect.move(velocity, 0.f);
        sprite.setTextureRect(sf::IntRect(walkSpriteNumber * spriteWidth, spriteHeight * 3, spriteWidth, spriteHeight));
    }
    else
    {
        canMoveUp = true;
        canMoveDown = true;
        canMoveLeft = true;
        canMoveRight = true;
    }

    walkSpriteNumber++;
    if (walkSpriteNumber == 5) {
        walkSpriteNumber = 0;
    }

    delayCounter++;
    if (delayCounter == movementDelay)
    {
        delayCounter = 0;
        direction = generateRandom(4);
    }
}
