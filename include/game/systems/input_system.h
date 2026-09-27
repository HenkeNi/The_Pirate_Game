#pragma once
#include <engine/ecs/system/system.h>
#include <engine/platform/input_api.h>

namespace cursed_engine
{
	class InputAPI;
}

class InputSystem : public cursed_engine::UpdateSystem
{
public:
	InputSystem(cursed_engine::InputAPI input);

	void update(cursed_engine::SystemUpdateContext& context) override;

private:
	cursed_engine::InputAPI m_input;
};