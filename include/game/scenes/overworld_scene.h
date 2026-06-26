#pragma once
#include "game/scenes/scene.h"
#include "game/map/tile_map.h"
#include "game/map/map_generator.h"

class OverworldScene : public Scene
{
public:
	OverworldScene(SceneContext context);
	void onUpdate(float deltaTime) override;

	void onEnter();
	void onExit();

private:
	TileMap m_tileMap; // or pointer? mapgenerator returns map?
	MapGenerator m_mapGenerator; // put in GameScene? (base)
};