#pragma once
#include "engine/platform/action.h" // remove?
#include <unordered_map> // remove when removing key bindings...

namespace cursed_engine
{
	enum class Key;
	using KeyBindings = std::unordered_map<Key, Action>; // redo...

	enum class InputState
	{
		Pressed,
		Held,
		Released,
		None
	};

	enum class MouseButton : uint8_t
	{
		Left = 1,
		Middle = 2,
		Right = 3,
		X1 = 4,
		X2 = 5,
		Count
	};

	enum class Key
	{
		UNKNOWN = 0,
		A = 4,
		B = 5,
		C = 6,
		D = 7,
		E = 8,
		F = 9,
		G = 10,
		H = 11,
		I = 12,
		J = 13,
		K = 14,
		L = 15,
		M = 16,
		N = 17,
		O = 18,
		P = 19,
		Q = 20,
		R = 21,
		S = 22,
		T = 23,
		U = 24,
		V = 25,
		W = 26,
		X = 27,
		Y = 28,
		Z = 29,

		Num1 = 30,
		Num2 = 31,
		Num3 = 32,
		Num4 = 33,
		Num5 = 34,
		Num6 = 35,
		Num7 = 36,
		Num8 = 37,
		Num9 = 38,
		Num0 = 39,

		RETURN = 40,
		ESCAPE = 41,
		BACKSPACE = 42,
		TAB = 43,
		SPACE = 44,

		MINUS = 45,
		EQUALS = 46,
		LEFTBRACKET = 47,
		RIGHTBRACKET = 48,
		BACKSLASH = 49,
		SEMICOLON = 51,
		APOSTROPHE = 52,
		GRAVE = 53,
		COMMA = 54,
		PERIOD = 55,
		SLASH = 56,
		COUNT = 512
	};

	struct InputInfo
	{
		InputState inputState = InputState::None; // Union? or remove?
		//InputState currentState = InputState::None; // all three needed? or remove this?
		//InputState previousState = InputState::None;
		bool isDown = false;
		bool wasDown = false;
	};
}