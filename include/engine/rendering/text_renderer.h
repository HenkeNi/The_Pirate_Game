#pragma once
#include "engine/core/subsystem.h"

struct TTF_TextEngine;

namespace cursed_engine
{
	class Surface;
	class Text;
	class Renderer;

	class TextRenderer : public Subsystem
	{
	public:
		TextRenderer();
		~TextRenderer(); // or do in shutdown?

		void init(Renderer& renderer);
		void renderText(Text& text, int x, int y);

		[[nodiscard]] inline TTF_TextEngine* getTextEngine() noexcept { return m_textEngine; }

	private:
		TTF_TextEngine* m_textEngine;
	};
}