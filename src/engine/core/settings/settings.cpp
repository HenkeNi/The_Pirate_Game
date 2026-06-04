#include "engine/core/settings/settings.h"

namespace cursed_engine
{
	Settings::Settings(ConfigManager& config, Window& window)
		: m_configManager{ config }, m_window{ window }
	{
	}
}