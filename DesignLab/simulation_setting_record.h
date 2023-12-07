#ifndef DESIGNLAB_SIMULATION_SETTING_RECORD_H_
#define DESIGNLAB_SIMULATION_SETTING_RECORD_H_

#include <string>

#include "designlab_string_util.h"
#include "toml_serialize_macro.h"


enum class MapCreateMode : int
{
	kForSimulation,
	kFromFile
};

struct SimulationSettingRecord final
{
	MapCreateMode map_create_mode{ MapCreateMode::kForSimulation };

	std::string simulation_map_param_file_name{ "" };

	std::string map_file_name{ "" };
};


DESIGNLAB_TOML11_DESCRIPTION_CLASS(SimulationSettingRecord)
{
	DESIGNLAB_TOML11_FILE_DESCRIPTION("シミュレーションを行う際の設定はこのファイルで設定します．");

	DESIGNLAB_TOML11_ADD_TABLE_DESCRIPTION("Map", "シミュレーションを行うマップについての設定です．");

	DESIGNLAB_TOML11_ADD_DESCRIPTION(map_create_mode, "Map", "マップの作成方法を設定します．(" + ::designlab::string_util::EnumValuesToString<MapCreateMode>("/") + ")");
	DESIGNLAB_TOML11_ADD_DESCRIPTION(simulation_map_param_file_name, "Map", "マップの作成方法がシミュレーション専用マップを出力するモードの場合，マップのパラメータを記述したtomlファイル名を設定します．");
	DESIGNLAB_TOML11_ADD_DESCRIPTION(map_file_name, "Map", "マップの作成方法がファイルから読み込むモードの場合，マップのcsvファイルの名前を設定します．");
};

DESIGNLAB_TOML11_SERIALIZE(
	SimulationSettingRecord,
	map_create_mode, simulation_map_param_file_name, map_file_name
);

#endif //DESIGNLAB_SIMULATION_SETTING_RECORD_H_