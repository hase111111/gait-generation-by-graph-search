#pragma once

#include "graphic_data_broker.h"
#include "graphic_system.h"
#include "application_setting_recorder.h"


class ResultViewerSystemMain
{
public:
	ResultViewerSystemMain(const SApplicationSettingRecorder* const setting);


	void main();

private:
	void read();


	GraphicDataBroker m_broker;

	GraphicSystem m_graphic_system;
};

