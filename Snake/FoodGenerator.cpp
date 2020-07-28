#include "FoodGenerator.h"

#include <cstdlib>
#include <random>

#include <SFML/Graphics.hpp>

using namespace sf;

FoodGenerator::FoodGenerator(RenderWindow& window, const unsigned int cellSize) : m_window(window), m_cellSize(cellSize)
{
	std::random_device dev;
	srand(static_cast<unsigned int>(dev()));

    m_fruit.setSize(Vector2f(m_cellSize, m_cellSize));
    m_fruit.setOutlineColor(Color::Black);
    m_fruit.setOutlineThickness(1.0f);
    m_fruit.setFillColor(sf::Color::Red);
}

void FoodGenerator::GenerateFruit(std::vector<RectangleShape> snakes)
{
	const int randomX = rand() % (m_window.getSize().x / m_cellSize);
	const int randomY = rand() % (m_window.getSize().y / m_cellSize);

    for (unsigned int i = 0; i < snakes.size(); i++)
    {
        if ((snakes[0].getPosition().x != static_cast<float>(randomX * m_cellSize)) && (snakes[0].getPosition().y != randomY))
        {
            m_fruit.setPosition(randomX * m_cellSize, randomY * m_cellSize);
        }
    }
}

void FoodGenerator::Draw() const
{
    m_window.draw(m_fruit);
}