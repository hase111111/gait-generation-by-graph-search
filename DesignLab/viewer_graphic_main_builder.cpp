#include "viewer_graphic_main_builder.h"

#include "graphic_main_graph_viewer.h"


std::unique_ptr<AbstractGraphicMain> ViewerGraphicMainBuilder::build(const GraphicDataBroker* const broker, const SApplicationSettingRecorder* const setting)
{
	return std::make_unique<GraphicMainGraphViewer>(broker, setting);
}
