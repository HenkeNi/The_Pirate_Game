#include "engine/ecs/system/ui_system.h"
#include "engine/ecs/component/core_components.h"
#include "engine/ecs/ecs_registry.h"
#include "engine/input/input_handler.h"

namespace cursed_engine
{
	UISystem::UISystem(InputHandler& inputHandler)
		: m_inputHandler{ inputHandler }
	{
	}

	void UISystem::update(ECSRegistry& registry, float deltaTime)
	{
		handleButtonInteractions(registry);
	}

	bool isInside(FVec2 min, FVec2 max, FVec2 point)
	{
		return point.x >= min.x && point.x <= max.x
			&& point.y >= min.y && point.y <= max.y;
	}

	void UISystem::handleButtonInteractions(ECSRegistry& registry)
	{
		FVec2 mousePosition = m_inputHandler.getMousePosition();

		auto view = registry.view<TransformComponent, ButtonComponent, BoundingBox>();
		view.forEach([&](TransformComponent& transformComponent, ButtonComponent& buttonComponent, BoundingBox& boundingBox)
			{
				FVec2 buttonPosition = transformComponent.position;
				buttonPosition.x += boundingBox.offset.x;
				buttonPosition.y += boundingBox.offset.y;

				// TODO; make sure max is larger than min!

				FVec2 max = buttonPosition + boundingBox.halfExtents;

				if (isInside(buttonPosition, max, mousePosition))
				{
					int x = 20;
					std::cout << "IS INSDIE!!!";
				}


			});
	}
}