#pragma once
#include "engine/utils/non_copyable.h"
#include "engine/rendering/render_api.h"
#include "engine/core/settings/engine_config.h"
#include "engine/core/result.h"
#include <memory>

namespace cursed_engine
{
	/*struct RenderCapabilities
	{
		std::function<class Texture(Renderer&, struct Surface)> createTexture;
	};*/

	class ResourceCreator;
	class RenderBackend;
	class Window;

	class RenderModule : public NonCopyable
	{
	public:
		RenderModule();
		~RenderModule();

		RenderModule(RenderModule&&) = delete;
		RenderModule& operator=(RenderModule&&) = delete;

		bool init(Window& window, const RenderConfig& config);
		void shutdown();

		void beginFrame();
		void endFrame();

		// Primary access
		[[nodiscard]] inline RenderAPI getRenderAPI() noexcept { return RenderAPI{ m_backend.get() }; }

		[[nodiscard]] ResourceCreator* getResourceCreator() noexcept;

		// Optional: Low-level access (use sparingly)
		//[[nodiscard]] inline RenderBackend& getBackend() noexcept { return *m_renderer; }

	private:
		std::unique_ptr<RenderBackend> m_backend;

		//RenderCapabilities m_capabilities;
		// RenderGraph?
	};
}