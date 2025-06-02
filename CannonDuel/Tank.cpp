#include <iostream>

#include "Utils.h"
#include "Tank.h"


Tank::Tank(sf::Vector2f spawnPosition, sf::Keyboard::Key key, sf::Vector2f screenBounds) :
	position(spawnPosition),
	size(100, 50),
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
	rocket(nullptr),
	powerShow(sf::Quads, 4),
	lives(3),
	screenBounds(screenBounds)
	{
		this->hitBox.setPosition(this->position);
		this->hitBox.setSize(sf::Vector2f(100, 50));
		this->hitBox.setOrigin(50, 50);
		this->hitBox.setFillColor(sf::Color(50, 50, 200));
		
		this->shootBar.setPosition(this->position);
		this->shootBar.setSize(sf::Vector2f(100, 20));
		this->shootBar.setOrigin(0, 10);
		this->shootBar.setFillColor(sf::Color(50, 50, 255));

		this->cooldownBar.setSize(sf::Vector2f(100.f, 10.f));
		this->cooldownBar.setOrigin(0, 5);
		sf::Vector2f newBarPos(this->position.x - 50.f, this->position.y + 15);
		this->cooldownBar.setPosition(newBarPos);
		this->cooldownBar.setFillColor(sf::Color(200, 200, 200));

		this->setPowerShowPosition();

		this->powerIndicator.setSize(sf::Vector2f(2.f, 10.f));
		this->powerIndicator.setPosition(this->position.x - 50.f, this->position.y);
		this->powerIndicator.setFillColor(sf::Color::White);
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

			this->rocket = new Rocket(angle, this->shootForce, spawnPosition, this->screenBounds);
			
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


bool Tank::tankCollision(Tank& tk) {
	if (this->rocket == nullptr) {
		return false;
	}
	if ((*this->rocket).tankCollision(tk)) {
		tk.reduceLife();
		return true;
	}

	return false;
}

void Tank::reduceLife() {
	if (this->lives > 0) {
		this->lives--;
	}
	
}

int Tank::getLives() {
	return this->lives; 
}

void Tank::display(sf::RenderWindow& window) {
	this->hitBox.setPosition(this->position);
	this->shootBar.setPosition(this->position);
	sf::Vector2f newBarPos(this->position.x - 50.f, this->position.y + 15);
	this->cooldownBar.setPosition(newBarPos);
	this->setPowerShowPosition();

	float xPowerIndicator = Utils::map(this->shootForce, 0.f, 20.f, -50.f, 50.f);
	this->powerIndicator.setPosition(this->position.x + xPowerIndicator, this->position.y);

	window.draw(this->hitBox);
	window.draw(this->shootBar);
	window.draw(this->cooldownBar);
	window.draw(this->powerShow);
	window.draw(this->powerIndicator);
	this->showLives(window);
	
	//sf::CircleShape c(5);
	//c.setFillColor(sf::Color::Green);
	//c.setPosition(this->position);
	//c.setOrigin(5, 5);
	//window.draw(c);

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

	this->cooldownTimer = Utils::constrain(this->cooldownTimer, 0.f, 1.f);

	float xScaleMap = Utils::map(this->cooldownTimer, 0.f, this->cooldownDuration, 100.f, 0.f);
	this->cooldownBar.setSize(sf::Vector2f(xScaleMap, 5.f));

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

	if ((*this->rocket).toRemove) {
		this->resetRocket();
		return;
	}

	(*this->rocket).update();

}

void Tank::setPowerShowPosition(){
	//BAR POWER LEVEL SHOWER 
	// Top-left
	this->powerShow[0].position = sf::Vector2f(this->position.x - 50.f +1, this->position.y);
	this->powerShow[0].color = sf::Color::Red;

	// Top-right
	this->powerShow[1].position = sf::Vector2f(this->position.x + 50.f, this->position.y);
	this->powerShow[1].color = sf::Color::Green;

	// Bottom-right
	this->powerShow[2].position = sf::Vector2f(this->position.x + 50.f, this->position.y + 10.f);
	this->powerShow[2].color = sf::Color::Green;

	// Bottom-left
	this->powerShow[3].position = sf::Vector2f(this->position.x - 50.f +1, this->position.y + 10.f);
	this->powerShow[3].color = sf::Color::Red;
}

void Tank::showLives(sf::RenderWindow& window) {
	sf::CircleShape life(10.f);

	float incr = this->size.x / 3.f;

	for (int i = 0; i < 3; i++) {
		if (i < lives) {
			life.setFillColor(sf::Color::Green);
		}else{
			life.setFillColor(sf::Color(50,50,50));
		}
		
		life.setPosition(this->position.x - 50.f + 7.5f + i * incr, this->position.y - 25.f);
		window.draw(life);

	}
}