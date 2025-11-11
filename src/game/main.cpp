#include "game/game.h"
#include <engine/core/engine.h>
#include <iostream>

// TODO: fix precompiled header
// TODO; naming convention: use snake_case for classes instead? and functions? lowercase for struct members?
// put scenes in a graph? should ui be its own scene? (easy to pop if not wanting it)
// Render batched tiles?
// Layer stack? 

int main()
{
	Game game;
	cursed_engine::Engine engine{ game };

	if (!engine.init())
	{
		std::cerr << "Failed to initialize game!";
		return 1;
	}

	engine.run();
	engine.shutdown();

	return 0;
}