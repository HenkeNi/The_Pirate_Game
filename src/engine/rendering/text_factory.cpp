#include "engine/rendering/text_factory.h"
#include "engine/rendering/font.h"
#include "engine/rendering/text.h"
#include "engine/resources/resource_handle.h"
#include <SDL3_ttf/SDL_ttf.h>

namespace cursed_engine
{
	TextFactory::TextFactory(FontManager& fontManager, TTF_TextEngine* engine)
		: m_fontManager{ fontManager }, m_textEngine { engine }
	{
	}

	Text TextFactory::createText(const std::string& text, ResourceHandle<Font> fontHandle)
	{
		if (auto* font = m_fontManager.get(fontHandle))
		{
			TTF_Text* textObject = TTF_CreateText(m_textEngine, font->getInternal(), text.c_str(), text.size());

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