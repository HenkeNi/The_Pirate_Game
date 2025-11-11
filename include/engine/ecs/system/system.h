#pragma once

namespace cursed_engine
{
	class ECSRegistry;

	class System
	{
	public:
		virtual ~System() = default;
		virtual void configure(ECSRegistry& registry) {};
		virtual void update(ECSRegistry& registry, float deltaTime) = 0; // render and update functions?? or different base classes?
	};
}