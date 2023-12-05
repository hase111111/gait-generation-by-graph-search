//! @file application_setting_recorder.h
//! @brief アプリの設定を記録する構造体


#ifndef DESIGNLAB_APPLICATION_SETTING_RECORDER_H_
#define DESIGNLAB_APPLICATION_SETTING_RECORDER_H_

#include <map>
#include <string>


#include "application_setting_toml_key.h"
#include "boot_mode.h"
#include "display_quality.h"
#include "output_detail.h"


//! @struct ApplicationSettingRecorder
//! @brief アプリの設定を記録する構造体．
struct ApplicationSettingRecorder final
{
	const std::string kSettingFileTitle = ApplicationSettingTomlKey::kFileTitleValue;		//!< 設定ファイルのタイトル

	int version_major{ 0 };	//!< バージョン番号(メジャー)
	int version_minor{ 5 };	//!< バージョン番号(マイナー)
	int version_patch{ 0 };	//!< バージョン番号(パッチ)

	bool ask_about_modes{ true };					//!< 起動時にモード選択の確認をするかどうか
	BootMode default_mode{ BootMode::kSimulation };	//!< デフォルトの起動モード
	bool do_step_execution_each_simulation{ true };	//!< 1シミュレーションごとにステップ実行をするかどうか
	bool do_step_execution_each_gait{ false };		//!< 1動作ごとにステップ実行をするかどうか

	bool do_cmd_output{ true };										//!< コマンドラインに出力するかどうか
	OutputDetail cmd_output_detail{ OutputDetail::kDebug };			//!< コマンドラインに出力する際，どこまで許可するか
	bool do_gui_display{ true };									//!< GUIを表示するかどうか
	DisplayQuality gui_display_quality{ DisplayQuality::kMedium };	//!< GUIを表示する際，どこまで許可するか
	int window_size_x{ 1280 };										//!< グラフィカルウィンドウの横幅
	int window_size_y{ 720 };										//!< グラフィカルウィンドウの縦幅
	int window_fps{ 60 }; 											//!< グラフィカルウィンドウのFPS	
};


#endif	// DESIGNLAB_APPLICATION_SETTING_RECORDER_H_