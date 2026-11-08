#include "engine/ecs/system/ui_layout_system.h"
#include "engine/ecs/component/core_components.h"
#include "engine/core/settings/settings.h"

namespace cursed_engine
{
	UILayoutSystem::UILayoutSystem()
	{
	}

	void UILayoutSystem::update(SystemUpdateContext& context)
	{
		auto componentView = context.registry.view<UILayoutComponent, UITransformComponent>();
		componentView.forEach([&](const UILayoutComponent& uiLayoutComponent, UITransformComponent& uiTransformComponent)
			{
				// handle nested (have parent)
				// handle offset...

				const FVec2 windowSize =  context.windowSize;

				FVec2 min = windowSize * uiLayoutComponent.anchorMin;
				FVec2 max = windowSize * uiLayoutComponent.anchorMax;
			
				min -= uiLayoutComponent.offsetMin;
				max += uiLayoutComponent.offsetMax;

				uiTransformComponent.position = min;
				uiTransformComponent.size = max - min; // wont work if both min and max anchors have the same value...
				// uiTransformComponent. // size insteaD?

				// calculate pivot... (use pivot in uilayout component(?) - rather than texture region pivot...
				// how to apply pivot? (add a size variable?)

				if (uiLayoutComponent.anchorMin == uiLayoutComponent.anchorMax)
				{
					// use offset as width
				}


				// use texture (region) offset here (pivot)??
				// rect.min = parentRect.min + parentRect.size() * layout.anchorMin + layout.offsetMin;

				//screenRectComponent.rect = FRect{ min.x, min.y, max.x, max.y };
			});
	}
}