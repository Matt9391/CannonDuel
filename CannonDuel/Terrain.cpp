#include "Terrain.h"
#include "Utils.h"
#include <iostream>

Terrain::Terrain(bool filled, int numOfPoints, int smoothness, sf::RenderWindow& window) :
	points(filled ? sf::TriangleStrip : sf::LinesStrip),
	movedPoints(sf::Lines),
	numOfPoints(Utils::constrain(numOfPoints + 1, 41,801)),
	pnSeed(Utils::randomUInt(0, 1000000)),
	noise(pnSeed),
	smoothness(Utils::constrain(smoothness,0,10)),
	filled(filled)
	{
		this->edgePoints.clear();	

		float incr = static_cast<float>(window.getSize().x) / this->numOfPoints;
		
		//Noise initial value
		float n = Utils::randomFloat(0.f, 1000.f);
		float groundY = window.getSize().y * 0.8;

		float minSmth = Utils::map(this->numOfPoints, 40, 400, 0.25f, 0.02f);
		float maxSmth = Utils::map(this->numOfPoints, 40, 400, 0.05f, 0.008f);

		this->smoothness = Utils::map(this->smoothness, 0.f, 10.f, minSmth, maxSmth);
		
		sf::Color clr = this->filled ? sf::Color(190, 100, 0) : sf::Color::Red;

		for (float i = -1; i < this->numOfPoints; i++) {
			float x = i * incr;

			float yTop = groundY + noise.noise1D(n) * 100.f;
			float yBottom = window.getSize().y + 100.f;
			
			sf::Vertex pointTop(sf::Vector2f(x, yTop), clr);
			points.append(pointTop);
			
			if (this->filled) {
				sf::Vertex pointBottom(sf::Vector2f(x, yBottom), clr);
				points.append(pointBottom);
			}

			n += this->smoothness;
		}
		
		points.append(sf::Vertex(sf::Vector2f(window.getSize().x, window.getSize().y + 100.f), clr));
		points.append(sf::Vertex(sf::Vector2f(0.f, window.getSize().y + 100.f), clr));

		this->UpdateEdgePoints();
	}

void Terrain::explosionEffect(sf::Vector2f explosionPos, int force) {
	std::cout << "NEW  EXPLO" << std::endl;
	sf::CircleShape c(5);
	c.setPosition(explosionPos);
	c.setOrigin(5, 5);
	this->exploPoints.push_back(c);
	this->movedPoints.clear();
	for (int i = 0; i < this->points.getVertexCount(); i++) {
		force = Utils::map(Utils::distance(this->points[i].position, explosionPos), 0, 100, 80, 0);
		
		if (Utils::distance(this->points[i].position, explosionPos) < 100.f) {
			this->movedPoints.append(sf::Vertex(this->points[i].position, sf::Color::Green));
			float angle = atan2(this->points[i].position.y - explosionPos.y, this->points[i].position.x - explosionPos.x);
			//float angle = atan2(explosionPos.y - this->points[i].position.y, explosionPos.x - this->points[i].position.x);
			std::cout << Utils::radiansToDegrees(angle) << std::endl;
			
			float nextYpos = this->points[i].position.y + sin(angle) * force;
			
			this->points[i].position.x += cos(angle) * force;
		
			if (nextYpos > this->points[i].position.y) {
				this->points[i].position.y += sin(angle) * force;
			}

			this->movedPoints.append(sf::Vertex(sf::Vector2f(this->points[i].position.x, this->points[i].position.y), sf::Color::Magenta));
		
		}
	}
	this->UpdateEdgePoints();
}

//void Terrain::explosionEffect(sf::Vector2f explosionPos, int force) {
//	std::cout << "NEW  EXPLO" << std::endl;
//	sf::CircleShape c(5);
//	c.setPosition(explosionPos);
//	c.setOrigin(5, 5);
//	this->exploPoints.push_back(c);
//	this->movedPoints.clear();
//	//force = Utils::map(Utils::distance(this->points[i].position, explosionPos), 0, 100, 80, 0);
//	int nOfPointsToMove = 0;
//	for (int i = 0; i < this->points.getVertexCount(); i++) {
//
//		if (Utils::distance(this->points[i].position, explosionPos) < 100.f) {
//			
//			nOfPointsToMove++;
//		}
//	}
//
//	force = 80;
//	float angleIncr = 3.14f / nOfPointsToMove;
//	int pointsMoved = 0;
//	for (int i = 0; i < this->points.getVertexCount(); i++) {
//
//		if (Utils::distance(this->points[i].position, explosionPos) < 100.f) {
//			this->movedPoints.append(sf::Vertex(this->points[i].position, sf::Color::Green));
//			float angle = pointsMoved * angleIncr;
//
//			pointsMoved++;
//
//			//float angle = atan2(explosionPos.y - this->points[i].position.y, explosionPos.x - this->points[i].position.x);
//			//std::cout << Utils::radiansToDegrees(angle) << std::endl;
//
//			float nextYpos = explosionPos.y + sin(angle) * force;
//
//			this->points[i].position.x += cos(angle) * force;
//
//			if (nextYpos > this->points[i].position.y) {
//				this->points[i].position.y += sin(angle) * force;
//			}
//
//			this->movedPoints.append(sf::Vertex(sf::Vector2f(this->points[i].position.x, this->points[i].position.y), sf::Color::Magenta));
//
//		}
//	}
//
//	std::vector<sf::Vertex> sortedVertices;
//	for (std::size_t i = 0; i < points.getVertexCount(); ++i) {
//		sortedVertices.push_back(points[i]);
//	}
//
//	// Ordina (es: per ascendente X)
//	std::sort(sortedVertices.begin(), sortedVertices.end(), [](const sf::Vertex& a, const sf::Vertex& b) {
//		return a.position.x < b.position.x;
//		});
//
//	// Ricrea il VertexArray ordinato
//	points.clear();
//	for (const auto& v : sortedVertices) {
//		points.append(v);
//	}
//	this->UpdateEdgePoints();
//}

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
	
	if (this->filled) {
		for (int i = 0; i < this->points.getVertexCount(); i += 2) {
			this->edgePoints.push_back(this->points[i]);
		}
	}
	else {
		for (int i = 0; i < this->points.getVertexCount(); i++) {
			this->edgePoints.push_back(this->points[i]);
		}
	}
}



void Terrain::display(sf::RenderWindow& window) {
	window.draw(this->points);

	for (int i = 0; i < this->points.getVertexCount(); i++) {
		sf::CircleShape c(2);
		c.setFillColor(sf::Color::Green);
		c.setPosition(this->points[i].position);
		c.setOrigin(2,2);
		window.draw(c);
	}

	for (int i = 0; i < this->exploPoints.size(); i++)
	{
		window.draw(this->exploPoints[i]);
	}
	
	window.draw(this->movedPoints);
	
	//sf::ConvexShape road;
	//road.setPointCount(points.getVertexCount());
	//for (size_t i = 0; i < points.getVertexCount(); ++i) {
	//	road.setPoint(i, points[i].position);
	//}

	//window.draw(road);
}