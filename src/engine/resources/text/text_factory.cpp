#include "engine/resources/text/text_factory.h"
#include "engine/resources/text/font.h"
#include "engine/resources/text/text.h"
#include "engine/resources/resource_handle.h"
#include "engine/rendering/renderer.h"
#include <SDL3_ttf/SDL_ttf.h>
#include <cassert>

namespace cursed_engine
{
	TextFactory::TextFactory(FontManager* fontManager, Renderer& renderer)
		: m_fontManager{ fontManager }, m_renderer{ &renderer }
	{
	}

	void TextFactory::init(FontManager* fontManager, Renderer& renderer)
	{
		m_fontManager = fontManager;
		m_renderer = &renderer;
	}

	Text TextFactory::createText(const std::string& text, ResourceHandle<Font> fontHandle)
	{
		assert(m_renderer && "Renderer is not valid!");

		if (auto* font = m_fontManager->get(fontHandle))
		{
			TTF_Text* textObject = TTF_CreateText(m_renderer->getTextEngine(), font->getInternal(), text.c_str(), text.size());

			if (!textObject)
			{
				Logger::logError("Unable to generate text: " + text + ", error: " + SDL_GetError());
				return Text{ nullptr };
			}

			return Text{ textObject };
		}

		Logger::logError("Text creation failed! Could not find the required font!");
		return Text{ nullptr };
	}
}