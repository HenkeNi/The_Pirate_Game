#pragma once
#include <vector>

// PlayerProgression?
using PlayerId = uint32_t;

struct PlayerState
{
	PlayerId id;

	int level;
	int maxHealth;
	int coins;

	// inventory...
};

// or world data?
struct WorldState 
{
	int seed;

	// defated boses?
	// cleared dungeons?
	// completed quests
};

struct GameSession
{
	WorldState worldState;
	std::vector<PlayerState> players;
};