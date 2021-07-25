#include "wall.h"

Wall::Wall()
{
	collisionRect.setSize(sf::Vector2f(50, 50));
	collisionRect.setPosition(0, 0);
	collisionRect.setFillColor(sf::Color::White);
}

Wall::~Wall()
{

}