#include <iostream>
#include <SFML/Graphics.hpp>
#include "Utils.h"
#include "Terrain.h"
#include "Tank.h"

int main()
{   
    const int width = 1500;
    const int height = 800;
    sf::RenderWindow window(sf::VideoMode(width, height), "SFML works!");

    window.setFramerateLimit(60);
    
    Terrain terrain(false,300, 8, window);
    Tank tank1(sf::Vector2f(width * 0.25f, 500), sf::Keyboard::Q);
    Tank tank2(sf::Vector2f(width * 0.75f, 500), sf::Keyboard::E);

    while (window.isOpen())
    {
        Utils::updateDt();

        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                terrain.explosionEffect(mousePos, 40);
            }
        }

        //std::cout << terrain.checkTerrainCollision(mousePos) << std::endl;

        tank1.update();
        tank2.update();


        if (terrain.checkTerrainCollision(tank1.getRocketPositon())) {
            terrain.explosionEffect(tank1.getRocketPreviousPositon(), 40);
            tank1.resetRocket();
        }
        if (terrain.checkTerrainCollision(tank2.getRocketPositon())) {
            terrain.explosionEffect(tank2.getRocketPreviousPositon(), 40);
            tank2.resetRocket();
        }


        if (terrain.checkTerrainCollision(tank1.position)) {
            tank1.disableGravity();
        }
        else {
            tank1.enableGravity();
        }

        if (terrain.checkTerrainCollision(tank2.position)) {
            tank2.disableGravity();
        }
        else {
            tank2.enableGravity();
        }

        window.clear();
        
        terrain.display(window);
        
        tank1.display(window);
        tank2.display(window);
        
        window.display();
    }

    return 0;
}

