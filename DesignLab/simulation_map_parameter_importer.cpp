#include "simulation_map_parameter_importer.h"

#include <fstream>
#include <filesystem>

#include "cmdio_util.h"
#include "simulation_setting_importer.h"

namespace dlio = ::designlab::cmdio;


SimulationMapParameter SimulationMapParameterImporter::ImportOrUseAndOutputDefault(const std::string& file_path) const
{
	dlio::Output("シミュレーション用マップの設定ファイルを読み込みます．", OutputDetail::kInfo);

	//ファイルを探す，存在しなかったらデフォルトの設定を出力して終了，fsystemはC++17から，実行できない場合は設定を見直してみてください
	if (!std::filesystem::is_regular_file(SimulationSettingImporter::kFilePath + file_path))
	{
		dlio::Output(SimulationSettingImporter::kFilePath + file_path + "が見つかりませんでした．デフォルトの設定を出力し，そのデータを使用します．", OutputDetail::kWarning);
		AskOutputDefault(SimulationSettingImporter::kFilePath + file_path);
		return SimulationMapParameter();
	}

	//ファイルを開く
	toml::value data;

	try
	{
		std::ifstream ifs(SimulationSettingImporter::kFilePath + file_path, std::ios::binary);		//バイナリモードで読み込む

		data = toml::parse(ifs, SimulationSettingImporter::kFilePath + file_path);					//ファイルをパース(読みこみ&解析)する
	}
	catch (toml::syntax_error err)
	{
		dlio::Output("シミュレーション用マップの設定ファイルのパースに失敗しました．デフォルトのファイルを出力し，そのデータを使用します．", OutputDetail::kError);
		dlio::Output(err.what(), OutputDetail::kError);

		AskOutputDefault(SimulationSettingImporter::kFilePath + file_path);
		return SimulationMapParameter();
	}

	//パースしたデータをSimulationMapParameterに変換
	SimulationMapParameter setting;

	try
	{
		setting = toml::from<SimulationMapParameter>::from_toml(data);
	}
	catch (toml::type_error err)
	{
		dlio::Output("シミュレーション用マップの設定ファイルのデータの変換に失敗しました．デフォルトのファイルを出力し，そのデータを使用します．", OutputDetail::kError);
		dlio::Output(err.what(), OutputDetail::kError);

		AskOutputDefault(SimulationSettingImporter::kFilePath + file_path);
		return SimulationMapParameter();
	}

	dlio::Output("シミュレーションの設定ファイルを読み込みました．", OutputDetail::kInfo);

	return setting;
}

void SimulationMapParameterImporter::AskOutputDefault(const std::string& file_path) const
{
	if (dlio::InputYesNo(file_path + "にデフォルトのファイルを出力しますか?"))
	{
		const SimulationMapParameter kDefaultSetting;

		toml::basic_value<toml::preserve_comments, std::map> value(kDefaultSetting);
		std::string res_str = toml::format(value);	// 設定を文字列に変換

		std::ofstream ofs;
		ofs.open(file_path);

		// ファイルが開けなかったら何もしない
		if (!ofs)
		{
			dlio::Output(file_path + "を開くことができませんでした．", OutputDetail::kError);
			return;
		}

		ofs.write(res_str.c_str(), res_str.length());	// ファイルに書き込む

		ofs.close();	// ファイルを閉じる
	}
}
