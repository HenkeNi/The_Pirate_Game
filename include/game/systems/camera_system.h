#pragma once
#include <engine/ecs/system/system.h>

namespace cursed_engine
{
	struct TransformComponent;
	struct CameraComponent;
	class Settings;
}

namespace ce = cursed_engine;

class CameraSystem : public ce::UpdateSystem
{
public:
	CameraSystem(ce::Settings& settings);
	void update(ce::SystemUpdateContext& context) override;

private:
	void updateFollow(ce::CameraComponent& cameraComponent, ce::TransformComponent& transformComponent);

	ce::Settings& m_settings;
};
