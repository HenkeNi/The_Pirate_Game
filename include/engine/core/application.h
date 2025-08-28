#pragma once
#include "engine/utils/non_copyable.h"

namespace cursed_engine
{
	class SubsystemRegistry;

	class Application : private NonCopyable
	{
	public:
		Application() = default;
		virtual ~Application() = default;

		Application(Application&&) = delete;
		Application& operator=(Application&&) = delete;

		void init();
		//void init(SubsystemRegistry* registry);

		virtual void onUpdate() {};
		virtual void onCreated() {};
		virtual void onDestroyed() {};

	protected:
		//SubsystemRegistry* m_subsystemRegistry;
	};
}