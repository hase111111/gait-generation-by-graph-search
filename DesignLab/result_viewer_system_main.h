#pragma once

#include <memory>

#include "graphic_data_broker.h"
#include "graphic_system.h"
#include "application_setting_recorder.h"


class ResultViewerSystemMain
{
public:
	ResultViewerSystemMain(const std::shared_ptr<const ApplicationSettingRecorder> setting_ptr);


	void Main();

private:
	void Read();


};

