#pragma once
#include <filesystem>
#include <string>
#include <typeindex>

namespace cursed_engine::utils
{
	template <typename T>
	[[nodiscard]] inline std::type_index getTypeIndex() noexcept
	{
		return std::type_index(typeid(std::remove_cvref_t<T>));
	}

	// elsewhere??
	[[nodiscard]] std::string extractAssetID(const std::filesystem::path& path);
	//[[nodiscard]] std::string extractResourceID(const std::filesystem::path& path); remove?

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

	namespace string_helpers
	{
		// constexpr? and noexcept?
		[[nodiscard]] constexpr std::string toLowercase(std::string str);

		[[nodiscard]] std::string remove(std::string str, const std::string& substr);

		[[nodiscard]] std::string removeLast(std::string str, const std::string& substr);

		[[nodiscard]] std::string removeSuffix(std::string str, const std::string& suffix);
	}
}