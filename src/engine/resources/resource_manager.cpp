#include "engine/resources/resource_manager.h"
#include "engine/resources/resource_cache.hpp"
#include "engine/resources/texture_loader.h"
#include "engine/resources/audio_loader.h"
#include "engine/rendering/texture.h"
#include "engine/audio/audio.h"

namespace cursed_engine
{
    struct ResourceManager::Impl
    {        
        Impl(Renderer& rdr)
            : renderer{ rdr }
        {
        }

        ResourceCache<Texture, TextureTag> textureCache;
        ResourceCache<Audio, AudioTag> audioCache;

        std::unordered_map<std::string, TextureHandle> pathToTextureHandle;
        std::unordered_map<std::string, AudioHandle> pathToAudioHandle;

        TextureLoader textureLoader;
        AudioLoader audioLoader;

        Renderer& renderer;

        mutable std::mutex textureMutex;
        mutable std::mutex audioMutex;
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

    AudioHandle ResourceManager::getAudio(const std::filesystem::path& path)
    {
        std::lock_guard<std::mutex> lock(m_impl->audioMutex);

        auto& audioCache = m_impl->audioCache;
        auto& audioLoader = m_impl->audioLoader;

        auto& pathToAudioHandle = m_impl->pathToAudioHandle;

        if (auto it = pathToAudioHandle.find(path.string()); it != pathToAudioHandle.end())
        {
            if (audioCache.isValidHandle(it->second))
                return it->second;
            else
                pathToAudioHandle.erase(path.string());
        }

        auto audio = audioLoader.load(path);
        auto handle = audioCache.insert(std::move(audio));

        pathToAudioHandle.insert({ path.string(), handle });
        return handle;
    }

    Audio* ResourceManager::resolve(AudioHandle handle)
    {
        std::lock_guard<std::mutex> lock(m_impl->audioMutex);

        auto& audioCache = m_impl->audioCache;
        return audioCache.tryGet(handle);
    }
}