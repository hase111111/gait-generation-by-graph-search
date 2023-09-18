#pragma once

#include "interface_graphic_main_builder.h"


class TestGraphicMainBuilder : public IGraphicMainBuilder
{
public:

	std::unique_ptr<IGraphicMain> build(const GraphicDataBroker* const broker, std::shared_ptr<AbstractHexapodStateCalculator> calc,
		const SApplicationSettingRecorder* const setting) override;

};


class AdvanceGraphicMainBuilder : public IGraphicMainBuilder
{
public:

	std::unique_ptr<IGraphicMain> build(const GraphicDataBroker* const broker, std::shared_ptr<AbstractHexapodStateCalculator> calc,
		const SApplicationSettingRecorder* const setting) override;

};

