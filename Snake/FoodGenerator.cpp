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

void FoodGenerator::GenerateFruit(std::vector<RectangleShape> snakes)
{
    const auto randomX = RandomInt(0, m_window.getSize().x / m_cellSize);
    const auto randomY = RandomInt(0, m_window.getSize().y / m_cellSize);

    for (unsigned i = 0; i < snakes.size(); i++)
    {
        if (snakes[0].getPosition().x != static_cast<float>(randomX * m_cellSize) && snakes[0].getPosition().y != randomY)
        {
            m_fruit.setPosition(randomX * m_cellSize, randomY * m_cellSize);
        }
    }
}

void FoodGenerator::Draw() const
{
    m_window.draw(m_fruit);
}