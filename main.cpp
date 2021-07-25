#include <SFML\Audio.hpp>
#include <SFML\Graphics.hpp>
#include <iostream>

#include "player.h"
#include "enemy.h"
#include "projectile.h"
#include "random.h"
#include "ingameText.h"
#include "item.h"
#include "animatedGIF.h"

#define RESOURCE_DIR (string)"C:\\Users\\1z3r0\\Desktop\\game_project\\RPG\\Resources\\"
using namespace std;

enum types
{
    COIN = 1,
    ETC
};

int main()
{
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
    energyBallSprite.setScale(sf::Vector2f(0.2, 0.2));

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

    vector<IngameText>::const_iterator ingameTextIter;
    vector<IngameText> ingameTextArr;

    IngameText ingameText;
    ingameText.text.setPosition(200, 200);
    ingameText.text.setFont(maumFont);
    ingameTextArr.push_back(ingameText);

    sf::Text scoreText("Score: ", maumFont, 50);
    scoreText.setPosition(50, 50);

    int counter;
    int counter2;

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

        // create enemy (c-Key)
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
        {
            enemy.collisionRect.setPosition(generateRandom(window.getSize().x), generateRandom(window.getSize().y));
            enemyArr.push_back(enemy);
        }

        // create projectile (space-Key)
        if (projectileClockElapsed.asSeconds() >= 0.5 && sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
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