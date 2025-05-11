#include <iostream>

#include "Utils.h"
#include "Tank.h"


Tank::Tank(sf::Vector2f spawnPosition, sf::Keyboard::Key key) :
	position(spawnPosition),
	velocity(sf::Vector2f(0, 0)),
	gravity(9.81f),
	gravityEnable(true),
	keyToShoot(key),
	onGround(false),
	canShoot(true),
	chargingShoot(false),
	cooldownTimer(0.f),
	cooldownDuration(1.f),
	shootForce(0.f),
	angleShootBar(0.f),
	swingDir(-1),
	rocket(nullptr)
	{
		this->hitBox.setPosition(this->position);
		this->hitBox.setSize(sf::Vector2f(100, 50));
		this->hitBox.setOrigin(50, 50);
		this->hitBox.setFillColor(sf::Color(50, 50, 200));
		
		this->shootBar.setPosition(this->position);
		this->shootBar.setSize(sf::Vector2f(100, 20));
		this->shootBar.setOrigin(0, 10);
		this->shootBar.setFillColor(sf::Color(50, 50, 255));

		this->cooldownBar.setRadius(1);
		this->cooldownBar.setPointCount(100);
		this->cooldownBar.setPosition(this->position);
		this->cooldownBar.setScale(50.f, 5.f);
		this->cooldownBar.setFillColor(sf::Color(100, 100, 100));
		this->cooldownBar.setOrigin(1, 1);
	}

void Tank::update() {
	this->move();

	this->shootCooldown();
	
	if (!this->chargingShoot) {
		this->rotateShootBar();
	}


	if (sf::Keyboard::isKeyPressed(this->keyToShoot) && this->onGround) {
		
		if (this->canShoot && !this->rocket) {
			//std::cout << "SHOOTTT"<<std::endl;
			this->chargingShoot = true;
		}
	}

	if (this->chargingShoot) {
		if (!sf::Keyboard::isKeyPressed(this->keyToShoot)) {
			this->chargingShoot = false;
			this->cooldownTimer = 0.f;

			float angle = Utils::degreesToRadians(this->angleShootBar);
			sf::Vector2f spawnPosition( this->position.x + this->shootBar.getSize().x * cos(angle),
										this->position.y + this->shootBar.getSize().x * sin(angle)
									  );

			this->rocket = new Rocket(angle, this->shootForce, spawnPosition);
			
			this->shootForce = 0.f;
		}

		this->shootForce += 10 * Utils::dt;
		this->shootForce = Utils::constrain(this->shootForce, 0.f, 20.f);

		std::cout << this->shootForce << std::endl;
		
	}

	this->handleRocket();
}

void Tank::enableGravity() {
	this->gravityEnable = true;
}

void Tank::disableGravity() {
	this->gravityEnable = false;
}

sf::Vector2f Tank::getRocketPositon() {
	if (this->rocket) {
		return (*this->rocket).getPosition();
	}
	else {
		return sf::Vector2f(0, 0);
	}
}
sf::Vector2f Tank::getRocketPreviousPositon() {
	if (this->rocket) {
		return (*this->rocket).getPreviousPosition();
	}
	else {
		return sf::Vector2f(0, 0);
	}
}

void Tank::resetRocket() {
	this->rocket = nullptr;
}

void Tank::display(sf::RenderWindow& window) {
	this->hitBox.setPosition(this->position);
	this->shootBar.setPosition(this->position);
	this->cooldownBar.setPosition(this->position);

	window.draw(this->hitBox);
	window.draw(this->shootBar);
	window.draw(this->cooldownBar);
	
	sf::CircleShape c(5);
	c.setFillColor(sf::Color::Green);
	c.setPosition(this->position);
	c.setOrigin(5, 5);
	window.draw(c);

	if (this->rocket) {
		(*this->rocket).display(window);
	}
}

//PRIVATE METHODS

void Tank::move() {
	if (this->gravityEnable) {
		this->velocity.y += this->gravity * Utils::dt;
	}
	else {
		this->onGround = true;
		this->velocity.y = 0;
	}

	this->position += this->velocity;
}

void Tank::shootCooldown() {
	float xScaleMap = Utils::map(this->cooldownTimer, 0.f, this->cooldownDuration, 0.f, 50.f);
	this->cooldownBar.setScale(xScaleMap, 5.f);

	if (this->cooldownTimer >= this->cooldownDuration) {
		this->canShoot = true;
	}
	else {
		this->canShoot = false;
		this->cooldownTimer += Utils::dt;
	}
}

void Tank::rotateShootBar() {
	if (this->angleShootBar >= 0 || this->angleShootBar <= -180) {
		this->swingDir *= -1;
	}

	this->angleShootBar += 50 * Utils::dt * this->swingDir;
	this->angleShootBar = Utils::constrain(this->angleShootBar, -180.f, 0.f);
	this->shootBar.setRotation(this->angleShootBar);

}

void Tank::handleRocket() {
	if (this->rocket == nullptr) {
		return;
	}

	(*this->rocket).update();

}