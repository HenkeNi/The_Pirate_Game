#pragma once

struct SDL_Cursor;

namespace cursed_engine
{
	enum class CursorMode
	{
		Normal,
		Hidden
	};

#pragma region Cursor

	class Cursor
	{
	public:
		virtual ~Cursor() = default;

		virtual void SetVisible(bool visible) = 0;
		//virtual void SetImage(const CursorImage& image) = 0;
		virtual void SetMode(CursorMode mode) = 0;
	};

#pragma endregion

#pragma region SDL_Cursor

	class SDLCursor : public Cursor
	{
	public:
		void SetVisible(bool visible) override;
		//void SetImage(const CursorImage& image) override;
		void SetMode(CursorMode mode) override;

	private:
		SDL_Cursor* m_cursor;
	};

#pragma endregion
}