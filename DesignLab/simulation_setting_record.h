#ifndef DESIGNLAB_SIMULATION_SETTING_RECORD_H_
#define DESIGNLAB_SIMULATION_SETTING_RECORD_H_

#include <string>

#include "designlab_euler.h"
#include "designlab_string_util.h"
#include "designlab_vector3.h"
#include "toml_serialize_macro.h"


enum class MapCreateMode : int
{
	kForSimulation,
	kFromFile
};

enum class SimulationTargetUpdateMode : int
{
	kForGloabalPathGeneration,
	kOnce
};

enum class SimulationEndCheckType : int
{
	kGoalTape,	//!< ゴールテープに到達したかどうか．
	kPosture,	//!< 姿勢が一定の範囲内に収まっているかどうか．
	kPosition,	//!< 目的の座標に到達したかどうか．
};


struct SimulationSettingRecord final
{
	MapCreateMode map_create_mode{ MapCreateMode::kForSimulation };

	std::string simulation_map_param_file_name{ "simulation_map.toml" };

	std::string map_file_name{ "map_state.csv" };


	SimulationEndCheckType simulation_end_check_type{ SimulationEndCheckType::kGoalTape };

	float goal_tape_position_x{ 1200.0f };

	::designlab::EulerXYZ target_posture{ 0.f,0.f,0.f };

	float target_posture_allowable_error_deg{ 1.0f };

	::designlab::Vector3 target_position{ 0.f,0.f,0.f };

	float target_position_allowable_error{ 50.0f };

	SimulationTargetUpdateMode simulation_target_update_mode{ SimulationTargetUpdateMode::kOnce };

	std::string simulation_target_update_file_name{ "similation_target_update.toml" };
};


DESIGNLAB_TOML11_DESCRIPTION_CLASS(SimulationSettingRecord)
{
	DESIGNLAB_TOML11_FILE_DESCRIPTION("シミュレーションを行う際の設定はこのファイルで設定します．");

	DESIGNLAB_TOML11_ADD_TABLE_DESCRIPTION(
		"Map", "シミュレーションを行うマップについての設定です．",
		"SimulationEndCheck", "シミュレーションの終了条件についての設定です．",
		"TargetUpdate", "シミュレーションの目標の更新方法についての設定です．"
	);

	DESIGNLAB_TOML11_ADD_DESCRIPTION(map_create_mode, "Map", "マップの作成方法を設定します．(" + ::designlab::string_util::EnumValuesToString<MapCreateMode>("/") + ")");
	DESIGNLAB_TOML11_ADD_DESCRIPTION(simulation_map_param_file_name, "Map", "マップの作成方法がシミュレーション専用マップを出力するモードの場合，マップのパラメータを記述したtomlファイル名を設定します．");
	DESIGNLAB_TOML11_ADD_DESCRIPTION(map_file_name, "Map", "マップの作成方法がファイルから読み込むモードの場合，マップのcsvファイルの名前を設定します．");

	DESIGNLAB_TOML11_ADD_DESCRIPTION(simulation_end_check_type, "SimulationEndCheck", "シミュレーションの終了条件の種類を設定します．(" +
		::designlab::string_util::EnumValuesToString<SimulationEndCheckType>("/") + ")");
	DESIGNLAB_TOML11_ADD_DESCRIPTION(goal_tape_position_x, "SimulationEndCheck", "シミュレーションの終了条件がゴールテープに到達したかどうかの場合，ゴールテープのx座標を設定します．[mm]");
	DESIGNLAB_TOML11_ADD_DESCRIPTION(target_posture, "SimulationEndCheck", "シミュレーションの終了条件が姿勢が一定の範囲内に収まっているかどうかの場合，目標の姿勢のx軸周りの角度を設定します．[deg]");
	DESIGNLAB_TOML11_ADD_DESCRIPTION(target_posture_allowable_error_deg, "SimulationEndCheck", "シミュレーションの終了条件が姿勢が一定の範囲内に収まっているかどうかの場合，目標の姿勢の角度の許容誤差を設定します．[deg]");
	DESIGNLAB_TOML11_ADD_DESCRIPTION(target_position, "SimulationEndCheck", "シミュレーションの終了条件が目的の座標に到達したかどうかの場合，目標の座標のx座標を設定します．[mm]");
	DESIGNLAB_TOML11_ADD_DESCRIPTION(target_position_allowable_error, "SimulationEndCheck", "シミュレーションの終了条件が目的の座標に到達したかどうかの場合，目標の座標の許容誤差を設定します．[mm]");

	DESIGNLAB_TOML11_ADD_DESCRIPTION(simulation_target_update_mode, "TargetUpdate", "シミュレーションの目標の更新方法を設定します．(" +
		::designlab::string_util::EnumValuesToString<SimulationTargetUpdateMode>("/") + ")");
	DESIGNLAB_TOML11_ADD_DESCRIPTION(simulation_target_update_file_name, "TargetUpdate", "シミュレーションの目標の更新方法がファイルから読み込むモードの場合，目標の更新方法の設定を記述したtomlファイル名を設定します．");
};

DESIGNLAB_TOML11_SERIALIZE(
	SimulationSettingRecord,
	map_create_mode, simulation_map_param_file_name, map_file_name,
	simulation_end_check_type, goal_tape_position_x, target_posture, target_posture_allowable_error_deg,
	target_position, target_position_allowable_error,
	simulation_target_update_mode, simulation_target_update_file_name
);

#endif //DESIGNLAB_SIMULATION_SETTING_RECORD_H_