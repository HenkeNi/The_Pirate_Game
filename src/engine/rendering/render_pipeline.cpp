#include "engine/rendering/render_pipeline.h"

namespace cursed_engine
{
	void RenderPipeline::insert(std::unique_ptr<RenderPass> pass)
	{
		m_renderPasses.push_back(std::move(pass));
	}
}