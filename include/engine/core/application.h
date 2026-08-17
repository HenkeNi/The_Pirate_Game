#pragma once
#include "engine/utils/non_copyable.h"

namespace cursed_engine
{
	struct EngineContext;
	struct RenderContext;

	class Application : private NonCopyable
	{
	public:
		Application() = default;
		virtual ~Application() = default;

		Application(Application&&) = delete;
		Application& operator=(Application&&) = delete;

		virtual void onUpdate(float deltaTime) = 0;
		virtual void onRender(const RenderContext& ctx) = 0;
		
		virtual void onCreated(const EngineContext& ctx) = 0; // NOTE, context will go out of scope... maybe pass copy or pass m_impl directly?
		virtual void onDestroyed() = 0;
	};
}