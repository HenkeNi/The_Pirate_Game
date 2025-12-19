#pragma once
#include <vector>

namespace cursed_engine
{
	// TODO; store "completed" animations in a structure (flyweight pattern)

	struct Animation
	{
		struct Frame
		{
			// uniformed duration?

			// Texture atlas?

			// uv

			float duration;
		};

		// SpriteSheetHandle?
		std::vector<Frame> frames;
	};
}