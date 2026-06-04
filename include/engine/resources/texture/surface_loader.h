#pragma once
#include <filesystem>

namespace cursed_engine
{
	struct Surface;

	struct SurfaceLoader
	{
		[[nodiscard]] Surface operator()(const std::filesystem::path& path) const;
	};
}