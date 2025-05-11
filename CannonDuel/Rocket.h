#include "SFML/Graphics.hpp"

#pragma once
class Rocket
{
public:
	Rocket(float angle, float force, sf::Vector2f pos);

	void update();
	void display(sf::RenderWindow& window);

	sf::Vector2f getPosition();
	sf::Vector2f getPreviousPosition();
private:

	sf::Vector2f position;
	sf::Vector2f previousPosition;
	sf::Vector2f velocity;

	int frameCounter;

	float angle;
	float force;

	float gravity;

	sf::CircleShape hitBox;

};

