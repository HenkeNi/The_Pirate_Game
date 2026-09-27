#pragma once
#include <engine/ecs/system/system.h>

namespace cursed_engine
{
	class FrameTimer;
}

class DebugSystem : public cursed_engine::UpdateSystem
{
public:
	DebugSystem(cursed_engine::FrameTimer& timer);
	void update(cursed_engine::SystemUpdateContext& context) override;

private:
	cursed_engine::FrameTimer& m_timer;
};