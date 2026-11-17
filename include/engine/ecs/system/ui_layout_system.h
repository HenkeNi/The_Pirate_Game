#pragma once
#include "engine/ecs/system/system.h"

namespace cursed_engine
{
	class ECSRegistry;
	class Settings;

	// Does ui system run after and use screen rect? or before and calculate correct pos?
	class UILayoutSystem : public UpdateSystem
	{
	public:
		UILayoutSystem();
		void update(SystemUpdateContext& context) override;

	private:
		void updateUIElements(ECSRegistry& registry);
		void updateText(ECSRegistry& registry);
	};
}