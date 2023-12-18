//! @file simulation_setting_record.h
//! @brief シミュレーションの設定を記述する構造体を定義します．

#ifndef DESIGNLAB_SIMULATION_SETTING_RECORD_H_
#define DESIGNLAB_SIMULATION_SETTING_RECORD_H_

#include <string>

#include "designlab_euler.h"
#include "designlab_string_util.h"
#include "designlab_vector3.h"
#include "toml_serialize_macro.h"


namespace designlab::enums
{

enum class MapCreateMode : int
{
	kForSimulation,
	kFromFile
};

enum class RobotOperateMode : int
{
	kForGloabalPathGenerate,
	kFixed,
	kForPath
};

enum class SimulationEndCheckMode : int
{
	kGoalTape,	//!< ゴールテープに到達したかどうか．
	kPosture,	//!< 姿勢が一定の範囲内に収まっているかどうか．
	kPosition,	//!< 目的の座標に到達したかどうか．
};

} // namespace designlab::enums


namespace designlab
{

struct SimulationSettingRecord final
{
	enums::MapCreateMode map_create_mode{ enums::MapCreateMode::kForSimulation };

	std::string simulation_map_param_file_name{ "simulation_map.toml" };

	std::string map_file_name{ "map_state.csv" };


	enums::SimulationEndCheckMode end_check_mode{ enums::SimulationEndCheckMode::kGoalTape };

	float goal_tape_position_x{ 1200.0f };

	EulerXYZ target_posture{ 0.f,0.f,0.f };

	float target_posture_allowable_error_deg{ 1.0f };

	Vector3 target_position{ 0.f,0.f,0.f };

	float target_position_allowable_error{ 50.0f };


	enums::RobotOperateMode operate_mode{ enums::RobotOperateMode::kFixed };

	std::string fixed_operate_file_name{ "robot_operator.toml" };

	std::vector<Vector3> path_points{ {0,0,0},{1000,0,0} };
};


DESIGNLAB_TOML11_DESCRIPTION_CLASS(SimulationSettingRecord)
{
	DESIGNLAB_TOML11_FILE_ADD_DESCRIPTION("シミュレーションを行う際の設定はこのファイルで設定します．");

	DESIGNLAB_TOML11_TABLE_ADD_DESCRIPTION(
		"Map", "マップについての設定です．",
		"EndChecker", "終了条件についての設定です．",
		"RobotOperator", "ロボットの操作方法の設定です．"
	);

	DESIGNLAB_TOML11_VARIABLE_ADD_DESCRIPTION(map_create_mode, "Map", "マップの作成方法を設定します．( \"" +
											  string_util::EnumValuesToString<enums::MapCreateMode>("\" / \"") + "\" )");
	DESIGNLAB_TOML11_VARIABLE_ADD_DESCRIPTION(simulation_map_param_file_name, "Map",
											  "マップの作成方法がシミュレーション専用マップを出力するモード ( " +
											  string_util::EnumToStringRemoveTopK(enums::MapCreateMode::kForSimulation) +
											  " ) の場合，マップのパラメータを記述したtomlファイルの名前を設定します．");
	DESIGNLAB_TOML11_VARIABLE_ADD_DESCRIPTION(map_file_name, "Map", "マップの作成方法がファイルから読み込むモード ( " +
											  string_util::EnumToStringRemoveTopK(enums::MapCreateMode::kFromFile) +
											  " ) の場合，マップのcsvファイルの名前を設定します．");

	DESIGNLAB_TOML11_VARIABLE_ADD_DESCRIPTION(end_check_mode, "EndChecker", "どのような方法で終了させるかを設定します．( \"" +
											  string_util::EnumValuesToString<enums::SimulationEndCheckMode>("\" / \"") + "\" )");
	DESIGNLAB_TOML11_VARIABLE_ADD_DESCRIPTION(goal_tape_position_x, "EndChecker", "終了条件がゴールテープに到達したか ( " +
											  string_util::EnumToStringRemoveTopK(enums::SimulationEndCheckMode::kGoalTape) +
											  " ) の場合，ゴールテープのx座標を設定します．[mm]");
	DESIGNLAB_TOML11_VARIABLE_ADD_DESCRIPTION(target_posture, "EndChecker", "終了条件が目標姿勢となったか ( " +
											  string_util::EnumToStringRemoveTopK(enums::SimulationEndCheckMode::kPosture) +
											  " ) の場合，目標の姿勢(xyzオイラー角)を設定します．[deg]");
	DESIGNLAB_TOML11_VARIABLE_ADD_DESCRIPTION(target_posture_allowable_error_deg, "EndChecker", "終了条件が目標姿勢となったか ( " +
											  string_util::EnumToStringRemoveTopK(enums::SimulationEndCheckMode::kPosture) +
											  " ) の場合，目標の姿勢の角度の許容誤差を設定します．[deg]");
	DESIGNLAB_TOML11_VARIABLE_ADD_DESCRIPTION(target_position, "EndChecker", "終了条件が目的の座標に到達したか ( " +
											  string_util::EnumToStringRemoveTopK(enums::SimulationEndCheckMode::kPosition) +
											  " ) の場合，目標の座標を設定します．[mm]");
	DESIGNLAB_TOML11_VARIABLE_ADD_DESCRIPTION(target_position_allowable_error, "EndChecker", "終了条件が目的の座標に到達したか ( " +
											  string_util::EnumToStringRemoveTopK(enums::SimulationEndCheckMode::kPosition) +
											  " ) の場合，目標の座標の許容誤差を設定します．[mm]");

	DESIGNLAB_TOML11_VARIABLE_ADD_DESCRIPTION(operate_mode, "RobotOperator", "ロボットの操作方法を設定します．( \"" +
											  string_util::EnumValuesToString<enums::RobotOperateMode>("\" / \"") + "\" )");
	DESIGNLAB_TOML11_VARIABLE_ADD_DESCRIPTION(fixed_operate_file_name, "RobotOperator",
											  "ロボットの操作方法が固定 ( " +
											  string_util::EnumToStringRemoveTopK(enums::RobotOperateMode::kFixed) +
											  " ) の場合，その操作方法を指定するファイルを設定します．");
	DESIGNLAB_TOML11_VARIABLE_ADD_DESCRIPTION(path_points, "RobotOperator", "ロボットの操作方法がパス ( " +
											  string_util::EnumToStringRemoveTopK(enums::RobotOperateMode::kForPath) +
											  " ) の場合，そのパスを設定します．");
};

} // namespace designlab


DESIGNLAB_TOML11_SERIALIZE(
	designlab::SimulationSettingRecord,
	map_create_mode, simulation_map_param_file_name, map_file_name,
	end_check_mode, goal_tape_position_x, target_posture, target_posture_allowable_error_deg,
	target_position, target_position_allowable_error,
	operate_mode, fixed_operate_file_name, path_points
);

#endif //DESIGNLAB_SIMULATION_SETTING_RECORD_H_