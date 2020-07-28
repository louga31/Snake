#pragma once

#include <SFML/Graphics.hpp>

#include "Snake.h"

using namespace sf;

class Game
{
public:
	Game(unsigned int numberOfColumns, unsigned int numberOfLines, unsigned int cellSize);

	void Run() const;

private:
	unsigned int m_cellSize;
	unsigned int m_screenWidth;
	unsigned int m_screenHeight;
};
