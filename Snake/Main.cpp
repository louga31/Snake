// ReSharper disable CppClangTidyCppcoreguidelinesMacroUsage
#include <iostream>
#include <random>


#include "Game.h"

using namespace sf;

#define NUMBER_COLUMNS 40
#define NUMBER_LINES 20
#define CELL_SIZE 16

int main()
{
	std::random_device dev;
	srand(static_cast<unsigned>(dev()));
	
	const Game game(NUMBER_COLUMNS, NUMBER_LINES, CELL_SIZE);

	game.Run();
	
	return EXIT_SUCCESS;
}
