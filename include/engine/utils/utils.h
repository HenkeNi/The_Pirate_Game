#pragma once

namespace cursed_engine::utils
{
	namespace hash
	{
		// https://stackoverflow.com/questions/19195183/how-to-properly-hash-the-custom-struct

		template <typename T>
		inline void hashCombine(std::size_t& seed, const T& value)
		{
			seed ^= std::hash<T>{}(value)
				+0x9e3779b9
				+ (seed << 6)
				+ (seed >> 2);
		}
	}
}