#include "Game.h"

#include <SFML/Graphics.hpp>

#include "Snake.h"
#include "FoodGenerator.h"

using namespace sf;

Game::Game(const unsigned int numberOfColumns, const unsigned int numberOfLines, const unsigned int cellSize) : m_cellSize(cellSize), m_screenWidth(cellSize* numberOfColumns), m_screenHeight(cellSize* numberOfLines)
{
	
}

void Game::Run() const
{
	RenderWindow window(VideoMode(m_screenWidth, m_screenHeight), "Snake !!!!!");

	window.setFramerateLimit(120);
	window.setKeyRepeatEnabled(false);

	FoodGenerator foodGen(window, m_cellSize);

	Snake snake(window, m_cellSize, foodGen);
	
	while (window.isOpen())
	{
		snake.Move();
		
		Event event{};
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
				case Event::Closed:
					window.close();

				case Event::KeyReleased:
				{
					if (event.key.code == Keyboard::Escape)
					{
						window.close();
					}
				}
				default: break;
			}
			
			snake.Update(event);
		}

		/// Draw ///

		window.clear();

		snake.Draw();
		foodGen.Draw();
		
		window.display();
	}
}
