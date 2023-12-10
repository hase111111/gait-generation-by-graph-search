#include "simulation_setting_importer.h"

#include <fstream>
#include <filesystem>

#include "cmdio_util.h"

namespace dlio = ::designlab::cmdio;


const std::string SimulationSettingImporter::kFilePath = "./simulation_condition/";


SimulationSettingRecord SimulationSettingImporter::ImportOrUsDefault() const
{
	dlio::OutputNewLine(1, OutputDetail::kSystem);
	dlio::Output("[" + static_cast<std::string>(typeid(SimulationSettingImporter).name()) + "]", OutputDetail::kSystem);
	dlio::Output("シミュレーションの設定ファイルを読み込みます．", OutputDetail::kSystem);

	//ファイルを探す，存在しなかったらデフォルトの設定を出力して終了，fsystemはC++17から，実行できない場合は設定を見直すこと．
	if (!std::filesystem::is_regular_file(kFilePath + kFileName))
	{
		dlio::Output(kFilePath + kFileName + "が見つかりませんでした．デフォルトの設定を出力し，そのデータを使用します．", OutputDetail::kSystem);
		OutputDefault();
		return SimulationSettingRecord();
	}

	dlio::Output(kFilePath + kFileName + "が見つかりました．パースを開始します．", OutputDetail::kSystem);
	toml::value data;

	try
	{
		std::ifstream ifs(kFilePath + kFileName, std::ios::binary);		//バイナリモードで読み込む

		data = toml::parse(ifs, kFilePath + kFileName);					//ファイルをパース(読みこみ&解析)する
	}
	catch (toml::syntax_error err)
	{
		dlio::Output("設定ファイルのパースに失敗しました．", OutputDetail::kSystem);
		dlio::OutputNewLine(1, OutputDetail::kSystem);
		dlio::Output("<パースに失敗した箇所>", OutputDetail::kSystem);
		dlio::Output(err.what(), OutputDetail::kSystem);
		dlio::OutputNewLine(1, OutputDetail::kSystem);

		OutputDefault();
		return SimulationSettingRecord();
	}

	dlio::Output("設定ファイルのパースに成功しました．データのシリアライズをします．", OutputDetail::kSystem);
	SimulationSettingRecord setting;

	try
	{
		setting = toml::from<SimulationSettingRecord>::from_toml(data);
	}
	catch (...)
	{
		dlio::Output("データのシリアライズに失敗しました．", OutputDetail::kSystem);

		OutputDefault();
		return SimulationSettingRecord();
	}

	dlio::Output("読み込みは正常に完了しました．", OutputDetail::kSystem);

	return setting;
}

void SimulationSettingImporter::OutputDefault() const
{
	const SimulationSettingRecord kDefaultSetting;

	toml::basic_value<toml::preserve_comments, std::map> value(kDefaultSetting);
	std::string res_str = toml::format(value);	// 設定を文字列に変換

	std::ofstream ofs;
	ofs.open(kFilePath + kFileName);

	// ファイルが開けなかったら何もしない
	if (!ofs)
	{
		dlio::Output(kFilePath + kFileName + "を開くことができませんでした．", OutputDetail::kError);
		return;
	}

	ofs.write(res_str.c_str(), res_str.length());	// ファイルに書き込む

	ofs.close();	// ファイルを閉じる

	return;
}