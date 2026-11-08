#pragma once
#include "engine/rendering/render_api.h"
#include "engine/rendering/render_backend.h"
#include "engine/rendering/render_pipeline.h"

namespace cursed_engine
{
	/*struct RenderCapabilities
	{
		std::function<class Texture(Renderer&, struct Surface)> createTexture;
	};*/

	class TextureCreator;
	class TextCreator;
	class Window;

	struct GraphicsArgs
	{
		bool vsync;
	};

	class Graphics
	{
	public:
		Graphics();
		~Graphics();

		Graphics(const Graphics&) = delete;
		Graphics(Graphics&&) = delete;
		
		Graphics& operator=(const Graphics&) = delete;
		Graphics& operator=(Graphics&&) = delete;

		bool init(Window& window, const GraphicsArgs& args);
		void shutdown();

		void beginFrame();
		void endFrame();

		// Primary access
		[[nodiscard]] inline RenderAPI getRenderAPI() noexcept { return RenderAPI{ &m_backend }; } // move to cpp - remove include
		[[nodiscard]] inline RenderPipeline& getRenderPipeline() noexcept { return m_renderPipeline; }

		[[nodiscard]] const TextureCreator* getTextureCreator() const noexcept;
		[[nodiscard]] const TextCreator* getTextCreator() const noexcept;

		// Optional: Low-level access (use sparingly)
		//[[nodiscard]] inline RenderBackend& getBackend() noexcept { return *m_renderer; }

	private:
		RenderBackend m_backend;
		RenderPipeline m_renderPipeline;

		//RenderCapabilities m_capabilities;
		// RenderGraph?
	};
}