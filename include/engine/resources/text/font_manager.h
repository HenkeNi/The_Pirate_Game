#pragma once
#include "engine/resources/resource_manager.hpp"
#include "engine/resources/resource_loaders.h"
#include "engine/resources/text/font.h"

namespace cursed_engine
{
	using FontManager = ResourceManager<Font, FontDescriptor, FontLoader>;
}