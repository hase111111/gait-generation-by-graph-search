#pragma once

#include <string>

#include "application_setting_recorder.h"
#include "application_setting_key.h"

#define TOML11_PRESERVE_COMMENTS_BY_DEFAULT

//! @class ApplicationSettingReader
//! @date 2023/08/25
//! @author 長谷川
//! @brief アプリケーション設定ファイルを読み込むクラス
class ApplicationSettingReader final
{
public:

	//! @brief 設定ファイルを読み込む
	//! @n 設定ファイルが存在しない場合はデフォルトの設定ファイルを出力する
	//! @param [out] recorder 読み込んだ設定ファイルの内容を格納する
	void read(SApplicationSettingRecorder* recorder);

private:

	//ファイルが存在しなかった場合のためにデフォルトの設定ファイルを出力する
	void outputDefaultSettingFile();

	//改行する
	void outputIndention(std::ofstream& ofs, int indention) const;

	//テーブルを出力する
	void outputTable(std::ofstream& ofs, const SettingTableData& table) const;


	const std::string SETTING_FILE_NAME = "settings.toml";


};



//! @file application_setting_reader.h
//! @date 2023/08/25
//! @author 長谷川
//! @brief アプリケーション設定ファイルを読み込むクラス
//! @n 行数 : @lineinfo