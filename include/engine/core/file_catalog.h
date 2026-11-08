#pragma region
#include "engine/utils/utils.h"
#include <filesystem>
#include <string>
#include <typeindex>
#include <unordered_map>

namespace cursed_engine
{
	struct FileKey
	{
		std::type_index type;
		std::string id;

		bool operator==(const FileKey&) const = default;
	};
}

template<>
struct std::hash<cursed_engine::FileKey>
{
	std::size_t operator()(const cursed_engine::FileKey& key) const noexcept
	{
		std::size_t h = std::hash<std::string>{}(key.id);

		h ^= std::hash<std::type_index>{}(key.type)
			+ 0x9e3779b9 + (h << 6) + (h >> 2);

		return h;
	}
};

namespace cursed_engine
{
	// Or PathResolver?
	class FileCatalog
	{
	public:
		void scanDirectory(const std::filesystem::path& root);

		template <typename T>
		const std::filesystem::path* getPath(const std::string& id);

		void registerTypeToValidExtensions();

	private:
		struct PathEntry
		{

		};

		//void addValidFormat();
		// std::unordered_map<

	private:
		std::unordered_map<FileKey, std::filesystem::path> m_paths;
		//std::unordered_map<std::string> validFileFormats;
	};

#pragma region Definitions

	template <typename T>
	const std::filesystem::path* FileCatalog::getPath(const std::string& id)
	{
		FileKey key
		{
			utils::getTypeIndex<T>(),
			id
		};

		if (auto it = m_paths.find(key); it != m_paths.end())
		{
			return it->second;
		}
	
		return std::filesystem::path();
	}

#pragma endregion
}