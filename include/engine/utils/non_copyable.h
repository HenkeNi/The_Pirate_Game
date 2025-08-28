#pragma once

namespace cursed_engine
{
	class NonCopyable
	{
	protected:
		NonCopyable() = default;
		~NonCopyable() = default;

		NonCopyable(const NonCopyable&) = delete;
		NonCopyable& operator=(const NonCopyable&) = delete;
	};
}