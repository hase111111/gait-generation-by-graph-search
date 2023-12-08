#include "application_setting_importer_for_toml.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <map>

#include <magic_enum.hpp>

#include "output_detail.h"


std::shared_ptr<ApplicationSettingRecord> ApplicationSettingImporterForToml::ReadFileOrUseAndOutputDefault()
{
	std::cout << "[" << typeid(ApplicationSettingImporterForToml).name() << "]\n";
	std::cout << "設定ファイル " << kSettingFileName << " を読み込みます．\n";

	//ファイルを探す，存在しなかったらデフォルトの設定を出力して終了，fsystemはC++17から，実行できない場合は設定を見直してみてください
	if (!std::filesystem::is_regular_file(kSettingFileName))
	{
		std::cout << "設定ファイルが見つかりませんでした．デフォルトの設定ファイルを出力します．\n";
		OutputDefaultSettingFile();
		return std::make_shared<ApplicationSettingRecord>();
	}

	std::cout << "設定ファイルが見つかりました．読み込みを開始します．\n";

	//ファイルを読み込む
	toml::value data;

	try
	{
		std::ifstream ifs(kSettingFileName, std::ios::binary);		//バイナリモードで読み込む

		data = toml::parse(ifs, kSettingFileName);					//ファイルをパース(読みこみ&解析)する
	}
	catch (toml::syntax_error err)
	{
		std::cout << "設定ファイルの読み込みに失敗しました．デフォルトの設定ファイルを出力します．\n";
		std::cout << err.what() << std::endl;

		OutputDefaultSettingFile();

		return std::make_shared<ApplicationSettingRecord>();
	}

	std::cout << "設定ファイルの読み込みに成功しました．パースを開始します．\n";
	ApplicationSettingRecord setting;

	try
	{
		setting = toml::from<ApplicationSettingRecord>::from_toml(data);	//dataをApplicationSettingRecord型に変換する
	}
	catch (...)
	{
		std::cout << "設定ファイルのパースに失敗しました．デフォルトの設定ファイルを出力します．\n";
		OutputDefaultSettingFile();

		return std::make_shared<ApplicationSettingRecord>();
	}


	//dataをApplicationSettingRecord型に変換する
	std::shared_ptr<ApplicationSettingRecord> result;

	result = std::make_shared<ApplicationSettingRecord>(setting);

	std::cout << "設定ファイルの読み込みが完了しました．\n";

	return std::move(result);
}



void ApplicationSettingImporterForToml::OutputDefaultSettingFile()
{
	const ApplicationSettingRecord kDefaultSetting;

	toml::basic_value<toml::preserve_comments, std::map> value(kDefaultSetting);
	std::string res_str = toml::format(value);	// 設定を文字列に変換

	std::ofstream ofs;
	ofs.open(kSettingFileName);

	// ファイルが開けなかったら何もしない
	if (!ofs)
	{
		std::cout << "デフォルトの設定ファイルの出力に失敗しました．\n";
		return;
	}

	ofs.write(res_str.c_str(), res_str.length());	// ファイルに書き込む

	ofs.close();	// ファイルを閉じる

	std::cout << "デフォルトの設定ファイルを出力しました．\n";

	return;
}