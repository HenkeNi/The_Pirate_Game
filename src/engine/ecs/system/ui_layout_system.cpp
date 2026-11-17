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
		componentView.forEach([&](const Entity& entity, const UILayoutComponent& uiLayoutComponent, UITransformComponent& uiTransformComponent)
			{			
				// calc position and size

				// handle nested (have parent)
				// handle offset...

				const FVec2 windowSize = context.windowSize;

				FVec2 min = windowSize * uiLayoutComponent.anchorMin;
				FVec2 max = windowSize * uiLayoutComponent.anchorMax;

				min -= uiLayoutComponent.offsetMin;
				max += uiLayoutComponent.offsetMax;

				uiTransformComponent.position = min;
				uiTransformComponent.size = max - min; // wont work if both min and max anchors have the same value...

				if (auto* textComponent = context.registry.tryGetComponent<TextComponent>(entity))
				{
					// if min max == 0.5 (text should be as large as needed (only))
					// alignment does nothign?

					// if min == 0.0 & max == 1.0 
					// alignment does something, given there is enough space

					const FVec2& size = (FVec2)textComponent->text->getSize();

					switch (textComponent->alignment)
					{
					case TextAlignment::Left:
						// remove size (clamp to 0)
						break;
					case TextAlignment::Center:
						uiTransformComponent.position -= size * 0.5f;
						uiTransformComponent.size -= size * 0.5f;
						break;
					case TextAlignment::Right:
						//uiTransformComponent.position += size;
						break;
					}
								
					// does text uses offset?

					// need to center text, etc....
					int x = 20; // why text has anchor 0,0 for both min and max
				}



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

	void UILayoutSystem::updateUIElements(ECSRegistry& registry)
	{
	}

	void UILayoutSystem::updateText(ECSRegistry& registry)
	{
		auto componentView = registry.view<UILayoutComponent, UITransformComponent>();		
	}
}