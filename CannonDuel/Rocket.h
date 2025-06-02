#include "SFML/Graphics.hpp"

class Tank;

#pragma once
class Rocket
{
public:
	Rocket(float angle, float force, sf::Vector2f pos, sf::Vector2f screenBounds);

	void update();
	void display(sf::RenderWindow& window);

	sf::Vector2f getPosition();
	sf::Vector2f getPreviousPosition();

	bool tankCollision(const Tank& tk);

	bool toRemove;
private:

	sf::Vector2f position;
	sf::Vector2f previousPosition;
	sf::Vector2f velocity;
	sf::Vector2f screenBounds;

	int frameCounter;

	float angle;
	float force;

	float gravity;

	sf::CircleShape hitBox;

};

