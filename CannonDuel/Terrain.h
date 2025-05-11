#include <SFML/Graphics.hpp>
#include "PerlinNoise.hpp"

#pragma once
class Terrain
{
public:
	Terrain(int numOfPoints, int smoothness, sf::RenderWindow& window);

	void explosionEffect(sf::Vector2f explosionPos, int force);
	bool checkTerrainCollision(sf::Vector2f otherPosition);

	void display(sf::RenderWindow& window);
private:

	void UpdateEdgePoints();


	std::vector<sf::Vertex> edgePoints;

	int numOfPoints;
	sf::VertexArray points;
	float smoothness;

	siv::PerlinNoise::seed_type pnSeed;
	siv::PerlinNoise noise;
};

