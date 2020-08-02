// ReSharper disable CppClangTidyCppcoreguidelinesMacroUsage
#include "stdafx.h"

#include "Game.h"

using namespace sf;

#define NUMBER_COLUMNS 30
#define NUMBER_LINES 30
#define CELL_SIZE 32

int main()
{
	std::random_device dev;
	srand(static_cast<unsigned>(dev()));
	
	const Game game(NUMBER_COLUMNS, NUMBER_LINES, CELL_SIZE);

	game.Run();
	
	return EXIT_SUCCESS;
}
