#pragma once
#include "engine/ecs/system/system.h"
#include "engine/resources/resource_types.h"
#include "engine/rendering/render_api.h"

namespace cursed_engine
{
	class AssetManager;
	class ECSRegistry;

	class WorldRenderSystem : public System
	{
	public:
		WorldRenderSystem(TextureManager* textureManager, AssetManager* assetManager, RenderAPI renderer);

		void update(SystemContext& context) override;
	
	private:
		void renderSprites(ECSRegistry& registry);
		void renderDebug(ECSRegistry& registry);

		AssetManager* m_assetManager;
		TextureManager* m_textureManager;
		RenderAPI m_renderer;
	};
}