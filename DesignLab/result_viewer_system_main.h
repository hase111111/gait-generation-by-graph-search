#pragma once

#include <memory>
#include <string>

#include "application_setting_recorder.h"
#include "graphic_data_broker.h"
#include "graphic_system.h"
#include "interface_graphic_main.h"
#include "interface_system_main.h"
#include "result_file_importer.h"


class ResultViewerSystemMain final : public ISystemMain
{
public:
	ResultViewerSystemMain(
		std::unique_ptr<IGraphicMain>&& graphic_ptr,
		const std::shared_ptr<GraphicDataBroker>& broker_ptr,
		const std::shared_ptr<const ApplicationSettingRecorder> setting_ptr
	);


	void Main() override;

private:

	GraphicSystem graphic_system_;

	ResultFileImporter result_importer_;

	const std::shared_ptr<GraphicDataBroker> broker_ptr_;
};

