#include <SFML\Audio.hpp>
#include <SFML\Graphics.hpp>
#include <iostream>

#include "player.h"
#include "enemy.h"
#include "projectile.h"
#include "random.h"
#include "ingameText.h"

#define RESOURCE_DIR (string)"C:\\Users\\1z3r0\\Desktop\\game_project\\RPG\\Resources\\"
using namespace std;

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

    vector<Enemy>::const_iterator enemyIter;
    vector<Enemy> enemyArr;

    Enemy enemy(0,0, 37, 50);
    enemy.sprite.setTexture(enemyTexture);
    enemyArr.push_back(enemy);

    vector<IngameText>::const_iterator ingameTextIter;
    vector<IngameText> ingameTextArr;

    IngameText ingameText;
    ingameText.text.setPosition(200, 200);
    ingameText.text.setFont(maumFont);
    ingameTextArr.push_back(ingameText);


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

        sf::Time projectileClockElapsed = projectileClock.getElapsedTime();
        sf::Time playerCollisionClockElapsed = playerCollisionClock.getElapsedTime();
        sf::Time elapsed3 = clock3.getElapsedTime();

        // handle collision
        counter = 0;
        for (projectileIter = projectileArr.begin(); projectileIter != projectileArr.end(); projectileIter++)
        {
            counter2 = 0;
            for (enemyIter = enemyArr.begin(); enemyIter != enemyArr.end(); enemyIter++)
            {
                if (projectileArr[counter].collisionRect.getGlobalBounds().intersects(enemyArr[counter2].collisionRect.getGlobalBounds()))
                { 
                    projectileArr[counter].isAlive = false;

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

        counter = 0;
        for (enemyIter = enemyArr.begin(); enemyIter != enemyArr.end(); enemyIter++)
        {
            if (enemyArr[counter].isAlive == false)
            {
                enemyArr.erase(enemyIter);
                break;
            }
            counter++;
        }

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



        if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
        {
            enemy.collisionRect.setPosition(generateRandom(window.getSize().x), generateRandom(window.getSize().y));
            enemyArr.push_back(enemy);
        }

        if (projectileClockElapsed.asSeconds() >= 0.5 && sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            projectileClock.restart();
            projectile.collisionRect.setPosition(
                player1.collisionRect.getPosition().x + player1.collisionRect.getSize().x/2 - projectile.collisionRect.getSize().x/2,
                player1.collisionRect.getPosition().y + player1.collisionRect.getSize().y/2 - projectile.collisionRect.getSize().y/2);
            projectile.direction = player1.direction;
            projectileArr.push_back(projectile);
        }

        counter = 0;
        for (projectileIter = projectileArr.begin(); projectileIter != projectileArr.end(); projectileIter++)
        {
            projectileArr[counter].update();
            window.draw(projectileArr[counter].collisionRect);
            counter++;
        }

        counter = 0;
        for (enemyIter = enemyArr.begin(); enemyIter != enemyArr.end(); enemyIter++)
        {
            
            //window.draw(enemyArr[counter].collisionRect);
            window.draw(enemyArr[counter].sprite);
            enemyArr[counter].update();
            counter++;
        }

        window.draw(player1.sprite);
        player1.update();
        
        counter = 0;
        for (ingameTextIter = ingameTextArr.begin(); ingameTextIter != ingameTextArr.end(); ingameTextIter++)
        {
            window.draw(ingameTextArr[counter].text);
            ingameTextArr[counter].update();
            counter++;
        }
        //
        window.display();
    }
    return 0;
}