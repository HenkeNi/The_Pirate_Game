#pragma once
#include "engine/rendering/render_api.h"
#include "engine/physics/physics_types.h"

namespace cursed_engine
{
	class PhysicsDebugDraw
	{
	public:
		void init(RenderAPI renderAPI);
		void draw();
		
		void setDebugDrawEnabled(bool enabled); // or just setEnabled?
		void setWorldId(WorldId worldId);

		[[nodiscard]] inline constexpr bool isDebugDrawEnabled() const noexcept { return m_debugDrawEnabled; } 

	private:
		RenderAPI m_renderAPI;
		WorldId m_worldId;

		bool m_debugDrawEnabled = false;
	};
}