#pragma once

namespace cursed_engine
{
	class Subsystem
	{
	public:
		virtual ~Subsystem() = default;
		virtual void shutdown() {};

	protected:
		Subsystem() = default;

		Subsystem(const Subsystem&) = delete;
		Subsystem(Subsystem&&) = delete;

		Subsystem& operator=(const Subsystem&) = delete;
		Subsystem& operator=(Subsystem&&) = delete;
	};
}