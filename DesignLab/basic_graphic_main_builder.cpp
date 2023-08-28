#include "basic_graphic_main_builder.h"

#include "graphic_main_basic.h"


std::unique_ptr<AbstractGraphicMain> BasicGraphicMainBuilder::build(const GraphicDataBroker* const broker, const SApplicationSettingRecorder* const setting)
{
	//作成して，ユニークポインタにmoveして返す．
	return std::make_unique<GraphicMainBasic>(broker, setting);
}
