#pragma once

#include "interface_graphic_main_builder.h"


class TestGraphicMainBuilder : public IGraphicMainBuilder
{
public:

	std::unique_ptr<AbstractGraphicMain> build(const GraphicDataBroker* const broker, const SApplicationSettingRecorder* const setting) override;

};
