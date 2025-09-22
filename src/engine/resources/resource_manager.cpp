#include "engine/resources/resource_manager.h"
#include "engine/resources/resource_cache.hpp"
#include "engine/resources/texture_loader.h"
#include "engine/rendering/texture.h"

namespace cursed_engine
{
    struct ResourceManager::Impl
    {        
        Impl(Renderer& rdr )
            : renderer{ rdr }
        {
        }

        ResourceCache<Texture, TextureTag> textureCache;
        // ResourceCache<Audio> audioCache;

        std::unordered_map<std::string, ResourceHandle<TextureTag>> pathToTextureHandle;

        TextureLoader textureLoader;

        Renderer& renderer;

        mutable std::mutex textureMutex;
    };

    ResourceManager::ResourceManager(Renderer& renderer)
        : m_impl{ std::make_unique<ResourceManager::Impl>(renderer) }
    {
    }

    ResourceManager::~ResourceManager() = default;

    TextureHandle ResourceManager::getTexture(const std::filesystem::path& path)
    {
        std::lock_guard<std::mutex> lock(m_impl->textureMutex);

        //std::string normalizedPath = NormalizePath(path);

        auto& textureCache = m_impl->textureCache;
        auto& textureLoader = m_impl->textureLoader;

        auto& pathToTextureHandle = m_impl->pathToTextureHandle;
        
        if (auto it = pathToTextureHandle.find(path.string()); it != pathToTextureHandle.end())
        {
            if (textureCache.isValidHandle(it->second))
                return it->second; // TODO; increment ref count?
            else
                pathToTextureHandle.erase(path.string());
        }

        auto texture = textureLoader.load(m_impl->renderer, path);
        auto handle = textureCache.insert(std::move(texture));
            
        pathToTextureHandle.insert({ path.string(), handle });            
        return handle;        
    }

    Texture* ResourceManager::resolve(TextureHandle handle)
    {
        std::lock_guard<std::mutex> lock(m_impl->textureMutex);

        // TODO: check if handle is valid? - or leave that soley to cache?

        auto& textureCache = m_impl->textureCache;
        return textureCache.tryGet(handle);
    }
}