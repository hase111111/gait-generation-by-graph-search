#include "simulation_setting_exporter.h"

#include <fstream>
#include <filesystem>
#include <map>

#include "cmdio_util.h"
#include "simulation_setting_importer.h"
#include "toml11_define.h"

namespace dlio = ::designlab::cmdio;


void SimulationSettingExporter::Export(const SimulationSettingRecord& record, std::string file_path) const
{
	toml::basic_value<toml::preserve_comments, std::map> value(record);
	std::string res_str = toml::format(value);	// 設定を文字列に変換

	if (!std::filesystem::exists(SimulationSettingImporter::kFilePath))
	{
		std::filesystem::create_directory(SimulationSettingImporter::kFilePath);
	}

	std::ofstream ofs;
	ofs.open(file_path);

	// ファイルが開けなかったら何もしない
	if (!ofs)
	{
		dlio::Output("設定ファイルの出力に失敗しました．file_path : " + file_path, OutputDetail::kSystem);
		return;
	}

	ofs.write(res_str.c_str(), res_str.length());	// ファイルに書き込む

	ofs.close();	// ファイルを閉じる

	dlio::Output("設定ファイルを出力しました．file_path : " + file_path, OutputDetail::kSystem);
}
