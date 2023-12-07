//! @file application_setting_record.h
//! @brief アプリの設定を記録する構造体


#ifndef DESIGNLAB_APPLICATION_SETTING_RECORD_H_
#define DESIGNLAB_APPLICATION_SETTING_RECORD_H_

#include <map>
#include <string>


#include "application_setting_toml_key.h"
#include "boot_mode.h"
#include "display_quality.h"
#include "output_detail.h"

#include "toml_serialize_macro.h"

//! @struct ApplicationSettingRecord
//! @brief アプリの設定を記録する構造体．
struct ApplicationSettingRecord final
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


DESIGNLAB_TOML11_DESCRIPTION_CLASS(ApplicationSettingRecord)
{
	const std::vector<std::string> kFileDiscription{
		"If this file is garbled, the problem is most likely due to character encoding.",
		"This file is written in utf - 8, and can be read by installing VS Code and configuring it to automatically detect the character encoding.",
		"This file is written in the TOML format. Just google it and you'll find easy to understand information on the wiki, so try looking it up.",
		"This file is a file for describing program settings.",
		"This file can also be edited with a text editor such as Notepad.",
		"If you want to change the simulation conditions, try changing them from here."
		"Lines starting with a sharp are comments. They do not affect the program."
		"Following description is written in Japanese. ",
		"",
		"このファイルが文字化けしている場合は，文字コードの問題が考えられます．",
		"このファイルはutf-8で記述されており，VS Codeをインストールし，文字コードを自動で検出するように設定することで読み込むことができます．",
		"このファイルはTOML形式で記述されています．ググればwikiに分かりやすい情報が載っているので，調べてみてください．",
		"このファイルはプログラムの設定を記述するファイルです．",
		"このファイルはメモ帳などのテキストエディタでも編集することができます．",
		"シミュレーション条件を変更したい場合は，ここから変更してみてください．",
		"シャープから始まる行はコメントです．プログラムに影響を与えることはありません．",
	};

	DESIGNLAB_TOML11_FILE_DESCRIPTION_MULTI_LINE(kFileDiscription);

	DESIGNLAB_TOML11_ADD_TABLE_DESCRIPTION(
		"1-Verion", "ver major.minor.patch の順で記述されます．特に使う予定はないデータだけれど一応用意しておく．",
		"2-Mode", "プログラムの起動モードの設定です",
		"3-Output", "プログラムの出力の設定です"
	);

	DESIGNLAB_TOML11_ADD_NO_DESCRIPTION(version_major, "1-Verion");
	DESIGNLAB_TOML11_ADD_NO_DESCRIPTION(version_minor, "1-Verion");
	DESIGNLAB_TOML11_ADD_NO_DESCRIPTION(version_patch, "1-Verion");

	DESIGNLAB_TOML11_ADD_DESCRIPTION(ask_about_modes, "2-Mode", "起動時に実行モードについて質問をするようにします (true/false)");
	DESIGNLAB_TOML11_ADD_DESCRIPTION(default_mode, "2-Mode", "起動時のデフォルトの実行モードを設定します (simulation/real)");
	DESIGNLAB_TOML11_ADD_DESCRIPTION(do_step_execution_each_simulation, "2-Mode", "1シミュレーションごとにステップ実行をするかどうかを設定します (true/false)");
	DESIGNLAB_TOML11_ADD_DESCRIPTION(do_step_execution_each_gait, "2-Mode", "1動作ごとにステップ実行をするかどうかを設定します (true/false)");

	DESIGNLAB_TOML11_ADD_DESCRIPTION(do_cmd_output, "3-Output", "コマンドラインに出力するかどうかを設定します (true/false)");
	DESIGNLAB_TOML11_ADD_DESCRIPTION(cmd_output_detail, "3-Output", "コマンドラインに出力する際，どこまで許可するかを設定します (debug/info/warning/error)");
	DESIGNLAB_TOML11_ADD_DESCRIPTION(do_gui_display, "3-Output", "GUIを表示するかどうかを設定します (true/false)");
	DESIGNLAB_TOML11_ADD_DESCRIPTION(gui_display_quality, "3-Output", "GUIを表示する際，どこまで許可するかを設定します (low/medium/high)");
	DESIGNLAB_TOML11_ADD_DESCRIPTION(window_size_x, "3-Output", "グラフィカルウィンドウの横幅を設定します (int)");
	DESIGNLAB_TOML11_ADD_DESCRIPTION(window_size_y, "3-Output", "グラフィカルウィンドウの縦幅を設定します (int)");
	DESIGNLAB_TOML11_ADD_DESCRIPTION(window_fps, "3-Output", "グラフィカルウィンドウのFPSを設定します (int)");
};

DESIGNLAB_TOML11_SERIALIZE(
	ApplicationSettingRecord,
	version_major, version_minor, version_patch,
	ask_about_modes, default_mode, do_step_execution_each_simulation, do_step_execution_each_gait,
	do_cmd_output, cmd_output_detail, do_gui_display, gui_display_quality, window_size_x, window_size_y, window_fps
);


#endif	// DESIGNLAB_APPLICATION_SETTING_RECORD_H_