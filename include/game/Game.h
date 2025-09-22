#pragma once
#include <engine/core/application.h>

using cursed_engine::EngineSystems;

class Game : public cursed_engine::Application
{
public:
	Game() = default;

	void onUpdate(float deltaTime) override;
	void onCreated(const EngineSystems& systems) override;
};