#pragma once

namespace rapidjson
{
	class CrtAllocator;

	template<typename CharType>
	struct UTF8;

	template <typename BaseAllocator>
	class MemoryPoolAllocator;

	template <typename Encoding, typename Allocator>
	class GenericValue;

	typedef GenericValue<UTF8<char>, MemoryPoolAllocator<CrtAllocator> > Value;
}