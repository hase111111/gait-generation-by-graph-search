#include "application_setting_reader_toml.h"

#include <iostream>
#include <fstream>
#include <filesystem>

#include <magic_enum.hpp>

#include "output_detail.h"


std::shared_ptr<ApplicationSettingRecorder> ApplicationSettingReaderToml::Read()
{
	std::cout << "設定ファイル" << kSettingFileName << "を読み込みます\n\n";

	//ファイルを探す，存在しなかったらデフォルトの設定を出力して終了，fsystemはC++17から，実行できない場合は設定を見直してみてください
	if (!std::filesystem::is_regular_file(kSettingFileName))
	{
		std::cout << "設定ファイルが見つかりませんでした．デフォルトの設定ファイルを出力します．\n";
		OutputDefaultSettingFile();
		return std::make_shared<ApplicationSettingRecorder>();
	}

	std::cout << "設定ファイルが見つかりました．読み込みを開始いたします\n\n";

	//ファイルを読み込む
	toml::value data;

	try
	{
		std::ifstream ifs(kSettingFileName, std::ios::binary);		//バイナリモードで読み込む

		data = toml::parse(ifs, kSettingFileName);					//ファイルをパース(読みこみ&解析)する
	}
	catch (toml::syntax_error err)
	{
		std::cout << "設定ファイルの読み込みに失敗しました．デフォルトの設定ファイルを出力します．\n";
		std::cout << err.what() << std::endl;

		OutputDefaultSettingFile();

		return std::make_shared<ApplicationSettingRecorder>();
	}


	std::cout << "設定ファイルの読み込みに成功しました．ファイルのtitleキーを確認します\n\n";


	if (toml::get<std::string>(data.at(ApplicationSettingTomlKey::kFileTitle.key)) != ApplicationSettingTomlKey::kFileTitleValue)
	{
		//ファイルのタイトルが一致しない場合はデフォルトの設定を出力して終了
		std::cout << "設定ファイルのタイトルが一致しませんでした．デフォルトの設定ファイルを出力します．\n";
		
		OutputDefaultSettingFile();

		return std::make_shared<ApplicationSettingRecorder>();
	}


	std::cout << "設定ファイルのタイトルが一致しました．設定ファイルの読み込みを続行します\n\n";

	std::shared_ptr<ApplicationSettingRecorder> result = std::make_shared<ApplicationSettingRecorder>();

	try
	{
		ReadVersionSetting(data, result);

		ReadBootModeSetting(data, result);

		ReadDisplaySetting(data, result);
	}
	catch (...)
	{
		//設定ファイルの読み込みに失敗した場合はデフォルトの設定を出力して終了
		std::cout << "設定ファイルの読み込みの途中でエラーが発生しました．読み込めなかった設定はデフォルトの値を使用します．\n";
		std::cout << "デフォルトの設定ファイルを出力します．\n";

		OutputDefaultSettingFile();
		
		return std::make_shared<ApplicationSettingRecorder>();
	}


	std::cout << "設定ファイルの読み込みが完了しました．\n";

	return std::move(result);
}



void ApplicationSettingReaderToml::OutputDefaultSettingFile()
{
	const ApplicationSettingRecorder kDefaultSetting;

	std::string res_str;	//出力する文字列


	//ファイルのタイトル
	{
		res_str += u8"# If this file is garbled, the problem is most likely due to character encoding.\n";
		res_str += u8"# This file is written in utf - 8, and can be read by installing VS Code and configuring it to automatically detect the character encoding.\n\n";
		res_str += u8"# This file is written in the TOML format. Just google it and you'll find easy to understand information on the wiki, so try looking it up.\n";
		res_str += u8"# This file is a file for describing program settings.\n";
		res_str += u8"# This file can also be edited with a text editor such as Notepad.\n";
		res_str += u8"# If you want to change the simulation conditions, try changing them from here.\n";
		res_str += u8"# Lines starting with a sharp are comments. They do not affect the program.\n";
		res_str += u8"# Following description is written in Japanese. \n\n";
		res_str += u8"# TOMLという形式で記述しています．wikiでググるだけでもわかりやすい情報が出るので調べてみてください．\n";
		res_str += u8"# このファイルは，プログラムの設定を記述するためのファイルです．\n";
		res_str += u8"# メモ帳などのテキストエディタでもこのファイルは編集することができます．\n";
		res_str += u8"# シミュレーション条件を変更したい場合は，ここから変更を行うようにしてみてください．\n";
		res_str += u8"# このようにシャープで始まる行はコメントです．プログラムに影響を与えないため，メモ代わりに使うことができます．\n";
		res_str += u8"\n\n\n";

		toml::value title_data{
			{ ApplicationSettingTomlKey::kFileTitle.key, ApplicationSettingTomlKey::kFileTitleValue }
		};
		title_data.comments().push_back(ApplicationSettingTomlKey::kFileTitle.description);

		res_str += toml::format(title_data, 0);
		res_str += u8"\n";
	}


	//バージョン
	{
		toml::basic_value<toml::preserve_comments> version_data{
			{
				ApplicationSettingTomlKey::kVersionTable.table_name,
				{
					{ ApplicationSettingTomlKey::kVersionMajor.key, kDefaultSetting.version_major},
					{ ApplicationSettingTomlKey::kVersionMinor.key, kDefaultSetting.version_minor },
					{ ApplicationSettingTomlKey::kVersionPatch.key, kDefaultSetting.version_patch }
				}
			}
		};

		version_data.at(ApplicationSettingTomlKey::kVersionTable.table_name).comments().push_back(ApplicationSettingTomlKey::kVersionTable.description);
		version_data.at(ApplicationSettingTomlKey::kVersionTable.table_name).at(ApplicationSettingTomlKey::kVersionMajor.key).comments().push_back(ApplicationSettingTomlKey::kVersionMajor.description);
		version_data.at(ApplicationSettingTomlKey::kVersionTable.table_name).at(ApplicationSettingTomlKey::kVersionMinor.key).comments().push_back(ApplicationSettingTomlKey::kVersionMinor.description);
		version_data.at(ApplicationSettingTomlKey::kVersionTable.table_name).at(ApplicationSettingTomlKey::kVersionPatch.key).comments().push_back(ApplicationSettingTomlKey::kVersionPatch.description);

		res_str += toml::format(version_data, 0);
	}

	//モード
	{
		toml::basic_value<toml::preserve_comments> mode_data{
			{
				ApplicationSettingTomlKey::kMoveTable.table_name,
				{
					{ ApplicationSettingTomlKey::kAskAboutBootMode.key, kDefaultSetting.ask_about_modes },
					{ ApplicationSettingTomlKey::kDefaultMode.key, magic_enum::enum_name(kDefaultSetting.default_mode) },
					{ ApplicationSettingTomlKey::kDoStepExecution.key, kDefaultSetting.do_step_execution },
					{ ApplicationSettingTomlKey::kDoStepEexcutionEachGait.key, kDefaultSetting.do_step_execution_each_gait}
				}
			}
		};

		mode_data.at(ApplicationSettingTomlKey::kMoveTable.table_name).comments().push_back(ApplicationSettingTomlKey::kMoveTable.description);
		mode_data.at(ApplicationSettingTomlKey::kMoveTable.table_name).at(ApplicationSettingTomlKey::kAskAboutBootMode.key).comments().push_back(ApplicationSettingTomlKey::kAskAboutBootMode.description);
		mode_data.at(ApplicationSettingTomlKey::kMoveTable.table_name).at(ApplicationSettingTomlKey::kDefaultMode.key).comments().push_back(ApplicationSettingTomlKey::kDefaultMode.description);
		mode_data.at(ApplicationSettingTomlKey::kMoveTable.table_name).at(ApplicationSettingTomlKey::kDoStepExecution.key).comments().push_back(ApplicationSettingTomlKey::kDoStepExecution.description);
		mode_data.at(ApplicationSettingTomlKey::kMoveTable.table_name).at(ApplicationSettingTomlKey::kDoStepEexcutionEachGait.key).comments().push_back(ApplicationSettingTomlKey::kDoStepEexcutionEachGait.description);

		res_str += toml::format(mode_data, 0);
	}

	// 表示
	{
		toml::basic_value<toml::preserve_comments> display_data{
			{
				ApplicationSettingTomlKey::kDisplayTable.table_name,
				{
					{ ApplicationSettingTomlKey::kOutputCmd.key, kDefaultSetting.cmd_output},
					{ ApplicationSettingTomlKey::kCmdPermission.key, magic_enum::enum_name(kDefaultSetting.cmd_permission) },
					{ ApplicationSettingTomlKey::kDisplayGui.key, kDefaultSetting.gui_display },
					{ ApplicationSettingTomlKey::kGuiDisplayQuality.key, kDefaultSetting.gui_display_quality },
					{ ApplicationSettingTomlKey::kWindowSizeX.key,kDefaultSetting.window_size_x },
					{ ApplicationSettingTomlKey::kWindowSizeY.key,kDefaultSetting.window_size_y },
					{ ApplicationSettingTomlKey::kWindowFps.key,kDefaultSetting.window_fps }
				}
			}
		};

		display_data.at(ApplicationSettingTomlKey::kDisplayTable.table_name).comments().push_back(ApplicationSettingTomlKey::kDisplayTable.description);
		display_data.at(ApplicationSettingTomlKey::kDisplayTable.table_name).at(ApplicationSettingTomlKey::kOutputCmd.key).comments().push_back(ApplicationSettingTomlKey::kOutputCmd.description);
		display_data.at(ApplicationSettingTomlKey::kDisplayTable.table_name).at(ApplicationSettingTomlKey::kCmdPermission.key).comments().push_back(ApplicationSettingTomlKey::kCmdPermission.description);
		display_data.at(ApplicationSettingTomlKey::kDisplayTable.table_name).at(ApplicationSettingTomlKey::kDisplayGui.key).comments().push_back(ApplicationSettingTomlKey::kDisplayGui.description);
		display_data.at(ApplicationSettingTomlKey::kDisplayTable.table_name).at(ApplicationSettingTomlKey::kGuiDisplayQuality.key).comments().push_back(ApplicationSettingTomlKey::kGuiDisplayQuality.description);
		display_data.at(ApplicationSettingTomlKey::kDisplayTable.table_name).at(ApplicationSettingTomlKey::kWindowSizeX.key).comments().push_back(ApplicationSettingTomlKey::kWindowSizeX.description);
		display_data.at(ApplicationSettingTomlKey::kDisplayTable.table_name).at(ApplicationSettingTomlKey::kWindowSizeY.key).comments().push_back(ApplicationSettingTomlKey::kWindowSizeY.description);
		display_data.at(ApplicationSettingTomlKey::kDisplayTable.table_name).at(ApplicationSettingTomlKey::kWindowFps.key).comments().push_back(ApplicationSettingTomlKey::kWindowFps.description);

		res_str += toml::format(display_data, 0);
	}

	std::ofstream ofs;
	ofs.open(kSettingFileName);

	// ファイルが開けなかったら何もしない
	if (!ofs)
	{
		std::cout << "デフォルトの設定ファイルの出力に失敗しました．\n";
		return;
	}

	ofs.write(res_str.c_str(), res_str.length());	// ファイルに書き込む

	ofs.close();	// ファイルを閉じる

	return;
}


void ApplicationSettingReaderToml::ReadVersionSetting(const toml::value& value, std::shared_ptr<ApplicationSettingRecorder>& recorder)
{
	std::cout << std::endl;

	if (value.contains(ApplicationSettingTomlKey::kVersionTable.table_name))
	{
		std::cout << "〇バージョン設定を読み込みます．\n";

		// バージョン設定を読み込む
		if (value.at(ApplicationSettingTomlKey::kVersionTable.table_name).contains(ApplicationSettingTomlKey::kVersionMajor.key))
		{
			recorder->version_major = (int)value.at(ApplicationSettingTomlKey::kVersionTable.table_name).at(ApplicationSettingTomlKey::kVersionMajor.key).as_integer();
			std::cout << "〇メジャーバージョンを読み込みました．value = " << recorder->version_major << "\n";
		}
		else
		{
			std::cout << "×メジャーバージョンが見つかりませんでした．\n";
		}

		if (value.at(ApplicationSettingTomlKey::kVersionTable.table_name).contains(ApplicationSettingTomlKey::kVersionMinor.key))
		{
			recorder->version_minor = (int)value.at(ApplicationSettingTomlKey::kVersionTable.table_name).at(ApplicationSettingTomlKey::kVersionMinor.key).as_integer();
			std::cout << "〇マイナーバージョンを読み込みました．value = " << recorder->version_minor << "\n";
		}
		else
		{
			std::cout << "×マイナーバージョンが見つかりませんでした．\n";
		}

		if (value.at(ApplicationSettingTomlKey::kVersionTable.table_name).contains(ApplicationSettingTomlKey::kVersionPatch.key))
		{
			recorder->version_patch = (int)value.at(ApplicationSettingTomlKey::kVersionTable.table_name).at(ApplicationSettingTomlKey::kVersionPatch.key).as_integer();
			std::cout << "〇パッチバージョンを読み込みました．value = " << recorder->version_patch << "\n";
		}
		else
		{
			std::cout << "×パッチバージョンが見つかりませんでした．\n";
		}
	}
	else
	{
		std::cout << "×バージョン設定が見つかりませんでした．\n";
	}
}


void ApplicationSettingReaderToml::ReadBootModeSetting(const toml::value& value, std::shared_ptr<ApplicationSettingRecorder>& recorder)
{
	std::cout << std::endl;

	if (value.contains(ApplicationSettingTomlKey::kMoveTable.table_name))
	{
		std::cout << "〇起動モード設定を読み込みます．\n";

		if (value.at(ApplicationSettingTomlKey::kMoveTable.table_name).contains(ApplicationSettingTomlKey::kAskAboutBootMode.key))
		{
			recorder->ask_about_modes = value.at(ApplicationSettingTomlKey::kMoveTable.table_name).at(ApplicationSettingTomlKey::kAskAboutBootMode.key).as_boolean();
			std::cout << "〇起動モード選択の確認フラグを読み込みました．value = " << recorder->ask_about_modes << "\n";
		}
		else
		{
			std::cout << "×起動モード選択の確認フラグが見つかりませんでした．\n";
		}

		if (value.at(ApplicationSettingTomlKey::kMoveTable.table_name).contains(ApplicationSettingTomlKey::kDefaultMode.key))
		{
			std::string read_value = value.at(ApplicationSettingTomlKey::kMoveTable.table_name).at(ApplicationSettingTomlKey::kDefaultMode.key).as_string();
			recorder->default_mode = magic_enum::enum_cast<BootMode>(read_value).value();
			std::cout << "〇デフォルトの起動モードを読み込みました．value = " << magic_enum::enum_name(recorder->default_mode) << "\n";
		}
		else
		{
			std::cout << "×デフォルトの起動モードが見つかりませんでした．\n";
		}

		if (value.at(ApplicationSettingTomlKey::kMoveTable.table_name).contains(ApplicationSettingTomlKey::kDoStepExecution.key))
		{
			recorder->do_step_execution = value.at(ApplicationSettingTomlKey::kMoveTable.table_name).at(ApplicationSettingTomlKey::kDoStepExecution.key).as_boolean();
			std::cout << "〇ステップ実行フラグを読み込みました．value = " << std::boolalpha << recorder->do_step_execution << "\n";
		}
		else
		{
			std::cout << "×ステップ実行フラグが見つかりませんでした．\n";
		}

		if (value.at(ApplicationSettingTomlKey::kMoveTable.table_name).contains(ApplicationSettingTomlKey::kDoStepEexcutionEachGait.key))
		{
			recorder->do_step_execution_each_gait = value.at(ApplicationSettingTomlKey::kMoveTable.table_name).at(ApplicationSettingTomlKey::kDoStepEexcutionEachGait.key).as_boolean();
			std::cout << "〇ステップ実行フラグ(各歩容)を読み込みました．value = " << std::boolalpha << recorder->do_step_execution_each_gait << "\n";
		}
		else
		{
			std::cout << "×ステップ実行フラグ(各歩容)が見つかりませんでした．\n";
		}

	}
	else
	{
		std::cout << "×起動モード設定が見つかりませんでした．\n";
	}
}


void ApplicationSettingReaderToml::ReadDisplaySetting(const toml::value& value, std::shared_ptr<ApplicationSettingRecorder>& recorder)
{
	std::cout << "\n";

	if (value.contains(ApplicationSettingTomlKey::kDisplayTable.table_name))
	{
		std::cout << "〇表示設定を読み込みます．" << "\n";

		if (value.at(ApplicationSettingTomlKey::kDisplayTable.table_name).contains(ApplicationSettingTomlKey::kOutputCmd.key))
		{
			recorder->cmd_output = value.at(ApplicationSettingTomlKey::kDisplayTable.table_name).at(ApplicationSettingTomlKey::kOutputCmd.key).as_boolean();
			std::cout << "〇コマンド出力のフラグを読み込みました．value = " << std::boolalpha << recorder->cmd_output << "\n";
		}
		else
		{
			std::cout << "×コマンド出力のフラグが見つかりませんでした．\n";
		}

		if (value.at(ApplicationSettingTomlKey::kDisplayTable.table_name).contains(ApplicationSettingTomlKey::kCmdPermission.key))
		{
			std::string read_str = value.at(ApplicationSettingTomlKey::kDisplayTable.table_name).at(ApplicationSettingTomlKey::kCmdPermission.key).as_string();
			recorder->cmd_permission = magic_enum::enum_cast<OutputDetail>(read_str).value();
			std::cout << "〇コマンド表示制限の情報を読み込みました．value = " << magic_enum::enum_name(recorder->cmd_permission) << "\n";
		}
		else
		{
			std::cout << "×コマンド表示制限の情報が見つかりませんでした．\n";
		}

		if (value.at(ApplicationSettingTomlKey::kDisplayTable.table_name).contains(ApplicationSettingTomlKey::kDisplayGui.key))
		{
			recorder->gui_display = value.at(ApplicationSettingTomlKey::kDisplayTable.table_name).at(ApplicationSettingTomlKey::kDisplayGui.key).as_boolean();
			std::cout << "〇GUI表示のフラグを読み込みました．value = " << std::boolalpha << recorder->gui_display << "\n";
		}
		else
		{
			std::cout << "×GUI表示のフラグが見つかりませんでした．\n";
		}

		if (value.at(ApplicationSettingTomlKey::kGuiDisplayQuality.table_name).contains(ApplicationSettingTomlKey::kGuiDisplayQuality.key))
		{
			recorder->gui_display_quality = value.at(ApplicationSettingTomlKey::kGuiDisplayQuality.table_name).at(ApplicationSettingTomlKey::kGuiDisplayQuality.key).as_string();
			std::cout << "〇GUI表示品質を読み込みました．value = " << recorder->gui_display_quality << "\n";
		}
		else
		{
			std::cout << "×GUI表示品質が見つかりませんでした．\n";
		}

		if (value.at(ApplicationSettingTomlKey::kDisplayTable.table_name).contains(ApplicationSettingTomlKey::kWindowSizeX.key))
		{
			recorder->window_size_x = (int)value.at(ApplicationSettingTomlKey::kDisplayTable.table_name).at(ApplicationSettingTomlKey::kWindowSizeX.key).as_integer();
			std::cout << "〇ウィンドウのXサイズ(横幅)の値を読み込みました．value = " << recorder->window_size_x << "\n";
		}
		else
		{
			std::cout << "×ウィンドウのXサイズ(横幅)の値が見つかりませんでした．\n";
		}

		if (value.at(ApplicationSettingTomlKey::kDisplayTable.table_name).contains(ApplicationSettingTomlKey::kWindowSizeY.key))
		{
			recorder->window_size_y = (int)value.at(ApplicationSettingTomlKey::kDisplayTable.table_name).at(ApplicationSettingTomlKey::kWindowSizeY.key).as_integer();
			std::cout << "〇ウィンドウのYサイズ(縦幅)の値を読み込みました．value = " << recorder->window_size_y << "\n";
		}
		else
		{
			std::cout << "×ウィンドウのYサイズ(縦幅)の値が見つかりませんでした．\n";
		}

		if (value.at(ApplicationSettingTomlKey::kDisplayTable.table_name).contains(ApplicationSettingTomlKey::kWindowFps.key))
		{
			recorder->window_fps = (int)value.at(ApplicationSettingTomlKey::kDisplayTable.table_name).at(ApplicationSettingTomlKey::kWindowFps.key).as_integer();
			std::cout << "〇ウィンドウのFPSの値を読み込みました．value = " << recorder->window_fps << "\n";
		}
		else
		{
			std::cout << "×ウィンドウのFPSの値が見つかりませんでした．\n";
		}
	}
	else
	{
		std::cout << "×表示設定が見つかりませんでした．\n";
	}
}
