#pragma once

namespace cursed_engine
{
	struct SystemContext
	{
		class ECSRegistry& registry;
		class EventBus& eventBus;
		float deltaTime;
	};

	class System
	{
	public:
		virtual ~System() = default;
		virtual void configure(ECSRegistry& registry) {};
		virtual void update(SystemContext& context) = 0; // render and update functions?? or different base classes?
	};
}