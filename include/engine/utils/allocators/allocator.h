#pragma once

namespace cursed_engine
{
	class Allocator
	{
	public:
		virtual ~Allocator() = default;

		virtual void allocate(std::size_t size) = 0;
		virtual void free(void* p) = 0;
	};
}