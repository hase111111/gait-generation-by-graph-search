#include "other_graphic_main_builder.h"

#include "graphic_main_test.h"
#include "graphic_main_advance.h"


std::unique_ptr<IGraphicMain> TestGraphicMainBuilder::build(const GraphicDataBroker* const broker, std::shared_ptr<AbstractHexapodStateCalculator> calc,
	const SApplicationSettingRecorder* const setting)
{
	//作成して，ユニークポインタにmoveして返す．
	return std::make_unique<GraphicMainTest>(broker, calc, setting);
}

std::unique_ptr<IGraphicMain> AdvanceGraphicMainBuilder::build(const GraphicDataBroker* const broker, std::shared_ptr<AbstractHexapodStateCalculator> calc, const SApplicationSettingRecorder* const setting)
{
	return std::make_unique<GraphicMainAdvance>(broker, calc, setting);
}
