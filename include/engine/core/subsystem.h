#pragma once

namespace cursed_engine
{
	class Subsystem
	{
	protected:
		Subsystem() = default;
		virtual ~Subsystem() = default;

		Subsystem(const Subsystem&) = delete;
		Subsystem(Subsystem&&) = delete;

		Subsystem& operator=(const Subsystem&) = delete;
		Subsystem& operator=(Subsystem&&) = delete;

		virtual void shutdown() {};
	};
}