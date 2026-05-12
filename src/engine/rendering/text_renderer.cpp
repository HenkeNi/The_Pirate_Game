#include "engine/rendering/text_renderer.h"
#include "engine/rendering/renderer.h"
#include "engine/rendering/text.h"
#include <SDL3_ttf/SDL_ttf.h>

namespace cursed_engine
{
	TextRenderer::TextRenderer()
		: m_textEngine{ nullptr }
	{
	}

	TextRenderer::~TextRenderer()
	{
		TTF_DestroyRendererTextEngine(m_textEngine);
		m_textEngine = nullptr;
	}

	void TextRenderer::init(Renderer& renderer)
	{
		m_textEngine = TTF_CreateRendererTextEngine(renderer.getRenderer());
	}

	// Text having a "draw" function? or get TTF_Text?
	void TextRenderer::renderText(Text& text, int x, int y)
	{
		TTF_DrawRendererText(text.get(), x, y);
	}
}