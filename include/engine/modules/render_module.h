#pragma once
#include "engine/rendering/render_api.h"
#include "engine/rendering/renderer.h"
#include <functional>

namespace cursed_engine
{
	/*struct RenderCapabilities
	{
		std::function<class Texture(Renderer&, struct Surface)> createTexture;
	};*/

	class Window;

	class RenderModule
	{
	public:
		//RenderModule();

		bool init(Window& window);
		void shutdown();

		void beginFrame();
		void endFrame();

		//RenderCapabilities getCapabilities() const noexcept;

		// Primary access
		[[nodiscard]] inline RenderAPI getRenderAPI() noexcept { return RenderAPI{ &m_renderer }; }

		// Optional: Low-level access (use sparingly)
		[[nodiscard]] inline Renderer& getRenderer() noexcept { return m_renderer; }

	private:
		Renderer m_renderer; // use IRenderer instead??

		//RenderCapabilities m_capabilities;
		// RenderGraph?
	};
}