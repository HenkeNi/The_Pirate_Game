#pragma once
#include <engine/ecs/system/system.h>

class DebugSystem : public cursed_engine::UpdateSystem
{
public:
	void update(cursed_engine::SystemUpdateContext& context) override;
};