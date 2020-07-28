#pragma once

#include <SFML/Graphics.hpp>

#include "FoodGenerator.h"

using namespace sf;

class Snake
{
public:
	Snake(RenderWindow& window, unsigned int cellSize, FoodGenerator& foodGen);

	void Move();
	void Update(Event& event);
	void Draw() const;
	
private:
	RenderWindow& m_window;
	unsigned int m_cellSize;
	Vector2i m_direction;
	Clock m_clock;
	int m_timeToUpdate;
	std::vector<RectangleShape> m_snakes;
	FoodGenerator& m_foodGenerator;

	void CheckCollision();
	void AddCase();
};
