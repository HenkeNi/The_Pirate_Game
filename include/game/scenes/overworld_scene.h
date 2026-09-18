#pragma once
#include "game/scenes/scene.h"
#include "game/map/tilemap.h"
#include "game/map/map_generator.h"
#include <engine/physics/physics.h>

namespace ce = cursed_engine;

class OverworldScene : public Scene
{
public:
	OverworldScene(SceneContext context, ce::PhysicsAPI physicsAPI); // how to construct scene if need args? facotry?
	void onUpdate(float deltaTime) override;

	void onEnter();
	void onExit();

private:
	Tilemap m_tilemap; // or pointer? mapgenerator returns map?
	MapGenerator m_mapGenerator; // put in GameScene? (base)
	
	ce::PhysicsWorld m_physicsWorld;
	ce::PhysicsAPI m_physicsAPI;
};