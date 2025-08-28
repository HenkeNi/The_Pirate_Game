#pragma once
#include "engine/subsystems/subsystem_registry.h"

namespace cursed_engine
{
	class Application;

	class Engine
	{
	public:
		Engine(Application& app);
		~Engine();

		bool init();
		void shutdown();
		void run();

	private:
		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};
}