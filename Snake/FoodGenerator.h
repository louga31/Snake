#pragma once

#include <SFML/Graphics.hpp>

using namespace sf;

class FoodGenerator
{
public:
	FoodGenerator(RenderWindow& window, unsigned cellSize);

	RectangleShape m_fruit;

	void GenerateFruit(const std::vector<RectangleShape>& snakes);
	void Draw() const;

private:
	RenderWindow& m_window;
	unsigned m_cellSize;
};
