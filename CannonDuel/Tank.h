#include <SFML/Graphics.hpp>
#include "Rocket.h"	

#pragma once
class Tank
{
public:
	Tank(sf::Vector2f spawnPosition, sf::Keyboard::Key key, sf::Vector2f screenBounds);

	void enableGravity();
	void disableGravity();
	sf::Vector2f getRocketPositon();
	sf::Vector2f getRocketPreviousPositon();
	void resetRocket();
	bool tankCollision(Tank& tk);
	int getLives();

	void update();
	void display(sf::RenderWindow& window);
	
	sf::Vector2f position;
	sf::Vector2f size;
private:

	void move();
	void shootCooldown();
	void rotateShootBar();
	void handleRocket();
	void setPowerShowPosition();
	void showLives(sf::RenderWindow& window);
	void reduceLife();

	sf::Vector2f velocity;

	sf::Vector2f screenBounds;

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


	sf::RectangleShape hitBox;
	sf::RectangleShape shootBar;

	sf::Keyboard::Key keyToShoot;

	Rocket* rocket;

	int lives;

	//GFX
	sf::RectangleShape cooldownBar;
	sf::VertexArray powerShow;
	sf::RectangleShape powerIndicator;


};

