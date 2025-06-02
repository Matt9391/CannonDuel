#include <cmath>
#include "Rocket.h"
#include "Utils.h"
#include "Tank.h"
#include <iostream>

Rocket::Rocket(float angle, float force, sf::Vector2f pos, sf::Vector2f screenBounds) :
	angle(angle),
	force(force),
	position(pos),
	gravity(9.81),
	frameCounter(0),
	toRemove(false),
	screenBounds(screenBounds)
	{
	
		this->velocity = sf::Vector2f(cos(this->angle) * this->force, sin(this->angle) * this->force);
		

		this->hitBox.setFillColor(sf::Color::Magenta);
		this->hitBox.setRadius(15);
		this->hitBox.setOrigin(15, 15);
	}

void Rocket::update() {
	frameCounter++;

	this->velocity.y += this->gravity * Utils::dt;
	//this->velocity.x *= 0.99f;
	if (frameCounter % 5 == 0) {
		this->previousPosition = position;
	}

	if (this->position.x - this->hitBox.getRadius() < 0 || this->position.x + this->hitBox.getRadius() > this->screenBounds.x) {
		this->velocity.x *= -1;
	}
	if (this->position.y > this->screenBounds.y) {
		std::cout << this->screenBounds.y << std::endl;
		this->toRemove = true;
	}

	this->position += this->velocity;

}



sf::Vector2f Rocket::getPosition() {
	return this->position;
}

sf::Vector2f Rocket::getPreviousPosition() {
	return this->previousPosition;
}

bool Rocket::tankCollision(const Tank& tk) {
	//std::cout << "tank : " << Utils::distance(tk.position, this->position) << std::endl;
	if (Utils::distance(tk.position, this->position) < tk.size.x / 2.f) {
		this->toRemove = true;
		std::cout << "skibidi toilet" << std::endl;
		return true;
	}

	return false;
}


void Rocket::display(sf::RenderWindow& window) {
	this->hitBox.setPosition(this->position);

	window.draw(this->hitBox);
}

