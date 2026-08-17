#pragma once
#include <string>

// here or in engine?
struct NewGameEvent
{

};

struct MapChunkCreatedEvent
{
	int x;
	int y;
};


struct SceneTransitionEvent
{
	std::string scene;
	std::string transition; // replace with enum? replace, pop and push...
};