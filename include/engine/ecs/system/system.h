#pragma once
#include "engine/rendering/render_api.h"

namespace cursed_engine
{
	class ECSRegistry;
	class EventBus;

	struct SystemUpdateContext
	{
		ECSRegistry& registry;
		EventBus& eventBus;
		FVec2 windowSize{};
		float deltaTime{};
		float fps{};
	};

	struct SystemRenderContext
	{
		ECSRegistry& registry;
		RenderAPI renderAPI;
	};


	/*template <typename ExecutionContext>
	class System
	{
	public:
		virtual void execute(ExecutionContext& context) {};
	};*/

	//class System
	//{
	//public:
	//	virtual ~System() = default;
	//	virtual void configure(ECSRegistry& registry) {};
	//	virtual void update(SystemContext& context) {}; // render and update functions?? or different base classes?
	//};

	class UpdateSystem
	{
	public:
		virtual ~UpdateSystem() = default;
		virtual void update(SystemUpdateContext& context) {};
	};

	class RenderSystem
	{
	public:
		virtual ~RenderSystem() = default;
		virtual void render(SystemRenderContext& context) {}; // pass same render function all the way from engine??
	};
}