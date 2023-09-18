#include "basic_graphic_main_builder.h"

#include "graphic_main_basic.h"


std::unique_ptr<IGraphicMain> BasicGraphicMainBuilder::build(const GraphicDataBroker* const broker, std::shared_ptr<AbstractHexapodStateCalculator> calc,
	const SApplicationSettingRecorder* const setting)
{
	//作成して，ユニークポインタにmoveして返す．
	return std::make_unique<GraphicMainBasic>(broker, calc, setting);
}
