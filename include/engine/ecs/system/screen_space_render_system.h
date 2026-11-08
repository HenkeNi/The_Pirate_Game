#pragma once
#include "engine/ecs/system/system.h"
#include "engine/rendering/render_api.h"
#include "engine/resources/resources.h"

namespace cursed_engine
{
	class AssetManager;
	class ECSRegistry;

	// Rename UIRenderSystem?
	class ScreenSpaceRenderSystem : public RenderSystem
	{
	public:
		ScreenSpaceRenderSystem(Resources resources, AssetManager* assetManager, RenderAPI renderer);

		void render(SystemRenderContext& context) override; // or pass managers in update (renmae render)?

	private:
		void renderSprites(ECSRegistry& registry);
		void renderDebug(ECSRegistry& registry);
		void renderText(ECSRegistry& registry);

		AssetManager* m_assetManager;
		Resources m_resources;
		RenderAPI m_renderer;
	};
}