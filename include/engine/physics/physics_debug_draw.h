#pragma once
#include "engine/physics/physics_types.h"
#include "engine/resources/font/font_manager.h"
#include "engine/rendering/render_api.h"

namespace cursed_engine
{
	class TextCreator;

	struct PhysicsDebugDrawContext
	{
		RenderAPI renderAPI;
		FontManager* fontManager;
		const TextCreator* textCreator;
	};

	class PhysicsDebugDraw
	{
	public:
		void init(PhysicsDebugDrawContext context);
		void draw();
		
		void setEnabled(bool enabled);
		void setWorldId(WorldId worldId);

		[[nodiscard]] inline constexpr bool isEnabled() const noexcept { return m_enabled; }

	private:
		PhysicsDebugDrawContext m_drawContext;
		WorldId m_worldId;

		bool m_enabled = false;
	};
}