#include "stdafx.h"

#include "Game.h"

#include "Snake.h"

#include "Hamiltonian/HamiltonianCycle.h"


using namespace sf;

Game::Game(const unsigned numberOfColumns, const unsigned numberOfLines, const unsigned cellSize) : m_cellSize(cellSize), m_screenWidth(cellSize* numberOfColumns), m_screenHeight(cellSize* numberOfLines)
{
	
}

void Game::Run() const
{
	RenderWindow window(VideoMode(m_screenWidth, m_screenHeight), "Snake !!!!!");

	window.setVerticalSyncEnabled(false);
	window.setFramerateLimit(0);
	window.setKeyRepeatEnabled(false);

	HamiltonianCycle hamCycle(window, m_cellSize, m_screenWidth / m_cellSize, m_screenHeight / m_cellSize);
	
	FoodGenerator foodGen(window, m_cellSize);

	Snake snake(window, hamCycle, m_cellSize, foodGen);
	
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
		hamCycle.Draw();
		
		window.display();
	}
}
