#pragma once
#include <memory>
#include <vector>

namespace cursed_engine
{
	struct RenderContext;

	// here?
	class RenderPass
	{
	public:
		virtual ~RenderPass() = default;

		virtual void execute(RenderContext& ctx) = 0;
		//System* system; vector of systems?
	};

	class RenderPipeline
	{
	public:
		void insert(std::unique_ptr<RenderPass> pass);
		
		template <typename T, typename... Args>
		void emplace(Args&&... args);
		
		void render(); // private?	

	private:
		std::vector<std::unique_ptr<RenderPass>> m_renderPasses;
	};

#pragma region Definitions

	template <typename T, typename... Args>
	void RenderPipeline::emplace(Args&&... args)
	{
		m_renderPasses.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
	}

#pragma endregion
}