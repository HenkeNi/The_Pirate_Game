#pragma once
#include <engine/ecs/system/system.h>

namespace ce = cursed_engine;

class MovementSystem : public ce::UpdateSystem
{
public:
	void update(ce::SystemUpdateContext& context) override;
};