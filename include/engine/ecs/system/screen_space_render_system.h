#pragma once
#include "engine/resources/resource_types.h"
#include "engine/rendering/render_api.h"
#include "engine/ecs/system/system.h"

namespace cursed_engine
{
	class AssetManager;
	class ECSRegistry;

	class ScreenSpaceRenderSystem : public System
	{
	public:
		ScreenSpaceRenderSystem(TextureManager* textureManager, AssetManager* assetManager, RenderAPI renderer);

		void update(SystemContext& context) override; // or pass managers in update (renmae render)?

	private:
		void renderSprites(ECSRegistry& registry);
		void renderDebug(ECSRegistry& registry);
		void renderText(ECSRegistry& registry);

		AssetManager* m_assetManager;
		TextureManager* m_textureManager;
		RenderAPI m_renderer;
	};
}