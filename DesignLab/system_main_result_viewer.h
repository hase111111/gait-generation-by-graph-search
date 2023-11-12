//! @file system_main_result_viewer.h
//! @brief 結果を表示するシステムのクラス．


#ifndef DESIGNLAB_SYSTEM_MAIN_RESULT_VIEWER_H_
#define DESIGNLAB_SYSTEM_MAIN_RESULT_VIEWER_H_


#include <memory>
#include <string>

#include "application_setting_recorder.h"
#include "graphic_data_broker.h"
#include "interface_system_main.h"
#include "result_file_importer.h"


//! @class SystemMainResultViewer
//! @brief 結果を表示するシステムのクラス．
class SystemMainResultViewer final : public ISystemMain
{
public:
	SystemMainResultViewer(
		const std::shared_ptr<GraphicDataBroker>& broker_ptr,
		const std::shared_ptr<const ApplicationSettingRecorder> setting_ptr
	);


	void Main() override;

private:

	ResultFileImporter result_importer_;

	const std::shared_ptr<GraphicDataBroker> broker_ptr_;
};


#endif	// DESIGNLAB_SYSTEM_MAIN_RESULT_VIEWER_H_