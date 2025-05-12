#include <SFML/Graphics.hpp>
#include "PerlinNoise.hpp"

#pragma once
class Terrain
{
public:
	Terrain(bool filled,int numOfPoints, int smoothness, sf::RenderWindow& window);

	void explosionEffect(sf::Vector2f explosionPos, int force);
	bool checkTerrainCollision(sf::Vector2f otherPosition);

	void display(sf::RenderWindow& window);
private:

	void UpdateEdgePoints();

	bool filled;

	std::vector<sf::Vertex> edgePoints;
	sf::VertexArray movedPoints;
	std::vector<sf::CircleShape> exploPoints;

	int numOfPoints;
	sf::VertexArray points;
	float smoothness;

	siv::PerlinNoise::seed_type pnSeed;
	siv::PerlinNoise noise;
};

