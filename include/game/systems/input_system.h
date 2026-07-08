#pragma once
#include <engine/ecs/system/system.h>

namespace cursed_engine
{
	class Input;
}

class InputSystem : public cursed_engine::System
{
public:
	InputSystem(cursed_engine::Input* input);

	void update(cursed_engine::SystemContext& context) override;

private:
	cursed_engine::Input* m_input;
};