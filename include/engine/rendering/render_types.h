#pragma once
#include <cstdint>

namespace cursed_engine
{

	struct Color
	{
		/*Color(uint8_t r_, uint8_t g_, uint8_t b_, uint8_t a_)
			: r{ r_ }, g{ g_ }, b{ b_ }, a{ a_ }
		{
		}*/

		uint8_t r, g, b, a;
		 
		static const Color black;
		static const Color white;
		static const Color gray;
		static const Color red;
		static const Color pink;
		static const Color purple;
		static const Color green;
		static const Color blue;
		static const Color yellow;
		static const Color orange;
		static const Color brown;
	};
	     
	inline const Color Color::black{ 0, 0, 0, 255 };
	inline const Color Color::white{ 255, 255, 255, 255 };
	inline const Color Color::gray{ 128, 128, 128, 128 };
	inline const Color Color::red{ 255, 0, 0, 255 };
	inline const Color Color::pink{ 255, 96, 208, 255 };
	inline const Color Color::purple{ 160, 32, 255, 255 };
	inline const Color Color::blue{ 0, 0, 255, 255 };
	inline const Color Color::green{ 0, 255, 0, 255 };
	inline const Color Color::yellow{ 255, 255, 0, 255 };
	inline const Color Color::orange{ 255, 208, 160, 255 };
	inline const Color Color::brown{ 160, 128, 96, 255 };

	// TODO; put in math?
	struct Rect
	{
		int x;
		int y;
		int w;
		int h;
	};

	struct FRect
	{
		float x;
		float y;
		float w;
		float h;
	};

	struct Line
	{
		FVec2 start;
		FVec2 end;
	};
}