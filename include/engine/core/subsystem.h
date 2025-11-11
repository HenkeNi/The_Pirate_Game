#pragma once

namespace cursed_engine
{
	class Subsystem
	{
	public:
		virtual ~Subsystem() = default;
		virtual void shutdown() {};
	};
}