#include "other_graphic_main_builder.h"

#include "graphic_main_test.h"

std::unique_ptr<AbstractGraphicMain> TestGraphicMainBuilder::build(const GraphicDataBroker* const broker, std::shared_ptr<AbstractHexapodStateCalculator> calc,
	const SApplicationSettingRecorder* const setting)
{
	//作成して，ユニークポインタにmoveして返す．
	return std::make_unique<GraphicMainTest>(broker, calc, setting);
}
