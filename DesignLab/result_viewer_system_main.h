#pragma once

#include <memory>
#include <string>

#include "application_setting_recorder.h"
#include "graphic_data_broker.h"
#include "graphic_system.h"
#include "interface_graphic_main.h"
#include "interface_system_main.h"


class ResultViewerSystemMain final : public ISystemMain
{
public:
	ResultViewerSystemMain(
		std::unique_ptr<IGraphicMain>&& graphic_ptr,
		const std::shared_ptr<const ApplicationSettingRecorder> setting_ptr
	);


	void Main() override;

private:

	const std::string kDirectoryName;

	const std::string kFileTreePath;

	GraphicSystem graphic_system_;
	void Read();


};

