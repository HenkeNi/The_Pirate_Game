#pragma once
#include "engine/assets/asset_loader.h"
#include "engine/utils/type_traits.h"
#include "engine/utils/type_utils.h"
#include "engine/core/subsystem.h"
#include "engine/core/logger.h"
#include <cstdint>
#include <filesystem>
#include <limits>
#include <memory>
#include <mutex>
#include <string>
#include <type_traits>
#include <unordered_map>

namespace cursed_engine
{
	// put in mutex map (type index to mutex) if current approach is to slow
	// reserve vector size?

	struct AssetHandle
	{
		AssetHandle(uint32_t index_, uint32_t version_, std::type_index type_)
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

		bool operator==(const AssetHandle& other)
		{
			return index == other.index && version == other.version && type == other.type;
		}
	};


	class AssetManager : public Subsystem
	{
	public:
		template <typename Asset> // Dont use nodiscard here?
		[[nodiscard]] AssetHandle loadAsset(const std::filesystem::path& path); // Return optional handle?

		template <typename Asset>
		[[nodiscard]] const Asset& getAsset(AssetHandle handle);

		template <typename Asset>
		[[nodiscard]] const Asset* tryGetAsset(AssetHandle handle);

		template <DerivedFrom<AssetLoaderBase> Loader, typename... Args>
		void addLoader(Args&&... args);

		template <typename Asset>
		void addLoader(std::unique_ptr<AssetLoaderBase> loader);

		template <typename Asset>
		[[nodiscard]] bool isLoaderRegistered() const;

		void unloadAll();

	private:
		struct AssetCacheBase { virtual ~AssetCacheBase() = default; };

		template <typename Asset>
		struct AssetCache : AssetCacheBase
		{
			std::vector<Asset> storage;
		};

		template <typename Asset>
		AssetCache<Asset>& getOrCreateCache();

		template <typename Asset>
		[[nodiscard]] AssetLoader<Asset>* getLoader();

		using TypeToAssetCache = std::unordered_map<std::type_index, std::unique_ptr<AssetCacheBase>>;
		using TypeToLoaderMap = std::unordered_map<std::type_index, std::unique_ptr<AssetLoaderBase>>;

		TypeToAssetCache m_cachesByType;
		TypeToLoaderMap m_loadersByType;

		std::unordered_map<std::string, AssetHandle> m_pathToHandles;

		mutable std::mutex m_mutex;
	};

#pragma region Methods

	template <typename Asset>
	[[nodiscard]] AssetHandle AssetManager::loadAsset(const std::filesystem::path& path)
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		//std::string normalizedPath = NormalizePath(path);

		const std::string keyPath = path.string();

		if (auto it = m_pathToHandles.find(keyPath); it != m_pathToHandles.end())
		{
			// TODO; assert handle is same type?!
			// todo; check if valid handle (else remove handle/path)
			
			return it->second;
		}

		auto* assetLoader = getLoader<Asset>();
		if (!assetLoader)
		{
			Logger::logError("No asset loader for type found!");
			return AssetHandle{ AssetHandle::INVALID_INDEX, 0, getTypeIndex<Asset>() }; // TODO; throw instead!?
		}

		auto assetOpt = assetLoader->load(keyPath);

		if (!assetOpt.has_value())
		{
			Logger::logError("Failed to load asset!");
		}

		auto& storage = getOrCreateCache<Asset>().storage;
		storage.push_back(std::move(assetOpt.value()));

		AssetHandle handle{ (uint32_t)storage.size() - 1, 0, getTypeIndex<Asset>() };
		m_pathToHandles.insert({ keyPath, handle });

		return handle;
	}

	template <typename Asset>
	[[nodiscard]] const Asset& AssetManager::getAsset(AssetHandle handle)
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		// TODO; handle missing?

		auto& assetCache = getOrCreateCache<Asset>();
		return assetCache.storage.at(handle.index);
	}

	template <typename Asset>
	[[nodiscard]] const Asset* AssetManager::tryGetAsset(AssetHandle handle)
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		// TODO; handle missing?

		auto& assetCache = findOrCreateCache<Asset>();
		return &assetCache.storage.at(handle.index);
	}

	template <DerivedFrom<AssetLoaderBase> Loader, typename... Args>
	void AssetManager::addLoader(Args&&... args)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		//m_loadersByType.insert({ getTypeIndex<typename Loader::AssetType>(), nullptr });
		m_loadersByType.insert({ getTypeIndex<typename Loader::AssetType>(), std::make_unique<Loader>(std::forward<Args>(args)...) });
	}

	template <typename Asset>
	void AssetManager::addLoader(std::unique_ptr<AssetLoaderBase> loader)
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		// TEST
		//auto type = getTypeIndex<int>(); // if works, dont have to pass asset
		//auto type = getTypeIndex<typename loader::AssetType>(); // if works, dont have to pass asset

		m_loadersByType.insert({ getTypeIndex<Asset>(), std::move(loader) });
	}

	template <typename Asset>
	[[nodiscard]] bool AssetManager::isLoaderRegistered() const
	{
		return m_loadersByType.contains(getTypeIndex<Asset>());
	}

	template <typename Asset>
	AssetManager::AssetCache<Asset>& AssetManager::getOrCreateCache()
	{
		auto typeIndex = getTypeIndex<Asset>();

		if (!m_cachesByType.contains(typeIndex))
			m_cachesByType[typeIndex] = std::make_unique<AssetCache<Asset>>();

		return *static_cast<AssetCache<Asset>*>(m_cachesByType[typeIndex].get());
	}

	template <typename Asset>
	[[nodiscard]] AssetLoader<Asset>* AssetManager::getLoader()
	{
		auto typeIndex = getTypeIndex<Asset>();

		if (auto it = m_loadersByType.find(typeIndex); it != m_loadersByType.end())
		{
			return static_cast<AssetLoader<Asset>*>(it->second.get());
		}

		return nullptr;
	}

#pragma endregion
}


/*
* 
* class AssetManager : public Subsystem
	{
	public:
		template <typename Handle>
		[[nodiscard]] Handle load(const std::filesystem::path& path);

		template <typename Handle>
		[[nodiscard]] const typename Handle::AssetType& getAsset(Handle handle);

		template <typename Handle>
		[[nodiscard]] const typename Handle::AssetType* tryGetAsset(Handle handle);

		template <typename Loader, typename Asset, typename... Args>
		void addLoader(Args&&... args);

		template <typename Asset> // templated?
		void addLoader(std::unique_ptr<AssetLoaderBase> loader);

		template <typename Asset>
		[[nodiscard]] bool isLoaderRegistered() const;

	private:
		struct AssetCacheBase { virtual ~AssetCacheBase() = default; };

		template <typename Asset>
		struct AssetCache : AssetCacheBase
		{
			std::vector<Asset> storage;
		};

		template <typename Asset>
		AssetCache<Asset>& getOrCreateCache();

		template <typename Asset>
		[[nodiscard]] AssetLoader<Asset>* findLoader();

		template <typename Handle>
		[[nodiscard]] Handle* findHandle(const std::string& path); // make const?

		using TypeToLoaderMap = std::unordered_map<std::type_index, std::unique_ptr<AssetLoaderBase>>;
		using TypeToAssetCache = std::unordered_map<std::type_index, std::unique_ptr<AssetCacheBase>>;
		using PathToHandleMap = std::unordered_map<std::string, std::unique_ptr<AssetHandleBase>>;

		TypeToAssetCache m_cachesByType;  // TODO; create AssetCache class in separate file isntead??
		TypeToLoaderMap m_loadersByType;

		std::unordered_map<std::type_index, PathToHandleMap> m_handlesByType;

		mutable std::mutex m_mutex;
	};

#pragma region Methods

	template <typename Handle>
	[[nodiscard]] Handle AssetManager::load(const std::filesystem::path& path)
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		//std::string normalizedPath = NormalizePath(path);

		if (auto* handle = findHandle<Handle>(path.string()))
		{
			return *handle;
		}

		using Asset = typename Handle::AssetType;

		auto* assetLoader = findLoader<Asset>();
		if (!assetLoader)
		{
			Logger::logError("No asset loader for type found!");
			return Handle{ -1, -1 }; // TODO; FIX! return INVALID_ID....
		}

		auto asset = assetLoader->load(path);

		// TODO; use a cache instead?
		auto& assetContainer = getOrCreateCache<Asset>();
		assetContainer.storage.push_back(std::move(asset));

		// TODO; add to paths...
		auto& handles = m_handlesByType[getTypeIndex<Asset>()];

		Handle insertedHandle = Handle{ (uint32_t)assetContainer.size() - 1, 0 };
		handles.insert({ path.string(), insertedHandle });

		return insertedHandle;
	}

	template <typename Handle>
	[[nodiscard]] const typename Handle::AssetType& AssetManager::getAsset(Handle handle)
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		// TODO; handle missing?

		using Asset = typename Handle::AssetType;
		auto& assetCache = getOrCreateCache<Asset>();

		return assetCache.storage.at(handle.index);
	}

	template <typename Handle>
	[[nodiscard]] const typename Handle::AssetType* tryGetAsset(Handle handle)
	{

	}

	template <typename Loader, typename Asset, typename... Args>
	void AssetManager::addLoader(Args&&... args)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_loadersByType.insert({ getTypeIndex<Asset>(), std::make_unique<Loader>(std::forward<Args>(args)... });
	}

	template <typename Asset>
	void AssetManager::addLoader(std::unique_ptr<AssetLoaderBase> loader)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_loadersByType.insert({ getTypeIndex<Asset>(), std::move(loader) });
	}

	template <typename Asset>
	[[nodiscard]] bool AssetManager::isLoaderRegistered() const
	{
		return m_loadersByType.contains(getTypeIndex<Asset>());
	}

	template <typename Asset>
	AssetManager::AssetCache<Asset>& AssetManager::getOrCreateCache()
	{
		auto typeIndex = getTypeIndex<Asset>();

		if (!m_cachesByType.contains(typeIndex))
			m_cachesByType[typeIndex] = std::make_unique<AssetCache<Asset>>();

		return *static_cast<AssetCache<Asset>*>(m_cachesByType[typeIndex].get());
	}

	template <typename Asset>
	[[nodiscard]] AssetLoader<Asset>* AssetManager::findLoader()
	{
		auto typeIndex = getTypeIndex<Asset>();

		if (auto it = m_loadersByType.find(typeIndex); it != m_loadersByType.end())
		{
			return static_cast<AssetLoader<Asset>*>(it->second.get());
		}

		return nullptr;
	}

	template <typename Handle>
	[[nodiscard]] Handle* AssetManager::findHandle(const std::string& path) // make const?
	{
		using Asset = typename Handle::AssetType;

		auto typeIndex = getTypeIndex<Asset>();

		if (auto mapIt = m_handlesByType.find(typeIndex); mapIt != m_handlesByType.end())
		{
			if (auto handleIt = mapIt->second.find(path); handleIt != mapIt->second.end())
			{
				return handleIt->second;
			}
		}

		return nullptr;
	}

#pragma endregion
*/