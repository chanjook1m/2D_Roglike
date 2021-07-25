#include <SFML\Audio.hpp>
#include <SFML\Graphics.hpp>
#include <iostream>

#include "player.h"
#include "enemy.h"
#include "projectile.h"
#include "random.h"
#include "ingameText.h"
#include "item.h"
#include "wall.h"
#include "animatedGIF.h"
#define RESOURCE_DIR (string)"C:\\Users\\1z3r0\\Desktop\\game\\2D_Roguelike\\Resources\\"
using namespace std;

enum types
{
    COIN = 1,
    ETC
};

int main()
{
    int counter;
    int counter2;

    sf::RenderWindow window(sf::VideoMode(1000, 800), "My RPG");
    window.setFramerateLimit(60);

    sf::Texture playerTexture;
    if (!playerTexture.loadFromFile(RESOURCE_DIR + "rpg_sprite_walk.png")) 
    {
        return EXIT_FAILURE;
    }

    sf::Texture enemyTexture;
    if (!enemyTexture.loadFromFile(RESOURCE_DIR + "monsta.png")) 
    {
        return EXIT_FAILURE;
    }

    sf::Texture coinTexture;
    if (!coinTexture.loadFromFile(RESOURCE_DIR + "coin.png"))
    {
        return EXIT_FAILURE;
    }

    AnimatedGIF gif((RESOURCE_DIR + "energyball.gif").c_str());
    sf::Sprite energyBallSprite;
    energyBallSprite.setScale(sf::Vector2f(0.1, 0.1));

    sf::Font maumFont;
    if (!maumFont.loadFromFile(RESOURCE_DIR + "godoMaum.ttf")) 
    {
        return EXIT_FAILURE;
    }

    Player player1(24, 32);
    player1.sprite.setTexture(playerTexture);

    vector<Projectile>::const_iterator projectileIter;
    vector<Projectile> projectileArr;
    Projectile projectile;
    projectile.sprite = energyBallSprite;

    vector<Enemy>::const_iterator enemyIter;
    vector<Enemy> enemyArr;

    Enemy enemy(0,0, 37, 50);
    enemy.sprite.setTexture(enemyTexture);
    enemyArr.push_back(enemy);

    vector<Item>::const_iterator itemIter;
    vector<Item> itemArr;

    Item item(10,10, 200, 150, COIN);
    item.sprite.setTexture(coinTexture);
    item.sprite.setScale(sf::Vector2f(0.2, 0.2));
    item.collisionRect.setPosition(500, 500);
    itemArr.push_back(item);

    vector<Wall>::const_iterator wallIter;
    vector<Wall> wallArr;
    Wall wall;

    //wall.destructible = true;
    int roomSize = 5;
    int verticalDoorAt = 2;
    int horizontalDoorAt = 2;
    int initialRoomX = 200;
    int initialRoomY = 300;
    
    counter = 0;
    while (counter < roomSize)
    {
        wall.collisionRect.setPosition(50 * counter + initialRoomX, initialRoomY);
        wall.destructible = generateRandomBool();
        wall.collisionRect.setFillColor(wall.destructible ? sf::Color::Magenta : sf::Color::White);
        wallArr.push_back(wall);
        wall.collisionRect.setPosition(50 * counter + initialRoomX, 50 * roomSize + initialRoomY);
        wall.destructible = generateRandomBool();
        wall.collisionRect.setFillColor(wall.destructible ? sf::Color::Magenta : sf::Color::White);
        wallArr.push_back(wall);
        wall.collisionRect.setPosition(initialRoomX, 50 * counter + initialRoomY);
        wall.destructible = generateRandomBool();
        wall.collisionRect.setFillColor(wall.destructible ? sf::Color::Magenta : sf::Color::White);
        wallArr.push_back(wall);
        if (counter != verticalDoorAt)
        {
            wall.collisionRect.setPosition(50 * roomSize + initialRoomX, 50 * counter + initialRoomY);
            wall.destructible = generateRandomBool();
            wall.collisionRect.setFillColor(wall.destructible ? sf::Color::Magenta : sf::Color::White);
            wallArr.push_back(wall);
        }
        counter++;
    }
    wall.collisionRect.setPosition(50 * roomSize + initialRoomX, 50 * roomSize + initialRoomY);
    wallArr.push_back(wall);

    vector<IngameText>::const_iterator ingameTextIter;
    vector<IngameText> ingameTextArr;

    IngameText ingameText;
    ingameText.text.setPosition(200, 200);
    ingameText.text.setFont(maumFont);
    ingameTextArr.push_back(ingameText);

    sf::Text scoreText("Score: ", maumFont, 50);
    scoreText.setPosition(50, 50);

    sf::Clock projectileClock;
    sf::Clock playerCollisionClock;
    sf::Clock clock3;

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        //
        sf::Time projectileClockElapsed = projectileClock.getElapsedTime();
        sf::Time playerCollisionClockElapsed = playerCollisionClock.getElapsedTime();
        sf::Time elapsed3 = clock3.getElapsedTime();

        
        // projectile-enemy collision
        counter = 0;
        for (projectileIter = projectileArr.begin(); projectileIter != projectileArr.end(); projectileIter++)
        {
            counter2 = 0;
            for (enemyIter = enemyArr.begin(); enemyIter != enemyArr.end(); enemyIter++)
            {
                if (projectileArr[counter].collisionRect.getGlobalBounds().intersects(enemyArr[counter2].collisionRect.getGlobalBounds()))
                { 
                    projectileArr[counter].isAlive = false;

                    ingameText.text.setFillColor(sf::Color::Red);
                    ingameText.text.setString(to_string((int)projectileArr[counter].attackDamage));
                    ingameText.text.setPosition(enemyArr[counter2].collisionRect.getPosition().x + enemyArr[counter2].collisionRect.getSize().x/2
                        , enemyArr[counter2].collisionRect.getPosition().y - enemyArr[counter2].collisionRect.getSize().y / 2);
                    ingameTextArr.push_back(ingameText);
                        
                    enemyArr[counter2].hp -= projectileArr[counter].attackDamage;
                    if (enemyArr[counter2].hp <= 0)
                    {
                        enemyArr[counter2].isAlive = false;
                    }
                }
                counter2++;
            }
            counter++;
        }

        // player-enemy collision
        if (playerCollisionClockElapsed.asSeconds() >= 0.5)
        {
            playerCollisionClock.restart();
            counter = 0;
            for (enemyIter = enemyArr.begin(); enemyIter != enemyArr.end(); enemyIter++)
            {
                if (player1.collisionRect.getGlobalBounds().intersects(enemyArr[counter].collisionRect.getGlobalBounds()))
                {
                    player1.hp -= enemyArr[counter].attackDamage;

                    ingameText.text.setFillColor(sf::Color::Yellow);
                    ingameText.text.setString(to_string((int)enemyArr[counter].attackDamage));
                    ingameText.text.setPosition(player1.collisionRect.getPosition().x + player1.collisionRect.getSize().x / 2,
                        player1.collisionRect.getPosition().y - player1.collisionRect.getSize().y / 2);
                    ingameTextArr.push_back(ingameText);
                }
                counter++;
            }
        }

        // player-item collision
        counter = 0;
        for (itemIter = itemArr.begin(); itemIter != itemArr.end(); itemIter++)
        {
            if (player1.collisionRect.getGlobalBounds().intersects(itemArr[counter].collisionRect.getGlobalBounds()))
            {
                if (itemArr[counter].type == COIN) {
                    player1.score++;
                }
                itemArr[counter].isAlive = false;
            }
            counter++;
        }

        // player-wall collision
        counter = 0;
        for (wallIter = wallArr.begin(); wallIter != wallArr.end(); wallIter++)
        {
            if (player1.collisionRect.getGlobalBounds().intersects(wallArr[counter].collisionRect.getGlobalBounds()))
            {
                if (player1.direction == 1)
                {
                    player1.canMoveUp = false;
                    player1.collisionRect.move(0, player1.velocity);
                }
                else if (player1.direction == 2)
                {
                    player1.canMoveDown = false;
                    player1.collisionRect.move(0, -player1.velocity);
                }
                else if (player1.direction == 3)
                {
                    player1.canMoveLeft = false;
                    player1.collisionRect.move(player1.velocity, 0);
                }
                else if (player1.direction == 4)
                {
                    player1.canMoveRight = false;
                    player1.collisionRect.move(-player1.velocity, 0);
                }
            }
            counter++;
        }

        // enemy-wall collision
        counter = 0;
        for (enemyIter = enemyArr.begin(); enemyIter != enemyArr.end(); enemyIter++)
        {
            counter2 = 0;
            for (wallIter = wallArr.begin(); wallIter != wallArr.end(); wallIter++)
            {
                if (enemyArr[counter].collisionRect.getGlobalBounds().intersects(wallArr[counter2].collisionRect.getGlobalBounds()))
                {
                    if (enemyArr[counter].direction == 1)
                    {
                        enemyArr[counter].canMoveUp = false;
                        enemyArr[counter].collisionRect.move(0, enemyArr[counter].velocity);
                    }
                    else if (enemyArr[counter].direction == 2)
                    {
                        enemyArr[counter].canMoveDown = false;
                        enemyArr[counter].collisionRect.move(0, -enemyArr[counter].velocity);
                    }
                    else if (enemyArr[counter].direction == 3)
                    {
                        enemyArr[counter].canMoveLeft = false;
                        enemyArr[counter].collisionRect.move(enemyArr[counter].velocity, 0);
                    }
                    else if (enemyArr[counter].direction == 4)
                    {
                        enemyArr[counter].canMoveRight = false;
                        enemyArr[counter].collisionRect.move(-enemyArr[counter].velocity, 0);
                    }
                }

                counter2++;
            }
            counter++;
        }

        // projectile-wall collision
        counter = 0;
        for (projectileIter = projectileArr.begin(); projectileIter != projectileArr.end(); projectileIter++)
        {
            counter2 = 0;
            for (wallIter = wallArr.begin(); wallIter != wallArr.end(); wallIter++)
            {
                if (projectileArr[counter].collisionRect.getGlobalBounds().intersects(wallArr[counter2].collisionRect.getGlobalBounds()))
                {
                    if (wallArr[counter2].destructible == true)
                    {
                        wallArr[counter2].hp -= projectileArr[counter].attackDamage;

                        if (wallArr[counter2].hp <= 0)
                        {
                            wallArr[counter2].isAlive = false;
                        }
                    }

                    projectileArr[counter].isAlive = false;
                }

                counter2++;
            }
            counter++;
        }


        // delete not alive enemy
        counter = 0;
        for (enemyIter = enemyArr.begin(); enemyIter != enemyArr.end(); enemyIter++)
        {
            if (enemyArr[counter].isAlive == false)
            {
                // generate item           
                if (generateRandom(4) == 1)
                {
                    item.collisionRect.setPosition(enemyArr[counter].collisionRect.getPosition());
                    itemArr.push_back(item);
                }

                enemyArr.erase(enemyIter);
                break;
            }
            counter++;
        }

        // delete not alive projectile
        counter = 0;
        for (projectileIter = projectileArr.begin(); projectileIter != projectileArr.end(); projectileIter++)
        {
            if (projectileArr[counter].isAlive == false)
            {
                projectileArr.erase(projectileIter);
                break;
            }
            counter++;
        }

        // delete not alive ingameText
        counter = 0;
        for (ingameTextIter = ingameTextArr.begin(); ingameTextIter != ingameTextArr.end(); ingameTextIter++)
        {
            if (ingameTextArr[counter].isAlive == false)
            {
                ingameTextArr.erase(ingameTextIter);
                break;
            }
            counter++;
        }

        // delete not alive item
        counter = 0;
        for (itemIter = itemArr.begin(); itemIter != itemArr.end(); itemIter++)
        {
            if (itemArr[counter].isAlive == false)
            {
                itemArr.erase(itemIter);
                break;
            }
            counter++;
        }

        // delete not alive wall
        counter = 0;
        for (wallIter = wallArr.begin(); wallIter != wallArr.end(); wallIter++)
        {
            if (wallArr[counter].isAlive == false)
            {
                // generate item           
                if (generateRandom(4) == 1)
                {
                    item.collisionRect.setPosition(wallArr[counter].collisionRect.getPosition());
                    itemArr.push_back(item);
                }

                wallArr.erase(wallIter);
                break;
            }
            counter++;
        }

        // create enemy (c-Key)
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
        {
            enemy.collisionRect.setPosition(generateRandom(window.getSize().x), generateRandom(window.getSize().y));
            enemyArr.push_back(enemy);
        }

        // create projectile (space-Key)
        if (projectileClockElapsed.asSeconds() >= 0.2 && sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            projectileClock.restart();
            projectile.sprite = energyBallSprite;
            projectile.collisionRect.setPosition(
                player1.collisionRect.getPosition().x,
                player1.collisionRect.getPosition().y);
            projectile.direction = player1.direction;
            projectileArr.push_back(projectile);
        }

        // draw projectile
        counter = 0;
        for (projectileIter = projectileArr.begin(); projectileIter != projectileArr.end(); projectileIter++)
        {
            projectileArr[counter].update();
            // draw eneryball
            gif.update(projectileArr[counter].sprite);
            window.draw(projectileArr[counter].sprite);
            counter++;
        }

        // draw enemy
        counter = 0;
        for (enemyIter = enemyArr.begin(); enemyIter != enemyArr.end(); enemyIter++)
        {
            enemyArr[counter].update();
            window.draw(enemyArr[counter].sprite);
            counter++;
        }

        // draw wall
        counter = 0;
        for (wallIter = wallArr.begin(); wallIter != wallArr.end(); wallIter++)
        {
            //wallArr[counter].update();
            window.draw(wallArr[counter].collisionRect);
            counter++;
        }

        // draw player, scoreText
        window.draw(player1.sprite);
        player1.update();
        scoreText.setString("Score: " + to_string(player1.score));
        window.draw(scoreText);
        
        // draw ingameText
        counter = 0;
        for (ingameTextIter = ingameTextArr.begin(); ingameTextIter != ingameTextArr.end(); ingameTextIter++)
        {
            ingameTextArr[counter].update();
            window.draw(ingameTextArr[counter].text);
            counter++;
        }

        // draw item
        counter = 0;
        for (itemIter = itemArr.begin(); itemIter != itemArr.end(); itemIter++)
        {
            itemArr[counter].update();
            window.draw(itemArr[counter].sprite);
            counter++;
        }

        
        //
        window.display();
    }
    return 0;
}