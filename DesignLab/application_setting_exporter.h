//! @file application_setting_exporter.h
//! @brief アプリケーション設定ファイルを出力するクラス．

#ifndef DESIGNLAB_APPLICATION_SETTING_EXPORTER_H_
#define DESIGNLAB_APPLICATION_SETTING_EXPORTER_H_

#include "application_setting_record.h"

#include <string>


//! @class ApplicationSettingExporter
//! @brief アプリケーション設定ファイルを出力するクラス．
class ApplicationSettingExporter final
{
public:

	//! @brief 設定ファイルを出力する．
	//! @param[in] file_path 設定ファイルの出力先．
	//! @param[in] setting_record 設定ファイルの内容．
	void Export(const std::string& file_path, const ApplicationSettingRecord& setting_record) const;
};


#endif	// DESIGNLAB_APPLICATION_SETTING_EXPORTER_H_