#pragma once

namespace cursed_engine
{
	class World;

	class System
	{
	public:
		virtual ~System() = default;
		virtual void configure(World& world) {};
		virtual void update(World& world, float deltaTime) = 0;
	};
}