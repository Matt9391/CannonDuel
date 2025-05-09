#include <iostream>
#include <SFML/Graphics.hpp>
#include "Terrain.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1500, 800), "SFML works!");
    
    Terrain terrain(200, 8, window);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                terrain.explosionEffect(mousePos, 40);
            }
        }

        

        window.clear();
        terrain.display(window);
        window.display();
    }

    return 0;
}