#pragma once
#include "engine/utils/non_copyable.h"

namespace cursed_engine
{
	class Application : private NonCopyable
	{
	public:
		Application() = default;
		virtual ~Application() = default;

		Application(Application&&) = delete;
		Application& operator=(Application&&) = delete;

		virtual void onUpdate() {};
		virtual void onCreated() {};
		virtual void onDestroyed() {};
	};
}