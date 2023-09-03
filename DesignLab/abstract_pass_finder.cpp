#include "abstract_pass_finder.h"


void AbstractPassFinder::init(std::unique_ptr<AbstractPassFinderFactory>&& factory, std::shared_ptr<AbstractHexapodStateCalculator> calc, const SApplicationSettingRecorder* const setting)
{
	mp_factory = std::move(factory);

	mp_calculator = calc;

	mp_setting = setting;
}
