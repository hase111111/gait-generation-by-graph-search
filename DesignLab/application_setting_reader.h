//! @file application_setting_reader.h
//! @brief アプリケーション設定ファイルを読み込むクラス

#ifndef APPLICATION_SETTING_READER_H_
#define APPLICATION_SETTING_READER_H_


#include <memory>
#include <string>

#include "toml.hpp"

#include "application_setting_recorder.h"
#include "application_setting_key.h"


// toml11でコメントを保持するためのマクロ
#define TOML11_PRESERVE_COMMENTS_BY_DEFAULT


//! @class ApplicationSettingReader
//! @brief アプリケーション設定ファイルを読み込むクラス

class ApplicationSettingReader final
{
public:

	//! @brief 設定ファイルを読み込む
	//! @n 設定ファイルが存在しない場合はデフォルトの設定ファイルを出力する
	//! @return std::shared_ptr<SApplicationSettingRecorder> 設定ファイルの内容
	std::shared_ptr<SApplicationSettingRecorder> read();

private:

	//ファイルが存在しなかった場合のためにデフォルトの設定ファイルを出力する
	void outputDefaultSettingFile();

	//設定ファイルからバージョン情報を読み込む
	void readVersionSetting(const toml::value& value, std::shared_ptr<SApplicationSettingRecorder>& recorder);

	//設定ファイルから起動モードの情報を読み込む
	void readBootModeSetting(const toml::value& value, std::shared_ptr<SApplicationSettingRecorder>& recorder);

	//設定ファイルからディスプレイ情報を読み込む
	void readDisplaySetting(const toml::value& value, std::shared_ptr<SApplicationSettingRecorder>& recorder);

	const std::string SETTING_FILE_NAME = u8"settings.toml";


};


#endif //APPLICATION_SETTING_READER_H_