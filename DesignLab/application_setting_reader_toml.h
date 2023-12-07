//! @file application_setting_reader.h
//! @brief アプリケーション設定ファイルを読み込むクラス

#ifndef APPLICATION_SETTING_READER_TOML_H_
#define APPLICATION_SETTING_READER_TOML_H_


#include <memory>
#include <string>

#include "application_setting_record.h"
#include "application_setting_toml_key.h"
#include "interface_application_setting_reader.h"
#include "toml11_define.h"

//! @class ApplicationSettingReaderToml
//! @brief toml形式のアプリケーション設定ファイルを読み込むクラス
class ApplicationSettingReaderToml final : public IApplicationSettingReader
{
public:

	std::shared_ptr<ApplicationSettingRecord> ReadFileOrUseAndOutputDefault() override;

private:

	//ファイルが存在しなかった場合のためにデフォルトの設定ファイルを出力する
	void OutputDefaultSettingFile();

	const std::string kSettingFileName = u8"settings.toml";
};


#endif	// APPLICATION_SETTING_READER_TOML_H_