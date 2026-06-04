#pragma once
#include "engine/utils/non_copyable.h"

struct TTF_TextEngine;

namespace cursed_engine
{
	class Surface;
	class Text;
	class Renderer;

	class TextRenderer : private NonCopyable
	{
	public:
		TextRenderer();
		~TextRenderer(); // or do in shutdown?

		bool init(Renderer& renderer); // rename createTextRenderer?
		void shutdown();

		void renderText(Text& text, int x, int y);

		[[nodiscard]] inline TTF_TextEngine* getTextEngine() noexcept { return m_textEngine; }

	private:
		TTF_TextEngine* m_textEngine; // should live in TextManageR? TextContext? or return capabilities?
	};
}