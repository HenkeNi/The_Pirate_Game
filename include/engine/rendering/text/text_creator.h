#pragma once
#include "engine/core/result.h"
#include "engine/rendering/render_types.h"

struct TTF_TextEngine;

namespace cursed_engine
{
	template <typename T>
	class Result;

	class Font;

	class TextCreator final
	{
	public:
		void init(TTF_TextEngine* textEngine);
		[[nodiscard]] Result<TextPtr> createText(const std::string& str, const Font& font) const;

	private:
		TTF_TextEngine* m_textEngine = nullptr;
	};
}