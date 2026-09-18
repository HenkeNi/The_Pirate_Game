#pragma once
#include "engine/assets/asset_loader.h"
#include "engine/utils/concepts.h"
#include "engine/utils/utils.h"
#include "engine/core/logger.h"
#include "engine/core/result.h"
#include <cstdint>
#include <filesystem>
#include <limits>
#include <memory>
#include <mutex>
#include <string>
#include <type_traits>
#include <vector>
#include <unordered_map>

namespace cursed_engine
{
	// put in mutex map (type index to mutex) if current approach is to slow
	// reserve vector size?

	struct AssetHandle
	{
		AssetHandle(uint32_t index_, uint32_t version_, std::type_index type_) // TODO; try to make tempalted instead?
			: index{ index_ }, version{ version_ }, type{ type_ }
		{
		}

		static constexpr uint32_t INVALID_INDEX = std::numeric_limits<uint32_t>::max();

		uint32_t index;
		uint32_t version = 0; // ever used?

		std::type_index type;

		bool isValid() const
		{
			return index != INVALID_INDEX;
		}

		bool operator==(const AssetHandle& other) const
		{
			return index == other.index && version == other.version && type == other.type;
		}
	};

	// AssetStore, AssetRegistry?
	class AssetManager
	{
	public:
		void addSearchPath(std::filesystem::path path);
		void scanAssets();

		template <typename Asset> // Dont use nodiscard here? 
		AssetHandle preload(const std::string& id); // Return optional handle?

		template <typename Asset>
		[[nodiscard]] AssetHandle getAssetHandle(const std::string& id); // name or id? return optional?
		// TODO; getAssetHandle(const char* name) const;

		template <typename Asset>
		[[nodiscard]] const Asset& getAsset(AssetHandle handle) const; // make const?

		template <typename Asset>
		[[nodiscard]] const Asset* tryGetAsset(AssetHandle handle);

		template <DerivedFrom<AssetLoaderBase> Loader, typename... Args>
		void addLoader(Args&&... args);

		template <typename Asset>
		void addLoader(std::unique_ptr<AssetLoaderBase> loader);

		template <typename Asset>
		[[nodiscard]] bool isLoaderRegistered() const;

		template <typename Asset>
		[[nodiscard]] bool isValidHandle(AssetHandle handle) const noexcept;

		void unloadAll();

	private:
		template <typename Asset>
		[[nodiscard]] AssetHandle load(const std::string& id);

		template <typename Asset>
		[[nodiscard]] std::vector<Asset>* getOrCreateStorage();

		template <typename Asset>
		[[nodiscard]] const std::vector<Asset>* findStorage() const;
		
		[[nodiscard]] std::string extractAssetId(const std::filesystem::path& path, const std::string& format) const;

		struct AssetCacheBase { virtual ~AssetCacheBase() = default; };

		template <typename Asset>
		struct AssetCache : AssetCacheBase
		{
			std::vector<Asset> storage;
		};

		struct AssetMetaData
		{
			/*AssetMetaData(std::filesystem::path path, AssetLoaderBase* loader = nullptr)
				: path{ std::move(path) }, loader{ loader }
			{
			}*/

			std::filesystem::path path;
			//AssetLoaderBase* loader;
		};

		struct AssetType
		{
			std::unordered_map<std::string, AssetMetaData> idToMetaData;
			std::unordered_map<std::string, AssetHandle> idToAssetHandle;

			std::unique_ptr<AssetCacheBase> cache = nullptr;
			std::unique_ptr<AssetLoaderBase> loader = nullptr;
		};


		/*template <typename Asset>
		AssetCache<Asset>& getOrCreateCache();

		template <typename Asset>
		AssetCache<Asset>* findCache() const;

		template <typename Asset>
		[[nodiscard]] AssetLoader<Asset>* getLoader();*/

		

		//[[nodiscard]] std::string extractIdentifier(const std::filesystem::path& path); // TODO; make utlity function?

		//using TypeToAssetCache = std::unordered_map<std::type_index, std::unique_ptr<AssetCacheBase>>;
		//using TypeToLoader = std::unordered_map<std::type_index, std::unique_ptr<AssetLoaderBase>>;

		//TypeToAssetCache m_cachesByType; // m_cache or asset storage
		//TypeToLoader m_loadersByType; // m_loaders

		//std::unordered_map<std::string, AssetMetaData> m_idToMetaData;
		//std::unordered_map<std::string, AssetHandle> m_idToAssetHandle;

		//std::unordered_map<std::filesystem::path, AssetHandle> m_pathToHandles;
		//std::unordered_map<std::string, std::filesystem::path> m_idsToPaths;


		std::unordered_map<std::type_index, AssetType> m_assetTypes;

		std::vector<std::filesystem::path> m_searchPaths;

		// TODO; id's to paths or ids to handles?
		mutable std::mutex m_mutex; // put in archive?
	};

#pragma region Definitions

	template <typename Asset>
	AssetHandle AssetManager::preload(const std::string& id)
	{
		return load<Asset>(id);
	}

	template <typename Asset>
	[[nodiscard]] AssetHandle AssetManager::getAssetHandle(const std::string& id)
	{
		return load<Asset>(id);
	}

	template <typename Asset>
	[[nodiscard]] const Asset& AssetManager::getAsset(AssetHandle handle) const
	{
		//assert(handle.IsValid());

		std::lock_guard<std::mutex> lock(m_mutex);

		//auto* assetCache = findCache<Asset>();

		//assert(assetCache && "Not a valid asset cache");
		// TODO; handle missing?
		//const AssetType& assetType = m_assetTypes.at(utils::getTypeIndex<Asset>());
		//return assetType.cache->storage.at(handle.index);

		const auto* storage = findStorage<Asset>();
		return storage->at(handle.index);

		// Throw?
	}

	template <typename Asset>
	[[nodiscard]] const Asset* AssetManager::tryGetAsset(AssetHandle handle)
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		// TODO; handle missing?

		return getOrCreateStorage<Asset>()->at(handle.index);

		//const AssetType& assetType = m_assetTypes.at(utils::getTypeIndex<Asset>());

		//auto& assetCache = findOrCreateCache<Asset>();
		//return &assetType.cache.storage.at(handle.index);
	}

	template <DerivedFrom<AssetLoaderBase> Loader, typename... Args>
	void AssetManager::addLoader(Args&&... args)
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		auto& assetType = m_assetTypes[utils::getTypeIndex<typename Loader::AssetType>()];
		//auto& assetType = m_assetTypes.at();
		assetType.loader = std::make_unique<Loader>(std::forward<Args>(args)...);

		//m_loadersByType.insert({ getTypeIndex<typename Loader::AssetType>(), nullptr });
		//m_loadersByType.insert_or_assign(utils::getTypeIndex<typename Loader::AssetType>(), std::make_unique<Loader>(std::forward<Args>(args)...));
	}

	template <typename Asset>
	void AssetManager::addLoader(std::unique_ptr<AssetLoaderBase> loader)
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		// TEST
		//auto type = getTypeIndex<int>(); // if works, dont have to pass asset
		//auto type = getTypeIndex<typename loader::AssetType>(); // if works, dont have to pass asset
		//auto& assetType = m_assetTypes.at(utils::getTypeIndex<Asset>());

		auto& assetType = m_assetTypes[utils::getTypeIndex<Asset >()];
		assetType.loader = std::move(loader);
	}

	template <typename Asset>
	bool AssetManager::isLoaderRegistered() const
	{
		auto type = utils::getTypeIndex<Asset>();

		if (!m_assetTypes.contains(type))
			return false;

		auto& assetType = m_assetTypes.at(type);
		return assetType.loader != nullptr;
		//return m_loadersByType.contains(utils::getTypeIndex<Asset>());
	}

	template <typename Asset>
	bool AssetManager::isValidHandle(AssetHandle handle) const noexcept
	{
		auto type = utils::getTypeIndex<Asset>();

		if (!m_assetTypes.contains(type))
			return false;

		auto& assetType = m_assetTypes.at(type);
		return assetType.idToAssetHandle.contains(handle);
		//return m_idToAssetHandle.find(handle); // make sure work
	}

	/*template <typename Asset>
	AssetManager::AssetCache<Asset>& AssetManager::getOrCreateCache()
	{
		auto typeIndex = utils::getTypeIndex<Asset>();

		if (!m_cachesByType.contains(typeIndex))
			m_cachesByType[typeIndex] = std::make_unique<AssetCache<Asset>>();

		return *static_cast<AssetCache<Asset>*>(m_cachesByType[typeIndex].get());
	}*/

	/*template <typename Asset>
	AssetManager::AssetCache<Asset>* AssetManager::findCache() const
	{
		auto typeIndex = utils::getTypeIndex<Asset>();

		if (m_cachesByType.contains(typeIndex))
			return static_cast<AssetCache<Asset>*>(m_cachesByType.at(typeIndex).get());

		return nullptr;
	}*/

	/*template <typename Asset>
	[[nodiscard]] AssetLoader<Asset>* AssetManager::getLoader()
	{
		auto typeIndex = utils::getTypeIndex<Asset>();

		if (auto it = m_loadersByType.find(typeIndex); it != m_loadersByType.end())
		{
			return static_cast<AssetLoader<Asset>*>(it->second.get());
		}

		return nullptr;
	}*/

	template <typename Asset>
	AssetHandle AssetManager::load(const std::string& id)
	{
		//std::lock_guard<std::mutex> lock(m_mutex); // if loading resource cause other esource to load, error two locks!!

		auto& assetType = m_assetTypes[utils::getTypeIndex<Asset>()];

		// If already loaded...
		if (auto it = assetType.idToAssetHandle.find(id); it != assetType.idToAssetHandle.end())
		{
			return it->second;
		}

		auto it = assetType.idToMetaData.find(id);
		const bool found = it != assetType.idToMetaData.end();

		assert(found && "Asset path not registered!");

		if (!found)
		{
			Logger::logError("[AssetManager::Load] - Failed to find meta data for asset!");
			return AssetHandle{ AssetHandle::INVALID_INDEX, 0, utils::getTypeIndex<Asset>() };
		}

		AssetLoader<Asset>* assetLoader = static_cast<AssetLoader<Asset>*>(assetType.loader.get());
		
		assert(assetLoader && "Not a valid asset loader!");

		if (!assetLoader)
		{
			Logger::logError("[AssetManager::Load] - Invalid asset loader!");
			return AssetHandle{ AssetHandle::INVALID_INDEX, 0, utils::getTypeIndex<Asset>() };
		}

		Result<Asset> result = assetLoader->load(it->second.path);

		if (!result.ok())
		{
			Logger::logError("[AssetManager::Load] - Failed to load asset!");
			return AssetHandle{ AssetHandle::INVALID_INDEX, 0, utils::getTypeIndex<Asset>() };
		}

		// Check if cache is null, allocate memory, cast to correct 
	

		/*if (!assetType.cache)
		{
			std::unique_ptr<AssetCache<Asset>> cache = std::make_unique<AssetCache<Asset>>();
			storage = &cache->storage;
		} 
		else
		{
			AssetCache<Asset>* cache = static_cast<AssetCache<Asset>*>(it->cache.get());
			storage = &cache->storage;
		}*/

		//fix this line....
		//std::vector<Asset>& storage = static_cast<AssetCache<Asset>>(assetType.cache).storage;

		std::vector<Asset>* storage = getOrCreateStorage<Asset>();
		//storage->push_back(std::move(asset.value()));
		storage->push_back(std::move(result.take()));

		AssetHandle handle{ (uint32_t)storage->size() - 1, 0, utils::getTypeIndex<Asset>() }; // TODO; use 1 instaed of 0 for version
		assetType.idToAssetHandle.insert_or_assign(id, handle);

		return handle;

		/*if (auto* loader = it->second.loader)
		{
			assetLoader = static_cast<AssetLoader<Asset>*>(loader);
		}
		else
		{
			assetLoader = getLoader<Asset>();
			loader = assetLoader;
		}*/


		/*if (!loader)
		{
			AssetLoader<Asset>* assetLoader = getLoader<Asset>();
			loader = assetLoader;
		}*/

		//AssetLoader<Asset>* assetLoader = static_cast<AssetLoader<Asset>*>(loader);

		//assert(assetLoader && "Not a valid asset loader!");

		// assert(it->second.path is valid path)


		//if (auto asset = assetLoader->load(it->second.path))
		//{
		//	auto& storage = getOrCreateCache<Asset>().storage;

		//	storage.push_back(std::move(asset.value()));

		//	AssetHandle handle{ (uint32_t)storage.size() - 1, 0, utils::getTypeIndex<Asset>() }; // TODO; use 1 instaed of 0 for version
		//	m_idToAssetHandle.insert_or_assign(id, handle);

		//	return handle;
		//}
		
	}

	template <typename Asset>
	std::vector<Asset>* AssetManager::getOrCreateStorage()
	{
		AssetType& assetType = m_assetTypes[utils::getTypeIndex<Asset>()];

		if (!assetType.cache)
		{
			assetType.cache = std::make_unique<AssetCache<Asset>>();
		}

		return &static_cast<AssetCache<Asset>*>(assetType.cache.get())->storage;
	}

	template <typename Asset>
	const std::vector<Asset>* AssetManager::findStorage() const
	{
		const AssetType& assetType = m_assetTypes.at(utils::getTypeIndex<Asset>());

		if (assetType.cache)
		{
			return &static_cast<AssetCache<Asset>*>(assetType.cache.get())->storage;
		}

		return nullptr;
	}

#pragma endregion
}