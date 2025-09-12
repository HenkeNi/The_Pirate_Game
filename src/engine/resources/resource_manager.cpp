#include "C:/Dev/Projects/The_Pirate_Game/build/CMakeFiles/Engine.dir/Debug/cmake_pch.hxx"
#include "engine/resources/resource_manager.h"
#include "engine/resources/resource_cache.hpp"
#include "engine/resources/texture_loader.h"

namespace cursed_engine
{
    //class Audio;

    struct ResourceManager::Impl
    {        
        // Caches
        ResourceCache<Texture, TextureTag> m_textureCache;
        // ResourceCache<Audio> m_audioCache;

        // Path to handles
        std::unordered_map<std::string, ResourceHandle<TextureTag>> m_pathToTextureHandle;

        // Loaders
        TextureLoader m_textureLoader;

    };

    ResourceManager::ResourceManager(Renderer& renderer)
        : m_impl{ std::make_unique<ResourceManager::Impl>() }, m_renderer{ renderer }
    {
    }

    ResourceManager::~ResourceManager()
    {
    }

    TextureHandle ResourceManager::getTexture(const std::filesystem::path& path)
    {
        // TODO; mutex here or in cache?? 
        //std::string normalizedPath = NormalizePath(path);

        auto& textureCache = m_impl->m_textureCache;
        auto& textureLoader = m_impl->m_textureLoader;

        auto& pathToTextureHandle = m_impl->m_pathToTextureHandle;
        
        if (auto it = pathToTextureHandle.find(path.string()); it != pathToTextureHandle.end())
        {
            if (textureCache.isValidHandle(it->second))
                return it->second; // TODO; increment ref count?
            else
                pathToTextureHandle.erase(path.string());
        }

        auto texture = textureLoader.loadTexture(m_renderer, path);
        auto handle = textureCache.insert(std::move(texture));
            
        pathToTextureHandle.insert({ path.string(), handle });            
        return handle;        
    }

    Texture* ResourceManager::resolve(TextureHandle handle)
    {
        // TODO: check if handle is valid? - or leave that soley to cache?

        auto& textureCache = m_impl->m_textureCache;
        return textureCache.tryGet(handle);
    }
}