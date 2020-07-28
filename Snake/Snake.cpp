#include "Snake.h"

#include <SFML/Graphics.hpp>

#include "Direction.h"
#include "FoodGenerator.h"

using namespace sf;

Snake::Snake(RenderWindow& window, const unsigned int cellSize, FoodGenerator& foodGen) : m_window(window), m_cellSize(cellSize), m_direction(Direction::right),
														 m_timeToUpdate(100), m_foodGenerator(foodGen)
{
	AddCase();
	AddCase();
	m_foodGenerator.GenerateFruit(m_snakes);
}

void Snake::Move()
{
	// If enough time between move passed
	if (m_clock.getElapsedTime().asMilliseconds() >= m_timeToUpdate)
	{
		m_clock.restart();

		// Move all snake cells to previous one position, except for the first cell
		if (m_snakes.size() > 1)
		{
			for (unsigned int i = m_snakes.size() - 1; i > 0; i--)
			{
				m_snakes[i].setPosition(Vector2f(m_snakes[i - 1].getPosition().x, m_snakes[i - 1].getPosition().y));
			}
		}

		// Set first cell position based on movement direction
		m_snakes[0].move(static_cast<float>(m_direction.x * static_cast<int>(m_cellSize)), static_cast<float>(m_direction.y * static_cast<int>(m_cellSize)));

		CheckCollision();
	}
}

void Snake::Update(Event& event)
{
	if (event.type == Event::KeyPressed)
	{
		if (event.key.code == Keyboard::Left)
		{
			if (m_snakes[0].getPosition().x - m_snakes[0].getSize().x != m_snakes[1].getPosition().x)
			{
				if (m_direction != Direction::left && m_direction != Direction::right)
				{
					m_direction = Direction::left;
				}
			}
		}
		if (event.key.code == Keyboard::Right)
		{
			if (m_snakes[0].getPosition().x + m_snakes[0].getSize().x != m_snakes[1].getPosition().x)
			{
				if (m_direction != Direction::right && m_direction != Direction::left)
				{
					m_direction = Direction::right;
				}
			}
		}

		if (event.key.code == Keyboard::Up)
		{
			if (m_snakes[0].getPosition().y - m_snakes[0].getSize().x != m_snakes[1].getPosition().y)
			{
				if (m_direction != Direction::up && m_direction != Direction::down)
				{
					m_direction = Direction::up;
				}
			}
		}

		if (event.key.code == Keyboard::Down)
		{
			if (m_snakes[0].getPosition().y + m_snakes[0].getSize().x != m_snakes[1].getPosition().y)
			{
				if (m_direction != Direction::down && m_direction != Direction::up)
				{
					m_direction = Direction::down;
				}
			}
		}
	}
}

void Snake::Draw() const
{
	for (const auto& m_snakes : m_snakes)
		m_window.draw(m_snakes);
}

void Snake::CheckCollision()
{
	 // Own collision
	
	for (unsigned int i = 2; i < m_snakes.size(); i++)
	{
		if (m_snakes[0].getPosition().x == m_snakes[i].getPosition().x
			&& m_snakes[0].getPosition().y == m_snakes[i].getPosition().y)
		{
			m_window.close();
		}
	}

	// Out of screen collisions
	
	if (m_snakes[0].getPosition().x < 0) // Snake got out on the left
	{
		m_snakes[0].setPosition(static_cast<float>(m_window.getSize().x - m_cellSize), m_snakes[0].getPosition().y);
	}

	if (m_snakes[0].getPosition().x + m_snakes[0].getSize().x > m_window.getSize().x) // Snake got out on the right
	{
		m_snakes[0].setPosition(0.0f, m_snakes[0].getPosition().y);
	}
	
	if (m_snakes[0].getPosition().y < 0) // Snake got out on the top
	{
		m_snakes[0].setPosition(m_snakes[0].getPosition().x, static_cast<float>(m_window.getSize().y - m_cellSize));
	}

	if (m_snakes[0].getPosition().y + m_snakes[0].getSize().y > m_window.getSize().y) // Snake got out on the bottom
	{
		m_snakes[0].setPosition(m_snakes[0].getPosition().x, 0.0f);
	}

	// Food Collision

	if (m_snakes[0].getPosition().x == m_foodGenerator.m_fruit.getPosition().x
		&& m_snakes[0].getPosition().y == m_foodGenerator.m_fruit.getPosition().y)
	{
		AddCase();
		m_foodGenerator.GenerateFruit(m_snakes);
	}
}

void Snake::AddCase() // Add 1 more case
{
	RectangleShape cell;
	cell.setSize(Vector2f(m_cellSize, m_cellSize));
	cell.setOutlineColor(Color::Black);
	cell.setOutlineThickness(1.0f);

	// First cell is yellow and spawn at the middle of the screen
	if (m_snakes.empty())
	{
		cell.setFillColor(Color::Yellow);
		cell.setPosition(m_window.getSize().x / 2.0f, m_window.getSize().y / 2.0f);
	}
	else
	{
		cell.setFillColor(Color::Green);
		cell.setPosition(m_snakes[m_snakes.size() - 1].getPosition().x - cell.getSize().x, m_snakes[m_snakes.size() - 1].getPosition().y);
	}

	m_snakes.push_back(cell);
}
