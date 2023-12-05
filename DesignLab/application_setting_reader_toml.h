//! @file application_setting_reader.h
//! @brief アプリケーション設定ファイルを読み込むクラス

#ifndef APPLICATION_SETTING_READER_TOML_H_
#define APPLICATION_SETTING_READER_TOML_H_


#include <memory>
#include <string>

// toml11でコメントを保持するためのマクロ
#define TOML11_PRESERVE_COMMENTS_BY_DEFAULT
#include <toml.hpp>

#include "application_setting_recorder.h"
#include "application_setting_toml_key.h"
#include "interface_application_setting_reader.h"


//! @class ApplicationSettingReaderToml
//! @brief toml形式のアプリケーション設定ファイルを読み込むクラス
class ApplicationSettingReaderToml final : public IApplicationSettingReader
{
public:

	std::shared_ptr<ApplicationSettingRecorder> ReadFileOrUseAndOutputDefault() override;

private:

	//ファイルが存在しなかった場合のためにデフォルトの設定ファイルを出力する
	void OutputDefaultSettingFile();

	//設定ファイルからバージョン情報を読み込む
	void ReadVersionSetting(const toml::value& value, std::shared_ptr<ApplicationSettingRecorder>& recorder);

	//設定ファイルから起動モードの情報を読み込む
	void ReadBootModeSetting(const toml::value& value, std::shared_ptr<ApplicationSettingRecorder>& recorder);

	//設定ファイルからディスプレイ情報を読み込む
	void ReadDisplaySetting(const toml::value& value, std::shared_ptr<ApplicationSettingRecorder>& recorder);

	const std::string kSettingFileName = u8"settings.toml";
};


#endif	// APPLICATION_SETTING_READER_TOML_H_