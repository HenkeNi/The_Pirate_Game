#pragma once
#include "engine/math/vec2.hpp"
#include <cstdint>
#include <vector>

namespace cursed_engine
{
	struct Color
	{
		constexpr Color()
			: r{ 255 }, g{ 255 }, b{ 255 }, a{ 255 }
		{
		}

		constexpr Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255)
			: r{ r }, g{ g }, b{ b }, a{ a }
		{
		}

		bool operator==(const Color& other) const
		{
			return r == other.r 
				&& g == other.g 
				&& b == other.b 
				&& a == other.a;
		}

		bool operator!=(const Color& other) const
		{
			return !(*this == other);
		}

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

	struct Vertex
	{
		Vertex() = default;
		Vertex(FVec2 position, FVec2 uv, Color color = Color::white)
			: position{ position }, uv{ uv }, color{ color }
		{
		}

		FVec2 position; // or structure same order as SDL_Vertex?
		FVec2 uv; // or tex_coords?
		Color color;
	};

	struct Geometry
	{
		std::vector<Vertex> vertices;
		std::vector<int> indices;
		//std::vector<uint32_t> indices;
	};

	enum class Orientation
	{
		Horizontal,
		Vertical
	};

	// add constexpr constructor?
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

	struct UVRect
	{
		float u0;
		float v0;
		float u1;
		float v1;
	};

	struct Line
	{
		FVec2 start;
		FVec2 end;
	};
}