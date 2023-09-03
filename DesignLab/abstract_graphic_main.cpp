#include "abstract_graphic_main.h"


AbstractGraphicMain::AbstractGraphicMain(const GraphicDataBroker* broker, std::shared_ptr<AbstractHexapodStateCalculator> calc,
	const SApplicationSettingRecorder* const setting) : mp_broker(broker), mp_calculator(calc), mp_setting(setting)
{
}
