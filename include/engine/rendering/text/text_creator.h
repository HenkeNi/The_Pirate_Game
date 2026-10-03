#pragma once
#include "engine/core/result.h"
#include "engine/rendering/render_types.h"

struct TTF_TextEngine;

namespace cursed_engine
{
	template <typename T>
	class Result;

	class Font;

	class TextCreator
	{
	public:
		virtual ~TextCreator() = default;

		[[nodiscard]] virtual Result<TextPtr> createText(const std::string& text, const Font& font) const = 0;
	};

#pragma region SDL_Text_Creator

	class SDLTextCreator final : public TextCreator
	{
	public:
		void init(TTF_TextEngine* textEngine);
		[[nodiscard]] Result<TextPtr> createText(const std::string& str, const Font& font) const override;

	private:
		TTF_TextEngine* m_textEngine = nullptr;
	};

#pragma endregion
}