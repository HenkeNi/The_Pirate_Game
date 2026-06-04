#pragma once
#include <string>

// here or in engine?
struct NewGameEvent
{

};

struct SceneTransitionEvent
{
	std::string scene;
	std::string transition; // replace with enum? replace, pop and push...
};