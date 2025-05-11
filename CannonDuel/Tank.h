#include <SFML/Graphics.hpp>
#include "Rocket.h"

#pragma once
class Tank
{
public:
	Tank(sf::Vector2f spawnPosition, sf::Keyboard::Key key);

	void enableGravity();
	void disableGravity();
	sf::Vector2f getRocketPositon();
	sf::Vector2f getRocketPreviousPositon();
	void resetRocket();

	void update();
	void display(sf::RenderWindow& window);
	
	sf::Vector2f position;
private:

	void move();
	void shootCooldown();
	void rotateShootBar();
	void handleRocket();

	sf::Vector2f velocity;

	float gravity;
	bool gravityEnable;
	bool onGround;

	bool canShoot;
	bool chargingShoot;
	float cooldownTimer;
	float cooldownDuration;

	float shootForce;
	float angleShootBar;
	int swingDir;

	sf::CircleShape cooldownBar;

	sf::RectangleShape hitBox;
	sf::RectangleShape shootBar;

	sf::Keyboard::Key keyToShoot;

	Rocket* rocket;
};

