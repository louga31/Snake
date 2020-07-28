#pragma once

#include <SFML/Graphics.hpp>

using namespace sf;

class FoodGenerator
{
public:
	FoodGenerator(RenderWindow& window, unsigned int cellSize);

	RectangleShape m_fruit;

	void GenerateFruit(std::vector<RectangleShape> snakes);
	void Draw() const;

private:
	RenderWindow& m_window;
	unsigned int m_cellSize;
};
