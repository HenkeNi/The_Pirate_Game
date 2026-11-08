#pragma once
#include "engine/resources/audio/audio_manager.h"
#include "engine/resources/font/font_manager.h"
#include "engine/resources/texture/texture_manager.h"

namespace cursed_engine
{
	template <typename T>
	struct ResourceManagerTraits;

	// Rename? ResourceFacade?
	class Resources
	{
	public:
		Resources();
		Resources(AudioManager* audioManager, FontManager* fontManager, TextureManager* textureManager);

		// ==================== Resource insertion ====================,
		template <typename T, typename... Args>
		ResourceHandle<T> insert(std::unique_ptr<T> resource, Args&&... descriptorArgs);
		
		// ==================== Handle acquisition ====================
		template <typename T, typename U>
		[[nodiscard]] ResourceHandle<T> getHandle(const U& descriptor);

		template <typename T, typename... Args >
		[[nodiscard]] ResourceHandle<T> getHandleById(const std::string& id, Args&&... args);

		template <typename T, typename U>
		[[nodiscard]] ResourceHandle<T> findHandle(const U& descriptor) const;

		// ==================== Resource access ====================
		template <typename T>
		[[nodiscard]] const T& get(ResourceHandle<T> handle) const;

		template <typename T>
		[[nodiscard]] const T* tryGet(ResourceHandle<T> handle) const;	
		 	
		template <typename T>
		[[nodiscard]] const T& operator[](ResourceHandle<T> handle) const;

		template <typename T>
		void clear();

		// ==================== Queries ====================
		template <typename T, typename U>
		[[nodiscard]] bool contains(const U& descriptor) const noexcept;

		template <typename T>
		[[nodiscard]] bool isValid(ResourceHandle<T> handle) const noexcept;
		
	private:
		template <typename T>
		friend struct ResourceManagerTraits;

		TextureManager* m_textureManager;
		AudioManager* m_audioManager;
		FontManager* m_fontManager;
	};

	template <>
	struct ResourceManagerTraits<Texture>
	{
		static const TextureManager* getManager(const Resources* resources) noexcept
		{
			return resources->m_textureManager;
		}
		
		static TextureManager* getManager(Resources* resources) noexcept
		{
			return resources->m_textureManager;
		}
	};

	template <>
	struct ResourceManagerTraits<Audio>
	{
		static const AudioManager* getManager(const Resources* resources)
		{
			return resources->m_audioManager;
		}

		static AudioManager* getManager(Resources* resources)
		{
			return resources->m_audioManager;
		}
	};

	template <>
	struct ResourceManagerTraits<Font>
	{
		static const FontManager* getManager(const Resources* resources)
		{
			return resources->m_fontManager;
		}

		static FontManager* getManager(Resources* resources)
		{
			return resources->m_fontManager;
		}
	};

#pragma region Definitions

	template <typename T, typename... Args>
	ResourceHandle<T> Resources::insert(std::unique_ptr<T> resource, Args&&... descriptorArgs)
	{
		auto* manager = ResourceManagerTraits<T>::getManager(this);
		return manager->insert(std::move(resource), std::forward<Args>(descriptorArgs)...);
	}

	template <typename T, typename U>
	ResourceHandle<T> Resources::getHandle(const U& descriptor)
	{
		auto* manager = ResourceManagerTraits<T>::getManager(this);
		return manager->getHandle(descriptor);
	}

	template <typename T, typename... Args >
	ResourceHandle<T> Resources::getHandleById(const std::string& id, Args&&... args)
	{
		auto* manager = ResourceManagerTraits<T>::getManager(this);
		return manager->getHandleById(id, std::forward<Args>(args)...);
	}

	template <typename T, typename U>
	ResourceHandle<T> Resources::findHandle(const U& descriptor) const
	{
		const auto* manager = ResourceManagerTraits<T>::getManager(this);
		return manager->findHandle(descriptor);
	}

	template <typename T>
	const T& Resources::get(ResourceHandle<T> handle) const
	{
		const auto* manager = ResourceManagerTraits<T>::getManager(this);
		return manager->get(handle);
	}

	template <typename T>
	const T* Resources::tryGet(ResourceHandle<T> handle) const
	{
		const auto* manager = ResourceManagerTraits<T>::getManager(this);
		return manager->tryGet(handle);
	}

	template <typename T>
	const T& Resources::operator[](ResourceHandle<T> handle) const
	{
		const auto* manager = ResourceManagerTraits<T>::getManager(this);
		return manager[handle];
	}

	template <typename T>
	void Resources::clear()
	{
		auto* manager = ResourceManagerTraits<T>::getManager(this);
		manager->clear();
	}

	template <typename T, typename U>
	bool Resources::contains(const U& descriptor) const noexcept
	{
		const auto* manager = ResourceManagerTraits<T>::getManager(this);
		return manager->contains(descriptor);
	}

	template <typename T>
	bool Resources::isValid(ResourceHandle<T> handle) const noexcept 
	{
		const auto* manager = ResourceManagerTraits<T>::getManager(this);
		return manager->isValid(handle);
	}

#pragma endregion
}