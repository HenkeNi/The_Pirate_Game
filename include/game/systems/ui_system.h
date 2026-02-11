#pragma once
#include <engine/ecs/system/system.h>

class UISystem : public cursed_engine::System
{
public:
	void update(cursed_engine::ECSRegistry& registry, float deltaTime) override;

private:

};