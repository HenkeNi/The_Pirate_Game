#pragma once
#include <engine/ecs/system/system.h>

namespace cursed_engine
{
	class EventBus;
	class ECSRegistry;
	class EntityFactory;
}

namespace ce = cursed_engine;

class HUDSystem : public ce::UpdateSystem
{
public:
	HUDSystem(ce::EventBus& eventBus, ce::EntityFactory& factory);
	void update(ce::SystemUpdateContext& context) override;

	void setECSRegistry(ce::ECSRegistry* registry); // or setWorld(); ?

private:
	void createHealthContainer(int currentHealth, int maxHealth); // or construct?

	ce::EventBus& m_eventBus;
	ce::ECSRegistry* m_ecsRegistry; // better option? -- pass Registry to constructor?
	ce::EntityFactory& m_entityFactory;
};