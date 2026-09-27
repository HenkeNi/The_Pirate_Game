#pragma once
#include <engine/ecs/system/system.h>

class PlayerControllerSystem : public cursed_engine::UpdateSystem
{
public:
	PlayerControllerSystem();

	void update(cursed_engine::SystemUpdateContext& context) override;

private:

};