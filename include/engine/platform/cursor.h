#pragma once

struct SDL_Cursor;

namespace cursed_engine
{
	enum class CursorMode
	{
		Normal,
		Hidden
	};

	class Cursor final
	{
	public:
		Cursor();

		void SetVisible(bool visible);
		//void SetImage(const CursorImage& image);
		void SetMode(CursorMode mode);

	private:
		SDL_Cursor* m_cursor;
	};
}