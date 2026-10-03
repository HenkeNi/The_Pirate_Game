#pragma once
#include "engine/resources/audio/audio.h"
#include "engine/resources/resource_loaders.h"
#include "engine/resources/resource_manager.hpp"

namespace cursed_engine
{
	using AudioManager = ResourceManager<Audio, AudioDescriptor, AudioLoader>;
}