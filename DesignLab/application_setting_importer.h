//! @file application_setting_importer.h
//! @brief アプリケーション設定ファイルを読み込むクラス．

#ifndef DESIGNLAB_APPLICATION_SETTING_IMPORTER_H_
#define DESIGNLAB_APPLICATION_SETTING_IMPORTER_H_


#include <memory>
#include <string>

#include "application_setting_record.h"
#include "toml11_define.h"

//! @class ApplicationSettingImporter
//! @brief アプリケーション設定ファイルを読み込むクラス．
class ApplicationSettingImporter final
{
public:

	//! @brief 設定ファイルを読み込む．
	//! @n 設定ファイルが存在しない場合はデフォルトの設定ファイルを出力する．
	//! @return ApplicationSettingRecord 設定ファイルの内容．
	ApplicationSettingRecord ImportOrUseDefault() const;

private:

	//! @breif ファイルが存在しなかった場合のためにデフォルトの設定ファイルを出力する
	void OutputDefaultSettingFile() const;

	const std::string kSettingFileName = "./settings.toml";
};


#endif	// DESIGNLAB_APPLICATION_SETTING_IMPORTER_H_