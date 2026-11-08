#include "engine/resources/resources.h"
#include "engine/resources/resource_manager.hpp"

namespace cursed_engine
{
	Resources::Resources()
		: m_audioManager{ nullptr }, m_fontManager{ nullptr }, m_textureManager{ nullptr }
	{
	}

	Resources::Resources(AudioManager* audioManager, FontManager* fontManager, TextureManager* textureManager)
		: m_audioManager{ audioManager }, m_fontManager{ fontManager }, m_textureManager{ textureManager }
	{
	}

	//Resources::TextureHandle Resources::insertTexture(TextureDescriptor descriptor, std::unique_ptr<Texture> texture)
	//{
	//	return m_textureManager->insert(std::move(descriptor), std::move(texture));
	//}

	//Resources::AudioHandle Resources::insertAudio(AudioDescriptor descriptor, std::unique_ptr<Audio> audio)
	//{
	//	return m_audioManager->insert(std::move(descriptor), std::move(audio));
	//}

	//Resources::FontHandle Resources::insertFont(FontDescriptor descriptor, std::unique_ptr<Font> font)
	//{
	//	return m_fontManager->insert(std::move(descriptor), std::move(font));
	//}

	//Resources::TextureHandle Resources::getTextureHandle(const TextureDescriptor& descriptor)
	//{
	//	return m_textureManager->getHandle(descriptor);
	//}

	//Resources::AudioHandle Resources::getAudioHandle(const AudioDescriptor& descriptor)
	//{
	//	return m_audioManager->getHandle(descriptor);
	//}
	//
	//Resources::FontHandle Resources::getFontHandle(const FontDescriptor& descriptor)
	//{
	//	return m_fontManager->getHandle(descriptor);
	//}

	//Resources::TextureHandle Resources::findTextureHandle(const TextureDescriptor& descriptor) const
	//{
	//	return m_textureManager->findHandle(descriptor);
	//}

	//Resources::AudioHandle Resources::findAudioHandle(const AudioDescriptor& descriptor) const
	//{
	//	return m_audioManager->findHandle(descriptor);
	//}

	//Resources::FontHandle Resources::findFontHandle(const FontDescriptor& descriptor) const
	//{
	//	return m_fontManager->findHandle(descriptor);
	//}

	//const Texture& Resources::getTexture(TextureHandle handle) const
	//{
	//	return m_textureManager->get(handle);
	//}

	//const Audio& Resources::getAudio(AudioHandle handle) const
	//{
	//	return m_audioManager->get(handle);
	//}

	//const Font& Resources::getFont(FontHandle handle) const
	//{
	//	return m_fontManager->get(handle);
	//}

	//const Texture* Resources::tryGetTexture(TextureHandle handle) const
	//{
	//	return m_textureManager->tryGet(handle);
	//}

	//const Audio* Resources::tryGetAudio(Resources::AudioHandle handle) const
	//{
	//	return m_audioManager->tryGet(handle);
	//}

	//const Font* Resources::tryGetFont(Resources::FontHandle handle) const
	//{
	//	return m_fontManager->tryGet(handle);
	//}

	//void Resources::clearTextures()
	//{
	//	m_textureManager->clear();
	//}

	//void Resources::clearAudio()
	//{
	//	m_audioManager->clear();
	//}

	//void Resources::clearFonts()
	//{
	//	m_fontManager->clear();
	//}

	//void Resources::clearAll()
	//{
	//	clearTextures();
	//	clearAudio();
	//	clearFonts();
	//}

	//bool Resources::containsTexture(const TextureDescriptor& descriptor) const noexcept
	//{
	//	return m_textureManager->contains(descriptor);
	//}

	//bool Resources::containsAudio(const AudioDescriptor& descriptor) const noexcept
	//{
	//	return m_audioManager->contains(descriptor);
	//}

	//bool Resources::containsFont(const FontDescriptor& descriptor) const noexcept
	//{
	//	return m_fontManager->contains(descriptor);
	//}

	//bool Resources::isTextureValid(TextureHandle handle) const noexcept
	//{
	//	return m_textureManager->isValid(handle);
	//}

	//bool Resources::isAudioValid(AudioHandle handle) const noexcept
	//{ 
	//	return m_audioManager->isValid(handle);
	//}

	//bool Resources::iFontValid(FontHandle handle) const noexcept
	//{
	//	return m_fontManager->isValid(handle);
	//}
}