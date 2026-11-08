#include "engine/platform/cursor.h"
#include <SDL3/SDL.h>

namespace cursed_engine
{
	Cursor::Cursor()
		: m_cursor{ nullptr }
	{
	}

	void Cursor::SetVisible(bool visible)
	{
		//SDL_CreateColorCursor
		//m_cursor->
		//SDL_SetCursor();
	}

	/*void Cursor::SetImage(const CursorImage& image)
	{

	}*/

	void Cursor::SetMode(CursorMode mode)
	{

	}
}