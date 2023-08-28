#pragma once

#include <memory>

#include "abstract_graphic_main.h"
#include "graphic_data_broker.h"
#include "application_setting_recorder.h"


class IGraphicMainBuilder
{
public:
	IGraphicMainBuilder() = default;
	virtual ~IGraphicMainBuilder() = default;

	virtual std::unique_ptr<AbstractGraphicMain> build(const GraphicDataBroker* const broker, const SApplicationSettingRecorder* const setting) = 0;

private:

};
