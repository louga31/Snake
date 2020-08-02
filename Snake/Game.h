#pragma once

using namespace sf;

class Game
{
public:
	Game(unsigned numberOfColumns, unsigned numberOfLines, unsigned cellSize);

	void Run() const;

private:
	unsigned m_cellSize;
	unsigned m_screenWidth;
	unsigned m_screenHeight;
};
