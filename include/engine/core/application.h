#pragma once
#include "engine/utils/non_copyable.h"
#include <filesystem>

namespace cursed_engine
{
	struct EngineContext;

	class Application : private NonCopyable
	{
	public:
		Application() = default;
		virtual ~Application() = default;

		Application(Application&&) = delete;
		Application& operator=(Application&&) = delete;

		virtual void onUpdate(float deltaTime) = 0;

		virtual void onCreated(const EngineContext& context) {}; // NOTE, context will go out of scope... maybe pass copy or pass m_impl directly?
		virtual void onDestroyed() {};
	};
}