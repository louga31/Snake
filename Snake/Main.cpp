#include <iostream>
#include <SFML/Graphics.hpp>

#include "Game.h";

using namespace sf;

#define NUMBER_COLUMNS 30
#define NUMBER_LINES 20
#define CELL_SIZE 16

int main()
{
	const Game game(NUMBER_COLUMNS, NUMBER_LINES, CELL_SIZE);

	game.Run();
	
	return EXIT_SUCCESS;
}
