#pragma once
#include <memory>

namespace cursed_engine
{
	class Application;
	struct EngineContext;

	class Engine
	{
	public:
		explicit Engine(Application& app);
		~Engine();

		bool init();
		void shutdown();
		void run();

	private:
		EngineContext context() const;

		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};
}