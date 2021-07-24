#include "Player.h"

Player::Player(int width, int height)
{
    spriteWidth = width;
    spriteHeight = height;
	collisionRect.setSize(sf::Vector2f(spriteWidth, spriteHeight));	
	collisionRect.setPosition(400, 200);
	collisionRect.setFillColor(sf::Color::Blue);
    velocity = 2.0f;
    sprite.setTextureRect(sf::IntRect(0,0, spriteWidth, spriteHeight));
	sprite.setPosition(400, 200);
}

Player::~Player()
{

}

void Player::update()
{
    move();
	sprite.setPosition(collisionRect.getPosition());
}

void Player::move()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        collisionRect.move(0.f, -velocity);
        sprite.setTextureRect(sf::IntRect(walkSpriteNumber * spriteWidth, spriteHeight * 1, spriteWidth, spriteHeight));
        direction = 1;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        collisionRect.move(0.f, velocity);
        sprite.setTextureRect(sf::IntRect(walkSpriteNumber * spriteWidth, 0, spriteWidth, spriteHeight));
        direction = 2;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        collisionRect.move(-velocity, 0.f);
        sprite.setTextureRect(sf::IntRect(walkSpriteNumber * spriteWidth, spriteHeight * 2, spriteWidth, spriteHeight));
        direction = 3;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        collisionRect.move(velocity, 0.f);
        sprite.setTextureRect(sf::IntRect(walkSpriteNumber * spriteWidth, spriteHeight * 3, spriteWidth, spriteHeight));
        direction = 4;
    }

    walkSpriteNumber++;
    if (walkSpriteNumber == 7) {
        walkSpriteNumber = 0;
    }
}