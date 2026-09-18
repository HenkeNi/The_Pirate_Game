#include "game/systems/camera_system.h"
#include "game/components/components.h"
#include <engine/ecs/component/core_components.h>
#include <engine/ecs/ecs_registry.h>
#include <engine/core/settings/settings.h>

CameraSystem::CameraSystem(ce::Settings& settings)
	: m_settings{ settings }
{
}

void CameraSystem::update(ce::SystemContext& context)
{
	//auto view = context.registry.view<ce::CameraComponent, ce::HierarchyComponent, ce::TransformComponent>();
	//view.forEach([](const ce::CameraComponent& cameraComponent, const ce::HierarchyComponent& hierarchyComponent, ce::TransformComponent& transformComponent)

	

	auto view = context.registry.view<ce::CameraComponent, ce::TransformComponent, ce::FollowComponent>();
	view.forEach([&](const ce::CameraComponent& cameraComponent, ce::TransformComponent& transformComponent, const ce::FollowComponent& followComponent) // TODO; find first instead??
		{
			if (!followComponent.target.isValid())
				return;

			const ce::TransformComponent& targetTransformComponent = followComponent.target.getComponent<ce::TransformComponent>();

			const ce::FVec2& windowSize = m_settings.getWindowSize();

			transformComponent.position.x = targetTransformComponent.position.x - (windowSize.x * 0.5f);
			transformComponent.position.y = targetTransformComponent.position.y - (windowSize.y * 0.5f);

			// maybe target is not needed??
		});
}

void CameraSystem::updateFollow(ce::CameraComponent& cameraComponent, ce::TransformComponent& transformComponent)
{

}