#include "Terrain.h"
#include "Utils.h"
#include <iostream>

Terrain::Terrain(int numOfPoints, int smoothness, sf::RenderWindow& window) :
	points(sf::TriangleStrip),
	numOfPoints(numOfPoints + 1),
	pnSeed(Utils::randomUInt(0, 1000000)),
	noise(pnSeed),
	smoothness(Utils::map(smoothness, 0.f, 10.f, 0.2f, 0.05f))
	{
		float incr = static_cast<float>(window.getSize().x) / numOfPoints;
		//noise.noise1D(this->noiseValues.x);
		float n = Utils::randomFloat(0.f, 1000.f);
		float groundY = window.getSize().y * 0.8;

		this->smoothness /= incr / 2;
		for (float i = -1; i < this->numOfPoints; i++) {
			float x = i * incr;

			float yTop = groundY + noise.noise1D(n) * 100.f;
			float yBottom = window.getSize().y + 100.f;

			sf::Vertex pointTop(sf::Vector2f(x, yTop), sf::Color(190, 100, 0));
			sf::Vertex pointBottom(sf::Vector2f(x, yBottom), sf::Color(190, 100, 0));
			n += this->smoothness;
			points.append(pointTop);
			points.append(pointBottom);
		}
		
		points.append(sf::Vertex(sf::Vector2f(window.getSize().x, window.getSize().y + 100.f), sf::Color(150,100,0)));
		points.append(sf::Vertex(sf::Vector2f(0.f, window.getSize().y + 100.f), sf::Color(150, 100, 0)));

	}

void Terrain::explosionEffect(sf::Vector2f& explosionPos, int force) {
	for (int i = 0; i < this->points.getVertexCount(); i++) {
		force = Utils::map(Utils::distance(this->points[i].position, explosionPos), 0, 100, 100, 0);
		if (Utils::distance(this->points[i].position, explosionPos) < 100.f) {
			float angle = atan2(this->points[i].position.y - explosionPos.y, this->points[i].position.x - explosionPos.x);
			//float angle = atan2(explosionPos.y - this->points[i].position.y, explosionPos.x - this->points[i].position.x);
			this->points[i].position.x += cos(angle) * force;
			this->points[i].position.y += sin(angle) * force;
		}
	}
}

void Terrain::display(sf::RenderWindow& window) {
	for (int i = 0; i < this->points.getVertexCount(); i++) {
		sf::CircleShape c(5);
		c.setFillColor(sf::Color::Red);
		c.setPosition(this->points[i].position);
		window.draw(c);
	}

	window.draw(this->points);
}