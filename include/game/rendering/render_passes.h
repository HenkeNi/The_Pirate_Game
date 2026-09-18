#pragma once
#include "game/systems/map_render_system.h"
#include <engine/rendering/render_pipeline.h>
#include <engine/ecs/system/ui_system.h>
#include <engine/ecs/system/render_system.h>
//
//class WorldPass : public cursed_engine::RenderPass
//{
//public:
//	WorldPass(cursed_engine::RenderAPI renderAPI, cursed_engine::TextureManager* textureManager, cursed_engine::AssetManager* assetManager);
//
//	void execute(cursed_engine::RenderContext& ctx) override; // pass scene?
//
//private:
//	//cursed_engine::RenderSystem m_renderSystem;
//	MapRenderSystem m_mapRenderSystem;
//};
//
//class UIPass : public cursed_engine::RenderPass
//{
//public:
//	void execute(cursed_engine::RenderContext& ctx) override; // pass scene?
//
//};