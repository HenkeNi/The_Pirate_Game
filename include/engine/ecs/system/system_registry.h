#pragma once

namespace cursed_engine
{
	class SystemManager;

	class SystemRegistry
	{
	public:
		static void registerCoreSystems(SystemManager& systemManager);
	};
}