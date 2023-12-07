//! @file application_setting_importer_for_toml.h
//! @brief アプリケーション設定ファイルを読み込むクラス

#ifndef APPLICATION_SETTING_READER_TOML_H_
#define APPLICATION_SETTING_READER_TOML_H_


#include <memory>
#include <string>

#include "application_setting_record.h"
#include "interface_application_setting_importer.h"
#include "toml11_define.h"

//! @class ApplicationSettingImporterForToml
//! @brief toml形式のアプリケーション設定ファイルを読み込むクラス
class ApplicationSettingImporterForToml final : public IApplicationSettingImporter
{
public:

	std::shared_ptr<ApplicationSettingRecord> ReadFileOrUseAndOutputDefault() override;

private:

	//ファイルが存在しなかった場合のためにデフォルトの設定ファイルを出力する
	void OutputDefaultSettingFile();

	const std::string kSettingFileName = u8"settings.toml";
};


#endif	// APPLICATION_SETTING_READER_TOML_H_