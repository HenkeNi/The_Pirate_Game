#pragma once
#include <string>
#include "game/scenes/scene_types.h"

// here or in engine?
struct NewGameEvent
{

};

struct MapChunkCreatedEvent
{
	int x;
	int y;
};


struct SceneTransitionRequestEvent
{
	std::string scene;
	SceneTransitionType transitionTyp;
};

struct SceneTransitionEvent
{
	std::string scene;
	//std::string transition; // replace with enum? replace, pop and push...
};