#include <cmath>
#include "Rocket.h"
#include "Utils.h"

Rocket::Rocket(float angle, float force, sf::Vector2f pos) :
	angle(angle),
	force(force),
	position(pos),
	gravity(9.81),
	frameCounter(0)
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

	this->position += this->velocity;

}

sf::Vector2f Rocket::getPosition() {
	return this->position;
}

sf::Vector2f Rocket::getPreviousPosition() {
	return this->previousPosition;
}

void Rocket::display(sf::RenderWindow& window) {
	this->hitBox.setPosition(this->position);

	window.draw(this->hitBox);
}