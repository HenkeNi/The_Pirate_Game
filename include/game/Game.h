#pragma once
#include <engine/core/application.h>

class Game : public cursed_engine::Application
{
public:
	Game() = default;

	void onCreated() override;

	//bool Initialize();
	//void Shutdown();

	//void Run();

};