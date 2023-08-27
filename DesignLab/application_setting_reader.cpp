#include "application_setting_reader.h"

#include <iostream>
#include <fstream>
#include <filesystem>


void ApplicationSettingReader::read(SApplicationSettingRecorder* recorder)
{
	std::cout << "[" << __func__ << "]" << "設定ファイル" << SETTING_FILE_NAME << "を読み込みます" << std::endl;
	std::cout << std::endl;


	//ファイルを探す，存在しなかったらデフォルトの設定を出力して終了，fsystemはC++17から，実行できない場合は設定を見直してみてください
	if (!std::filesystem::is_regular_file(SETTING_FILE_NAME))
	{
		std::cout << "設定ファイルが見つかりませんでした．デフォルトの設定ファイルを出力します．" << std::endl;
		outputDefaultSettingFile();
		return;
	}

	std::cout << "設定ファイルが見つかりました．読み込みを開始いたします" << std::endl;
	std::cout << std::endl;


	//ファイルを読み込む
	toml::value data;

	try
	{
		std::ifstream ifs(SETTING_FILE_NAME, std::ios::binary);		//バイナリモードで読み込む

		data = toml::parse(ifs, SETTING_FILE_NAME);					//ファイルをパース(読みこみ)する
	}
	catch (toml::syntax_error e)
	{
		std::cout << "設定ファイルの読み込みに失敗しました．デフォルトの設定ファイルを出力します．" << std::endl;
		std::cout << e.what() << std::endl;
		outputDefaultSettingFile();
		return;
	}


	std::cout << "設定ファイルの読み込みに成功しました．ファイルのtitleキーを確認します" << std::endl;
	std::cout << std::endl;


	if (toml::get<std::string>(data.at(ApplicationSettingKey::FILE_TITLE.key)) != ApplicationSettingKey::FILE_TITLE_VALUE)
	{
		//ファイルのタイトルが一致しない場合はデフォルトの設定を出力して終了
		std::cout << "設定ファイルのタイトルが一致しませんでした．デフォルトの設定ファイルを出力します．" << std::endl;
		outputDefaultSettingFile();
		return;
	}


	std::cout << "設定ファイルのタイトルが一致しました．設定ファイルの読み込みを続行します" << std::endl;
	std::cout << std::endl;


	try
	{
		readVersionSetting(data, recorder);

		readBootModeSetting(data, recorder);

		readDisplaySetting(data, recorder);
	}
	catch (...)
	{
		//設定ファイルの読み込みに失敗した場合はデフォルトの設定を出力して終了
		std::cout << "設定ファイルの読み込みの途中でエラーが発生しました．読み込めなかった設定はデフォルトの値を使用します．" << std::endl;
		std::cout << "デフォルトの設定ファイルを出力します．" << std::endl;
		outputDefaultSettingFile();
		return;
	}


	std::cout << std::endl;
	std::cout << "設定ファイルの読み込みが完了しました．" << std::endl;
}



void ApplicationSettingReader::outputDefaultSettingFile()
{
	const SApplicationSettingRecorder kDefaultSetting;

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
			{ ApplicationSettingKey::FILE_TITLE.key, ApplicationSettingKey::FILE_TITLE_VALUE }
		};
		title_data.comments().push_back(ApplicationSettingKey::FILE_TITLE.description);

		res_str += toml::format(title_data, 0);
		res_str += u8"\n";
	}


	//バージョン
	{
		toml::basic_value<toml::preserve_comments> version_data{
			{
				ApplicationSettingKey::VERSION_TABLE.table_name,
				{
					{ ApplicationSettingKey::VERSION_MAJOR.key, kDefaultSetting.version_major},
					{ ApplicationSettingKey::VERSION_MINOR.key, kDefaultSetting.version_minor },
					{ ApplicationSettingKey::VERSION_PATCH.key, kDefaultSetting.version_patch }
				}
			}
		};

		version_data.at(ApplicationSettingKey::VERSION_TABLE.table_name).comments().push_back(ApplicationSettingKey::VERSION_TABLE.description);
		version_data.at(ApplicationSettingKey::VERSION_TABLE.table_name).at(ApplicationSettingKey::VERSION_MAJOR.key).comments().push_back(ApplicationSettingKey::VERSION_MAJOR.description);
		version_data.at(ApplicationSettingKey::VERSION_TABLE.table_name).at(ApplicationSettingKey::VERSION_MINOR.key).comments().push_back(ApplicationSettingKey::VERSION_MINOR.description);
		version_data.at(ApplicationSettingKey::VERSION_TABLE.table_name).at(ApplicationSettingKey::VERSION_PATCH.key).comments().push_back(ApplicationSettingKey::VERSION_PATCH.description);

		res_str += toml::format(version_data, 0);
	}

	//モード
	{
		toml::basic_value<toml::preserve_comments> mode_data{
			{
				ApplicationSettingKey::MODE_TABLE.table_name,
				{
					{ ApplicationSettingKey::ASK_ABOUT_MODES.key, kDefaultSetting.ask_about_modes },
					{ ApplicationSettingKey::DEFAULT_MODE.key, std::to_string(kDefaultSetting.default_mode) },
					{ ApplicationSettingKey::DO_STEP_EXECUTION.key, kDefaultSetting.do_step_execution },
					{ApplicationSettingKey::DO_STEP_EXECUTION_EACH_GAIT.key, kDefaultSetting.do_step_execution_each_gait}
				}
			}
		};

		mode_data.at(ApplicationSettingKey::MODE_TABLE.table_name).comments().push_back(ApplicationSettingKey::MODE_TABLE.description);
		mode_data.at(ApplicationSettingKey::MODE_TABLE.table_name).at(ApplicationSettingKey::ASK_ABOUT_MODES.key).comments().push_back(ApplicationSettingKey::ASK_ABOUT_MODES.description);
		mode_data.at(ApplicationSettingKey::MODE_TABLE.table_name).at(ApplicationSettingKey::DEFAULT_MODE.key).comments().push_back(ApplicationSettingKey::DEFAULT_MODE.description);
		mode_data.at(ApplicationSettingKey::MODE_TABLE.table_name).at(ApplicationSettingKey::DO_STEP_EXECUTION.key).comments().push_back(ApplicationSettingKey::DO_STEP_EXECUTION.description);
		mode_data.at(ApplicationSettingKey::MODE_TABLE.table_name).at(ApplicationSettingKey::DO_STEP_EXECUTION_EACH_GAIT.key).comments().push_back(ApplicationSettingKey::DO_STEP_EXECUTION_EACH_GAIT.description);

		res_str += toml::format(mode_data, 0);
	}

	// 表示
	{
		toml::basic_value<toml::preserve_comments> display_data{
			{
				ApplicationSettingKey::DISPLAY_TABLE.table_name,
				{
					{ ApplicationSettingKey::CMD_OUTPUT.key, kDefaultSetting.cmd_output},
					{ ApplicationSettingKey::CMD_PERMISSION.key, std::to_string(kDefaultSetting.cmd_permission) },
					{ ApplicationSettingKey::GUI_DISPLAY.key, kDefaultSetting.gui_display },
					{ ApplicationSettingKey::GUI_DISPLAY_QUALITY.key, kDefaultSetting.gui_display_quality },
					{ ApplicationSettingKey::WINDOW_SIZE_X.key,kDefaultSetting.window_size_x },
					{ ApplicationSettingKey::WINDOW_SIZE_Y.key,kDefaultSetting.window_size_y },
					{ ApplicationSettingKey::WINDOW_FPS.key,kDefaultSetting.window_fps }
				}
			}
		};

		display_data.at(ApplicationSettingKey::DISPLAY_TABLE.table_name).comments().push_back(ApplicationSettingKey::DISPLAY_TABLE.description);
		display_data.at(ApplicationSettingKey::DISPLAY_TABLE.table_name).at(ApplicationSettingKey::CMD_OUTPUT.key).comments().push_back(ApplicationSettingKey::CMD_OUTPUT.description);
		display_data.at(ApplicationSettingKey::DISPLAY_TABLE.table_name).at(ApplicationSettingKey::CMD_PERMISSION.key).comments().push_back(ApplicationSettingKey::CMD_PERMISSION.description);
		display_data.at(ApplicationSettingKey::DISPLAY_TABLE.table_name).at(ApplicationSettingKey::GUI_DISPLAY.key).comments().push_back(ApplicationSettingKey::GUI_DISPLAY.description);
		display_data.at(ApplicationSettingKey::DISPLAY_TABLE.table_name).at(ApplicationSettingKey::GUI_DISPLAY_QUALITY.key).comments().push_back(ApplicationSettingKey::GUI_DISPLAY_QUALITY.description);
		display_data.at(ApplicationSettingKey::DISPLAY_TABLE.table_name).at(ApplicationSettingKey::WINDOW_SIZE_X.key).comments().push_back(ApplicationSettingKey::WINDOW_SIZE_X.description);
		display_data.at(ApplicationSettingKey::DISPLAY_TABLE.table_name).at(ApplicationSettingKey::WINDOW_SIZE_Y.key).comments().push_back(ApplicationSettingKey::WINDOW_SIZE_Y.description);
		display_data.at(ApplicationSettingKey::DISPLAY_TABLE.table_name).at(ApplicationSettingKey::WINDOW_FPS.key).comments().push_back(ApplicationSettingKey::WINDOW_FPS.description);

		res_str += toml::format(display_data, 0);
	}

	std::ofstream ofs;
	ofs.open(SETTING_FILE_NAME);

	// ファイルが開けなかったら何もしない
	if (!ofs)
	{
		std::cout << "デフォルトの設定ファイルの出力に失敗しました．" << std::endl;
		return;
	}

	ofs.write(res_str.c_str(), res_str.length());	// ファイルに書き込む

	ofs.close();	// ファイルを閉じる

	return;
}


void ApplicationSettingReader::readVersionSetting(const toml::value& value, SApplicationSettingRecorder* recorder)
{
	std::cout << std::endl;

	if (value.contains(ApplicationSettingKey::VERSION_TABLE.table_name))
	{
		std::cout << "〇バージョン設定を読み込みます．" << std::endl;

		// バージョン設定を読み込む
		if (value.at(ApplicationSettingKey::VERSION_TABLE.table_name).contains(ApplicationSettingKey::VERSION_MAJOR.key))
		{
			recorder->version_major = (int)value.at(ApplicationSettingKey::VERSION_TABLE.table_name).at(ApplicationSettingKey::VERSION_MAJOR.key).as_integer();
			std::cout << "〇メジャーバージョンを読み込みました．value = " << recorder->version_major << std::endl;
		}
		else
		{
			std::cout << "×メジャーバージョンが見つかりませんでした．" << std::endl;
		}

		if (value.at(ApplicationSettingKey::VERSION_TABLE.table_name).contains(ApplicationSettingKey::VERSION_MINOR.key))
		{
			recorder->version_minor = (int)value.at(ApplicationSettingKey::VERSION_TABLE.table_name).at(ApplicationSettingKey::VERSION_MINOR.key).as_integer();
			std::cout << "〇マイナーバージョンを読み込みました．value = " << recorder->version_minor << std::endl;
		}
		else
		{
			std::cout << "×マイナーバージョンが見つかりませんでした．" << std::endl;
		}

		if (value.at(ApplicationSettingKey::VERSION_TABLE.table_name).contains(ApplicationSettingKey::VERSION_PATCH.key))
		{
			recorder->version_patch = (int)value.at(ApplicationSettingKey::VERSION_TABLE.table_name).at(ApplicationSettingKey::VERSION_PATCH.key).as_integer();
			std::cout << "〇パッチバージョンを読み込みました．value = " << recorder->version_patch << std::endl;
		}
		else
		{
			std::cout << "×パッチバージョンが見つかりませんでした．" << std::endl;
		}
	}
	else
	{
		std::cout << "×バージョン設定が見つかりませんでした．" << std::endl;
	}
}


void ApplicationSettingReader::readBootModeSetting(const toml::value& value, SApplicationSettingRecorder* recorder)
{
	std::cout << std::endl;

	if (value.contains(ApplicationSettingKey::MODE_TABLE.table_name))
	{
		std::cout << "〇起動モード設定を読み込みます．" << std::endl;

		if (value.at(ApplicationSettingKey::MODE_TABLE.table_name).contains(ApplicationSettingKey::ASK_ABOUT_MODES.key))
		{
			recorder->ask_about_modes = value.at(ApplicationSettingKey::MODE_TABLE.table_name).at(ApplicationSettingKey::ASK_ABOUT_MODES.key).as_boolean();
			std::cout << "〇起動モード選択の確認フラグを読み込みました．value = " << recorder->ask_about_modes << std::endl;
		}
		else
		{
			std::cout << "×起動モード選択の確認フラグが見つかりませんでした．" << std::endl;
		}

		if (value.at(ApplicationSettingKey::MODE_TABLE.table_name).contains(ApplicationSettingKey::DEFAULT_MODE.key))
		{
			recorder->default_mode = std::sToMode(value.at(ApplicationSettingKey::MODE_TABLE.table_name).at(ApplicationSettingKey::DEFAULT_MODE.key).as_string());
			std::cout << "〇デフォルトの起動モードを読み込みました．value = " << std::to_string(recorder->default_mode) << std::endl;
		}
		else
		{
			std::cout << "×デフォルトの起動モードが見つかりませんでした．" << std::endl;
		}

		if (value.at(ApplicationSettingKey::MODE_TABLE.table_name).contains(ApplicationSettingKey::DO_STEP_EXECUTION.key))
		{
			recorder->do_step_execution = value.at(ApplicationSettingKey::MODE_TABLE.table_name).at(ApplicationSettingKey::DO_STEP_EXECUTION.key).as_boolean();
			std::cout << "〇ステップ実行フラグを読み込みました．value = " << std::boolalpha << recorder->do_step_execution << std::endl;
		}
		else
		{
			std::cout << "×ステップ実行フラグが見つかりませんでした．" << std::endl;
		}

		if (value.at(ApplicationSettingKey::MODE_TABLE.table_name).contains(ApplicationSettingKey::DO_STEP_EXECUTION_EACH_GAIT.key))
		{
			recorder->do_step_execution_each_gait = value.at(ApplicationSettingKey::MODE_TABLE.table_name).at(ApplicationSettingKey::DO_STEP_EXECUTION_EACH_GAIT.key).as_boolean();
			std::cout << "〇ステップ実行フラグ(各歩容)を読み込みました．value = " << std::boolalpha << recorder->do_step_execution_each_gait << std::endl;
		}
		else
		{
			std::cout << "×ステップ実行フラグ(各歩容)が見つかりませんでした．" << std::endl;
		}

	}
	else
	{
		std::cout << "×起動モード設定が見つかりませんでした．" << std::endl;
	}
}


void ApplicationSettingReader::readDisplaySetting(const toml::value& value, SApplicationSettingRecorder* recorder)
{
	std::cout << std::endl;

	if (value.contains(ApplicationSettingKey::DISPLAY_TABLE.table_name))
	{
		std::cout << "〇表示設定を読み込みます．" << std::endl;

		if (value.at(ApplicationSettingKey::DISPLAY_TABLE.table_name).contains(ApplicationSettingKey::CMD_OUTPUT.key))
		{
			recorder->cmd_output = value.at(ApplicationSettingKey::DISPLAY_TABLE.table_name).at(ApplicationSettingKey::CMD_OUTPUT.key).as_boolean();
			std::cout << "〇コマンド出力のフラグを読み込みました．value = " << std::boolalpha << recorder->cmd_output << std::endl;
		}
		else
		{
			std::cout << "×コマンド出力のフラグが見つかりませんでした．" << std::endl;
		}

		if (value.at(ApplicationSettingKey::DISPLAY_TABLE.table_name).contains(ApplicationSettingKey::CMD_PERMISSION.key))
		{
			recorder->cmd_permission = std::toOutputPriority(value.at(ApplicationSettingKey::DISPLAY_TABLE.table_name).at(ApplicationSettingKey::CMD_PERMISSION.key).as_string());
			std::cout << "〇コマンド表示制限の情報を読み込みました．value = " << std::to_string(recorder->cmd_permission) << std::endl;
		}
		else
		{
			std::cout << "×コマンド表示制限の情報が見つかりませんでした．" << std::endl;
		}

		if (value.at(ApplicationSettingKey::DISPLAY_TABLE.table_name).contains(ApplicationSettingKey::GUI_DISPLAY.key))
		{
			recorder->gui_display = value.at(ApplicationSettingKey::DISPLAY_TABLE.table_name).at(ApplicationSettingKey::GUI_DISPLAY.key).as_boolean();
			std::cout << "〇GUI表示のフラグを読み込みました．value = " << std::boolalpha << recorder->gui_display << std::endl;
		}
		else
		{
			std::cout << "×GUI表示のフラグが見つかりませんでした．" << std::endl;
		}

		if (value.at(ApplicationSettingKey::GUI_DISPLAY_QUALITY.table_name).contains(ApplicationSettingKey::GUI_DISPLAY_QUALITY.key))
		{
			recorder->gui_display_quality = value.at(ApplicationSettingKey::GUI_DISPLAY_QUALITY.table_name).at(ApplicationSettingKey::GUI_DISPLAY_QUALITY.key).as_string();
			std::cout << "〇GUI表示品質を読み込みました．value = " << recorder->gui_display_quality << std::endl;
		}
		else
		{
			std::cout << "×GUI表示品質が見つかりませんでした．" << std::endl;
		}

		if (value.at(ApplicationSettingKey::DISPLAY_TABLE.table_name).contains(ApplicationSettingKey::WINDOW_SIZE_X.key))
		{
			recorder->window_size_x = (int)value.at(ApplicationSettingKey::DISPLAY_TABLE.table_name).at(ApplicationSettingKey::WINDOW_SIZE_X.key).as_integer();
			std::cout << "〇ウィンドウのXサイズ(横幅)の値を読み込みました．value = " << recorder->window_size_x << std::endl;
		}
		else
		{
			std::cout << "×ウィンドウのXサイズ(横幅)の値が見つかりませんでした．" << std::endl;
		}

		if (value.at(ApplicationSettingKey::DISPLAY_TABLE.table_name).contains(ApplicationSettingKey::WINDOW_SIZE_Y.key))
		{
			recorder->window_size_y = (int)value.at(ApplicationSettingKey::DISPLAY_TABLE.table_name).at(ApplicationSettingKey::WINDOW_SIZE_Y.key).as_integer();
			std::cout << "〇ウィンドウのYサイズ(縦幅)の値を読み込みました．value = " << recorder->window_size_y << std::endl;
		}
		else
		{
			std::cout << "×ウィンドウのYサイズ(縦幅)の値が見つかりませんでした．" << std::endl;
		}

		if (value.at(ApplicationSettingKey::DISPLAY_TABLE.table_name).contains(ApplicationSettingKey::WINDOW_FPS.key))
		{
			recorder->window_fps = (int)value.at(ApplicationSettingKey::DISPLAY_TABLE.table_name).at(ApplicationSettingKey::WINDOW_FPS.key).as_integer();
			std::cout << "〇ウィンドウのFPSの値を読み込みました．value = " << recorder->window_fps << std::endl;
		}
		else
		{
			std::cout << "×ウィンドウのFPSの値が見つかりませんでした．" << std::endl;
		}
	}
	else
	{
		std::cout << "×表示設定が見つかりませんでした．" << std::endl;
	}
}
