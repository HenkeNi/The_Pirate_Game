#pragma once
#include <engine/ecs/system/system.h>

class PlayerControllerSystem : public cursed_engine::System
{
public:
	PlayerControllerSystem();

	void update(cursed_engine::SystemContext& context) override;

private:

};