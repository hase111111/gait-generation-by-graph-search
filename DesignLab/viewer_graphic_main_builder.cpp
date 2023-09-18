#include "viewer_graphic_main_builder.h"

#include "graphic_main_graph_viewer.h"


std::unique_ptr<IGraphicMain> ViewerGraphicMainBuilder::build(const GraphicDataBroker* const broker, std::shared_ptr<AbstractHexapodStateCalculator> calc,
	const SApplicationSettingRecorder* const setting)
{
	return std::make_unique<GraphicMainGraphViewer>(broker, calc, setting);
}
