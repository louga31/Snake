#include "FoodGenerator.h"

#include <SFML/Graphics.hpp>

#include "Utils.h"

using namespace sf;

FoodGenerator::FoodGenerator(RenderWindow& window, const unsigned cellSize) : m_window(window), m_cellSize(cellSize)
{
	m_fruit.setSize(Vector2f(m_cellSize, m_cellSize));
	m_fruit.setOutlineColor(Color::Black);
	m_fruit.setOutlineThickness(1.0f);
	m_fruit.setFillColor(Color::Red);
}

void FoodGenerator::GenerateFruit(const std::vector<RectangleShape>& snakes)
{
	int randomX, randomY;
	
	auto canSpawn = true;
	do
	{
		randomX = RandomInt(0, m_window.getSize().x / m_cellSize - 1);
		randomY = RandomInt(0, m_window.getSize().y / m_cellSize - 1);

		canSpawn = true;
		for (const auto& cell : snakes)
		{
			if (cell.getPosition().x == static_cast<float>(randomX * m_cellSize) && cell.getPosition().y == static_cast<float>(randomY * m_cellSize))
			{
				canSpawn = false;
			}
		}
	}
	while (!canSpawn);
	
	m_fruit.setPosition(randomX * m_cellSize, randomY * m_cellSize);
}

void FoodGenerator::Draw() const
{
	m_window.draw(m_fruit);
}