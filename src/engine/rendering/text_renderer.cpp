#include "engine/rendering/text_renderer.h"
#include "engine/rendering/renderer.h"
#include "engine/resources/text/text.h"
#include "engine/core/logger.h"
#include <SDL3_ttf/SDL_ttf.h>
#include <cassert>
#include <format>

namespace cursed_engine
{
	TextRenderer::TextRenderer()
		: m_textEngine{ nullptr }
	{
	}

	TextRenderer::~TextRenderer()
	{
	}

	bool TextRenderer::init(Renderer& renderer)
	{
		if (!TTF_Init())
		{
			Logger::logError(std::format("Failed to initialize TTF! Error: {}", SDL_GetError()));
			return false;
		}

		m_textEngine = TTF_CreateRendererTextEngine(renderer.getRenderer());
	
		if (!m_textEngine)
		{
			Logger::logError(std::format("Failed to create a TextEngine. Error: {}", SDL_GetError()));
			return false;
		}

		return true;
	}

	void TextRenderer::shutdown()
	{
		TTF_DestroyRendererTextEngine(m_textEngine);
		TTF_Quit();

		m_textEngine = nullptr;
	}

	// Text having a "draw" function? or get TTF_Text?
	void TextRenderer::renderText(Text& text, int x, int y)
	{
		assert(text.isValid() && "Not a valid text");
		TTF_DrawRendererText(text.get(), x, y);
	}
}