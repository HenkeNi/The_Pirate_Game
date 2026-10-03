#pragma once
#include "engine/rendering/render_api.h"
#include "engine/rendering/render_pipeline.h"
#include "engine/core/result.h"
#include <memory>

namespace cursed_engine
{
	/*struct RenderCapabilities
	{
		std::function<class Texture(Renderer&, struct Surface)> createTexture;
	};*/

	struct RenderConfig;
	class TextureCreator;
	class TextCreator;
	class RenderBackend;
	class Window;

	enum class Backend;

	class RenderModule
	{
	public:
		RenderModule();
		~RenderModule();

		RenderModule(const RenderModule&) = delete;
		RenderModule(RenderModule&&) = delete;
		
		RenderModule& operator=(const RenderModule&) = delete;
		RenderModule& operator=(RenderModule&&) = delete;

		bool init(Window& window, const RenderConfig& config, Backend backend);
		void shutdown();

		void beginFrame();
		void endFrame();

		// Primary access
		[[nodiscard]] inline RenderAPI getRenderAPI() noexcept { return RenderAPI{ m_backend.get() }; }
		[[nodiscard]] inline RenderPipeline& getRenderPipeline() noexcept { return m_renderPipeline; }

		[[nodiscard]] const TextureCreator* getTextureCreator() const noexcept;
		[[nodiscard]] const TextCreator* getTextCreator() const noexcept;

		// Optional: Low-level access (use sparingly)
		//[[nodiscard]] inline RenderBackend& getBackend() noexcept { return *m_renderer; }

	private:
		std::unique_ptr<RenderBackend> m_backend;
		RenderPipeline m_renderPipeline;

		//RenderCapabilities m_capabilities;
		// RenderGraph?
	};
}