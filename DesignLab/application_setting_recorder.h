#pragma once

#include <string>

#include "output_detail.h"
#include "boot_mode.h"
#include "map_creator.h"
#include "application_setting_key.h"


//! @struct SApplicationSettingRecorder
//! @date 2023/08/25
//! @author 長谷川
//! @brief アプリの設定を記録する構造体
struct SApplicationSettingRecorder
{
	const std::string SETTING_FILE_TITLE = ApplicationSettingKey::FILE_TITLE_VALUE;		//!< 設定ファイルのタイトル

	int version_major = 0;				//!< バージョン番号(メジャー)
	int version_minor = 5;				//!< バージョン番号(マイナー)
	int version_patch = 0;				//!< バージョン番号(パッチ)


	bool ask_about_modes = true;					//!< 起動時にモード選択の確認をするかどうか
	EBootMode default_mode = EBootMode::SIMULATION;	//!< デフォルトの起動モード
	bool do_step_execution = true;					//!< 1シミュレーションごとにステップ実行をするかどうか
	bool do_step_execution_each_gait = false;		//!< 1動作ごとにステップ実行をするかどうか


	bool cmd_output = true;								//!< コマンドラインに出力するかどうか
	OutputDetail cmd_permission = OutputDetail::kDebug;	//!< コマンドラインに出力する際，どこまで許可するか
	bool gui_display = true;							//!< GUIを表示するかどうか
	std::string gui_display_quality = "high";			//!< GUIを表示する際，どこまで許可するか
	int window_size_x = 1280;							//!< グラフィカルウィンドウの横幅
	int window_size_y = 720;							//!< グラフィカルウィンドウの縦幅
	int window_fps = 60; 								//!< グラフィカルウィンドウのFPS	


	EMapCreateMode map_create_mode = EMapCreateMode::FLAT;	//!< マップ生成モード
	int map_create_option = 0;								//!< マップ生成オプション
	bool do_output_map = true;								//!< マップを出力するかどうか
	int map_hole_rate = 40;									//!< マップの穴の割合
	float map_step_height = 500.0f;							//!< マップの段差の高さ
	float map_step_length = -120.0f;						//!< マップの段差の長さ
	float map_slope_angle_deg = 10.0f;						//!< マップの坂の角度
	float map_tilt_angle_deg = 5.0f;						//!< マップの傾きの角度
	float rough_max_dif = 30.0f;							//!< マップの粗さの最大値
	float rough_min_dif = -30.0f;							//!< マップの粗さの最小値

};

