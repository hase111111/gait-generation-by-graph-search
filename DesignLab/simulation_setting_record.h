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

	float target_posture_x_angle{ 0.0f };

	float target_posture_y_angle{ 0.0f };

	float target_posture_z_angle{ 0.0f };

	float target_posture_allowable_error{ 1.0f };

	float target_position_x{ 1200.0f };

	float target_position_y{ 0.0f };

	float target_position_z{ 0.0f };

	float target_position_allowable_error{ 50.0f };


	SimulationTargetUpdateMode similation_target_update_mode{ SimulationTargetUpdateMode::kOnce };

	std::string similation_target_update_file_name{ "similation_target_update.toml" };
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
	DESIGNLAB_TOML11_ADD_DESCRIPTION(target_posture_x_angle, "SimulationEndCheck", "シミュレーションの終了条件が姿勢が一定の範囲内に収まっているかどうかの場合，目標の姿勢のx軸周りの角度を設定します．[deg]");
	DESIGNLAB_TOML11_ADD_DESCRIPTION(target_posture_y_angle, "SimulationEndCheck", "シミュレーションの終了条件が姿勢が一定の範囲内に収まっているかどうかの場合，目標の姿勢のy軸周りの角度を設定します．[deg]");
	DESIGNLAB_TOML11_ADD_DESCRIPTION(target_posture_z_angle, "SimulationEndCheck", "シミュレーションの終了条件が姿勢が一定の範囲内に収まっているかどうかの場合，目標の姿勢のz軸周りの角度を設定します．[deg]");
	DESIGNLAB_TOML11_ADD_DESCRIPTION(target_posture_allowable_error, "SimulationEndCheck", "シミュレーションの終了条件が姿勢が一定の範囲内に収まっているかどうかの場合，目標の姿勢の角度の許容誤差を設定します．[deg]");
	DESIGNLAB_TOML11_ADD_DESCRIPTION(target_position_x, "SimulationEndCheck", "シミュレーションの終了条件が目的の座標に到達したかどうかの場合，目標の座標のx座標を設定します．[mm]");
	DESIGNLAB_TOML11_ADD_DESCRIPTION(target_position_y, "SimulationEndCheck", "シミュレーションの終了条件が目的の座標に到達したかどうかの場合，目標の座標のy座標を設定します．[mm]");
	DESIGNLAB_TOML11_ADD_DESCRIPTION(target_position_z, "SimulationEndCheck", "シミュレーションの終了条件が目的の座標に到達したかどうかの場合，目標の座標のz座標を設定します．[mm]");
	DESIGNLAB_TOML11_ADD_DESCRIPTION(target_position_allowable_error, "SimulationEndCheck", "シミュレーションの終了条件が目的の座標に到達したかどうかの場合，目標の座標の許容誤差を設定します．[mm]");

	DESIGNLAB_TOML11_ADD_DESCRIPTION(similation_target_update_mode, "TargetUpdate", "シミュレーションの目標の更新方法を設定します．(" +
		::designlab::string_util::EnumValuesToString<SimulationTargetUpdateMode>("/") + ")");
	DESIGNLAB_TOML11_ADD_DESCRIPTION(similation_target_update_file_name, "TargetUpdate", "シミュレーションの目標の更新方法がファイルから読み込むモードの場合，目標の更新方法の設定を記述したtomlファイル名を設定します．");
};

DESIGNLAB_TOML11_SERIALIZE(
	SimulationSettingRecord,
	map_create_mode, simulation_map_param_file_name, map_file_name,
	simulation_end_check_type, goal_tape_position_x, target_posture_x_angle, target_posture_y_angle, target_posture_z_angle, target_posture_allowable_error,
	target_position_x, target_position_y, target_position_z, target_position_allowable_error,
	similation_target_update_mode, similation_target_update_file_name
);

#endif //DESIGNLAB_SIMULATION_SETTING_RECORD_H_