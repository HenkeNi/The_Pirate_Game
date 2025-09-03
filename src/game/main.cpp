#include "game/game.h"
#include <engine/core/engine.h>
#include <iostream>


// Temp?
//#include <engine/subsystems/subsystem_registry.h>

// TODO: fix precompiled header
// TODO; use snake_case for classes instead? and functions?

// rapidjson
// scene graph?

// Render batched tiles?


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