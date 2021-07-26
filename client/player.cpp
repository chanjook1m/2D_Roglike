#include "Player.h"

Player::Player(int width, int height)
{
    spriteWidth = width;
    spriteHeight = height;
	collisionRect.setSize(sf::Vector2f(spriteWidth, spriteHeight));	
	collisionRect.setPosition(300, 400);
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
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && canMoveUp)
    {
        collisionRect.move(0.f, -velocity);
        sprite.setTextureRect(sf::IntRect(walkSpriteNumber * spriteWidth, spriteHeight * 1, spriteWidth, spriteHeight));
        direction = 1;

        
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && canMoveDown)
    {
        collisionRect.move(0.f, velocity);
        sprite.setTextureRect(sf::IntRect(walkSpriteNumber * spriteWidth, 0, spriteWidth, spriteHeight));
        direction = 2;

        
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && canMoveLeft)
    {
        collisionRect.move(-velocity, 0.f);
        sprite.setTextureRect(sf::IntRect(walkSpriteNumber * spriteWidth, spriteHeight * 2, spriteWidth, spriteHeight));
        direction = 3;

        
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && canMoveRight)
    {
        collisionRect.move(velocity, 0.f);
        sprite.setTextureRect(sf::IntRect(walkSpriteNumber * spriteWidth, spriteHeight * 3, spriteWidth, spriteHeight));
        direction = 4;

        
    } 
    else
    {
        canMoveUp = true;
        canMoveDown = true;
        canMoveLeft = true;
        canMoveRight = true;
    }
    
    walkSpriteNumber++;
    if (walkSpriteNumber == 7) {
        walkSpriteNumber = 0;
    }
}