#include "Terrain.h"
#include "Utils.h"
#include <iostream>

Terrain::Terrain(int numOfPoints, int smoothness, sf::RenderWindow& window) :
	points(sf::TriangleStrip),
	numOfPoints(Utils::constrain(numOfPoints + 1, 41,401)),
	pnSeed(Utils::randomUInt(0, 1000000)),
	noise(pnSeed),
	smoothness(Utils::constrain(smoothness,0,10))
	{
		this->edgePoints.clear();	

		float incr = static_cast<float>(window.getSize().x) / this->numOfPoints;
		
		//Noise initial value
		float n = Utils::randomFloat(0.f, 1000.f);
		float groundY = window.getSize().y * 0.8;

		float minSmth = Utils::map(this->numOfPoints, 40, 400, 0.25f, 0.02f);
		float maxSmth = Utils::map(this->numOfPoints, 40, 400, 0.05f, 0.008f);

		this->smoothness = Utils::map(this->smoothness, 0.f, 10.f, minSmth, maxSmth);
		
		
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

		this->UpdateEdgePoints();
	}

void Terrain::explosionEffect(sf::Vector2f explosionPos, int force) {
	for (int i = 0; i < this->points.getVertexCount(); i++) {
		force = Utils::map(Utils::distance(this->points[i].position, explosionPos), 0, 100, 100, 0);
		if (Utils::distance(this->points[i].position, explosionPos) < 100.f) {
			float angle = atan2(this->points[i].position.y - explosionPos.y, this->points[i].position.x - explosionPos.x);
			//float angle = atan2(explosionPos.y - this->points[i].position.y, explosionPos.x - this->points[i].position.x);
			float nextYpos = this->points[i].position.y + sin(angle) * force;
			this->points[i].position.x += cos(angle) * force;
			if (nextYpos > this->points[i].position.y) {
				this->points[i].position.y += sin(angle) * force;
			}
		}
	}
	this->UpdateEdgePoints();
}

bool Terrain::checkTerrainCollision(sf::Vector2f otherPosition) {
	int countCollision = 0;

	for (int i = 0; i < this->edgePoints.size() - 1; i++) {
		float x1 = this->edgePoints[i].position.x;
		float y1 = this->edgePoints[i].position.y;
		float x2 = this->edgePoints[i + 1].position.x;
		float y2 = this->edgePoints[i + 1].position.y;

		float inHeight = (otherPosition.y < y1) != (otherPosition.y < y2);
		if (inHeight && otherPosition.x < x1 + (((otherPosition.y - y1) / (y2 - y1)) * (x2 - x1))) {
			countCollision++;
		}
	}

	if (countCollision % 2 == 0) {
		return false;
	}
	else {
		return true;
	}
}

void Terrain::UpdateEdgePoints() {
	this->edgePoints.clear();
	
	for (int i = 0; i < this->points.getVertexCount(); i += 2) {
		this->edgePoints.push_back(this->points[i]);
	}
}



void Terrain::display(sf::RenderWindow& window) {
	window.draw(this->points);

	for (int i = 0; i < this->points.getVertexCount(); i++) {
		sf::CircleShape c(5);
		c.setFillColor(sf::Color::Red);
		c.setPosition(this->points[i].position);
		c.setOrigin(5,5);
		window.draw(c);
	}
	
	//sf::ConvexShape road;
	//road.setPointCount(points.getVertexCount());
	//for (size_t i = 0; i < points.getVertexCount(); ++i) {
	//	road.setPoint(i, points[i].position);
	//}

	//window.draw(road);
}