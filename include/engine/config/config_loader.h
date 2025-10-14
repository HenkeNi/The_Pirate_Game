#pragma once
#include <filesystem>
//
//namespace cursed_engine
//{
//	namespace fs = std::filesystem;
//
//	struct EngineConfig;
//
//	class ConfigHandler
//	{
//	public:
//		template <typename T>
//		[[nodiscard]] static std::optional<T> loadFromFile(const fs::path& path);
//
//		// load app info...
//
//		// load window confgs...
//
//
//		// tempalted instead?
//		static std::optional<EngineConfig> loadFromFile(const std::filesystem::path& path); // nodiscard?
//		static void saveToFile(const std::filesystem::path& path);
//
//	private:
//		//template <typename T>
//		//static T deserialize(const document& doc);
//
//		//template <typename T>
//		//static rapidjson::Document serialize(const T& config);
//
//	};
//
//#pragma region Methods
//
//	template<typename T>
//	std::optional<T> ConfigHandler::loadFromFile(const fs::path& path)
//	{
//		//JsonHandler json(path);
//
//		return std::optional<T>();
//	}
//
//
//	/*template <>
//	AppInfo ConfigHandler::deserialize()
//	{
//
//	}*/
//
//#pragma endregion
//}class ConfigLoader
//{
//public:
//	template <typename T>
//	[[nodiscard]] std::optional<T> loadFromFile(const fs::path& path) const;
//
//
//	// tempalted instead?
//	static std::optional<EngineConfig> loadFromFile(const std::filesystem::path& path); // nodiscard?
//	static void saveToFile(const std::filesystem::path& path);
//};
//
//#pragma region Methods
//
//template<typename T>
//std::optional<T> ConfigLoader::loadFromFile(const fs::path& path) const
//{
//	return std::optional<T>();
//}
//
//#pragma endregion