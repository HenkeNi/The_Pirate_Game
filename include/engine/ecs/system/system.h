#pragma once

namespace cursed_engine
{
	class ECSRegistry;
	class EventBus;

	struct SystemContext
	{
		ECSRegistry& registry;
		EventBus& eventBus;
		float deltaTime;
	};

	class System
	{
	public:
		virtual ~System() = default;
		virtual void configure(ECSRegistry& registry) {};
		virtual void update(SystemContext& context) {}; // render and update functions?? or different base classes?
	};
}