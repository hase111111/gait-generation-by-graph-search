#include "application_setting_reader.h"

#include <iostream>
#include <fstream>
#include <filesystem>

#include "toml.hpp"


void ApplicationSettingReader::read(SApplicationSettingRecorder* recorder)
{
	outputDefaultSettingFile();
	return;


	std::cout << "[" << __func__ << "]" << std::endl;
	std::cout << "設定ファイル" << SETTING_FILE_NAME << "を読み込みます" << std::endl;

	//ファイルを探す，存在しなかったらデフォルトの設定を出力して終了，fsystemはC++17から，実行できない場合は設定を見直してみてください
	if (!std::filesystem::is_regular_file(SETTING_FILE_NAME))
	{
		std::cout << "設定ファイルが見つかりませんでした．デフォルトの設定を出力します．" << std::endl;
		outputDefaultSettingFile();
		return;
	}


	//ファイルを読み込む
	try
	{
		std::ifstream ifs(SETTING_FILE_NAME, std::ios_base::binary);
		auto data = toml::parse(ifs, SETTING_FILE_NAME);
	}
	catch (toml::syntax_error e)
	{
		std::cout << "設定ファイルの読み込みに失敗しました．デフォルトの設定を出力します．" << std::endl;
		std::cout << e.what() << std::endl;
	}


	//if (toml::get<std::string>(data.at(ApplicationSettingKey::FILE_TITLE.key)) != ApplicationSettingKey::FILE_TITLE_VALUE)
	//{
	//	//ファイルのタイトルが一致しない場合はデフォルトの設定を出力して終了
	//	std::cout << "設定ファイルのタイトルが一致しませんでした．デフォルトの設定を出力します．" << std::endl;
	//	outputDefaultSettingFile();
	//	return;
	//}

	//(*recorder).version_major = toml::find<int>(data, ApplicationSettingKey::VERSION_MAJOR.key);
	//(*recorder).version_minor = toml::find<int>(data, ApplicationSettingKey::VERSION_MINOR.key);
	//(*recorder).version_patch = toml::find<int>(data, ApplicationSettingKey::VERSION_PATCH.key);

	//(*recorder).ask_about_modes = toml::find<bool>(data, ApplicationSettingKey::ASK_ABOUT_MODES.key);
	//(*recorder).default_mode = toml::find<std::string>(data, ApplicationSettingKey::DEFAULT_MODE.key);

	//(*recorder).cmd_output = toml::find<bool>(data, ApplicationSettingKey::CMD_OUTPUT.key);
	//(*recorder).cmd_permission = toml::find<std::string>(data, ApplicationSettingKey::CMD_PERMISSION.key);
	//(*recorder).gui_display = toml::find<bool>(data, ApplicationSettingKey::GUI_DISPLAY.key);
	//(*recorder).gui_display_quality = toml::find<std::string>(data, ApplicationSettingKey::GUI_DISPLAY_QUALITY.key);
	//(*recorder).gui_display_quality = toml::find<std::string>(data, ApplicationSettingKey::GUI_DISPLAY_QUALITY.key);
	//(*recorder).gui_display_quality = toml::find<std::string>(data, ApplicationSettingKey::GUI_DISPLAY_QUALITY.key);
	//(*recorder).gui_display_quality = toml::find<std::string>(data, ApplicationSettingKey::GUI_DISPLAY_QUALITY.key);
	//(*recorder).gui_display_quality = toml::find<std::string>(data, ApplicationSettingKey::GUI_DISPLAY_QUALITY.key);
	//(*recorder).gui_display_quality = toml::find<std::string>(data, ApplicationSettingKey::GUI_DISPLAY_QUALITY.key);
	//(*recorder).gui_display_quality = toml::find<std::string>(data, ApplicationSettingKey::GUI_DISPLAY_QUALITY.key);
	//(*recorder).gui_display_quality = toml::find<std::string>(data, ApplicationSettingKey::GUI_DISPLAY_QUALITY.key);
	//(*recorder).gui_display_quality = toml::find<std::string>(data, ApplicationSettingKey::GUI_DISPLAY_QUALITY.key);
}



void ApplicationSettingReader::outputDefaultSettingFile()
{

	toml::basic_value<toml::preserve_comments> data1_value1{10, { "a" }};
	toml::value data{ {"key1-1", data1_value1}, { "key1-2", 10 }, { "key1-3", 10 }};
	toml::value data2{{"key2", data1_value1}};
	toml::value data3{{"key3", 12}};
	toml::table table;
	table["table1"] = data;
	table["table2"] = data2;
	table["table3"] = data3;


	toml::value datalist = table;
	std::string str = toml::format(datalist, 0);

	std::cout << str;
	std::ofstream ofs;
	ofs.open(SETTING_FILE_NAME);

	// ファイルが開けなかったら何もしない
	if (!ofs)
	{
		std::cout << "デフォルトの設定ファイルの出力に失敗しました．" << std::endl;
		return;
	}

	ofs << str;
	ofs.close();
	return;

	const SApplicationSettingRecorder kDefaultSetting;
	const int kIndention = 1;
	const int kBigIndention = 3;

	ofs << " # TOMLという形式で記述しています．wikiでググるだけでもわかりやすい情報が出るので調べてみてください．" << std::endl;
	ofs << " # メモ帳などのテキストエディタでもこのファイルは編集することができます．" << std::endl;
	ofs << " # シミュレーション条件を変更したい場合は，ここから変更を行うようにしてみてください．" << std::endl;
	ofs << " # このようにシャープで始まる行はコメントです．プログラムに影響を与えないため，メモ代わりに使うことができます．" << std::endl;
	outputIndention(ofs, kBigIndention);

	//ファイルのタイトル
	ofs << ApplicationSettingKey::FILE_TITLE.key << " = " << '\"' << kDefaultSetting.SETTING_FILE_TITLE << '\"' << "  # " << ApplicationSettingKey::FILE_TITLE.description << std::endl;
	outputIndention(ofs, kBigIndention);

	//バージョン表示
	outputTable(ofs, ApplicationSettingKey::VERSION_TABLE);
	outputIndention(ofs, kIndention);
	ofs << ApplicationSettingKey::VERSION_MAJOR.key << " = " << kDefaultSetting.version_major << "  # " << ApplicationSettingKey::VERSION_MAJOR.description << std::endl;
	ofs << ApplicationSettingKey::VERSION_MINOR.key << " = " << kDefaultSetting.version_minor << "  # " << ApplicationSettingKey::VERSION_MINOR.description << std::endl;
	ofs << ApplicationSettingKey::VERSION_PATCH.key << " = " << kDefaultSetting.version_patch << "  # " << ApplicationSettingKey::VERSION_PATCH.description << std::endl;
	outputIndention(ofs, kBigIndention);

	//実行時のモードの設定
	outputTable(ofs, ApplicationSettingKey::MODE_TABLE);
	outputIndention(ofs, kIndention);
	ofs << ApplicationSettingKey::ASK_ABOUT_MODES.key << " = " << std::boolalpha << kDefaultSetting.ask_about_modes << "  # " << ApplicationSettingKey::ASK_ABOUT_MODES.description << std::endl;
	outputIndention(ofs, kIndention);
	ofs << ApplicationSettingKey::DEFAULT_MODE.key << " = " << '\"' << kDefaultSetting.default_mode << '\"' << "  # " << ApplicationSettingKey::DEFAULT_MODE.description << std::endl;
	outputIndention(ofs, kBigIndention);

	//表示についての設定
	outputTable(ofs, ApplicationSettingKey::DISPLAY_TABLE);
	outputIndention(ofs, kIndention);
	ofs << ApplicationSettingKey::CMD_OUTPUT.key << " = " << std::boolalpha << kDefaultSetting.cmd_output << "  # " << ApplicationSettingKey::CMD_OUTPUT.description << std::endl;
	outputIndention(ofs, kIndention);
	ofs << ApplicationSettingKey::CMD_PERMISSION.key << " = " << '\"' << kDefaultSetting.cmd_permission << '\"' << "  # " << ApplicationSettingKey::CMD_PERMISSION.description << std::endl;
	outputIndention(ofs, kIndention);
	ofs << ApplicationSettingKey::GUI_DISPLAY.key << " = " << std::boolalpha << kDefaultSetting.gui_display << "  # " << ApplicationSettingKey::GUI_DISPLAY.description << std::endl;
	outputIndention(ofs, kIndention);
	ofs << ApplicationSettingKey::GUI_DISPLAY_QUALITY.key << " = " << '\"' << kDefaultSetting.gui_display_quality << '\"' << "  # " << ApplicationSettingKey::GUI_DISPLAY_QUALITY.description << std::endl;
	outputIndention(ofs, kBigIndention);
	ofs << ApplicationSettingKey::WINDOW_SIZE_X.key << " = " << kDefaultSetting.window_size_x << "  # " << ApplicationSettingKey::WINDOW_SIZE_X.description << std::endl;
	outputIndention(ofs, kIndention);
	ofs << ApplicationSettingKey::WINDOW_SIZE_Y.key << " = " << kDefaultSetting.window_size_y << "  # " << ApplicationSettingKey::WINDOW_SIZE_Y.description << std::endl;
	outputIndention(ofs, kIndention);
	ofs << ApplicationSettingKey::WINDOW_FPS.key << " = " << kDefaultSetting.window_fps << "  # " << ApplicationSettingKey::WINDOW_FPS.description << std::endl;
	outputIndention(ofs, kBigIndention);

	//マップについての設定
	outputTable(ofs, ApplicationSettingKey::MAP_TABLE);
	outputIndention(ofs, kIndention);
	ofs << ApplicationSettingKey::MAP_CREATE_MODE.key << " = " << '\"' << (int)kDefaultSetting.map_create_mode << '\"' << "  # " << ApplicationSettingKey::MAP_CREATE_MODE.description << std::endl;
	outputIndention(ofs, kIndention);
	ofs << ApplicationSettingKey::MAP_CREATE_OPTION.key << " = " << '\"' << kDefaultSetting.map_create_option << '\"' << "  # " << ApplicationSettingKey::MAP_CREATE_OPTION.description << std::endl;
	outputIndention(ofs, kIndention);
	ofs << ApplicationSettingKey::DO_OUTPUT_MAP.key << " = " << std::boolalpha << kDefaultSetting.do_output_map << "  # " << ApplicationSettingKey::DO_OUTPUT_MAP.description << std::endl;
	outputIndention(ofs, kIndention);
	ofs << ApplicationSettingKey::MAP_HOLE_RATE.key << " = " << kDefaultSetting.map_hole_rate << "  # " << ApplicationSettingKey::MAP_HOLE_RATE.description << std::endl;
	outputIndention(ofs, kIndention);
	ofs << ApplicationSettingKey::MAP_STEP_HEIGHT.key << " = " << kDefaultSetting.map_step_height << "  # " << ApplicationSettingKey::MAP_STEP_HEIGHT.description << std::endl;
	outputIndention(ofs, kIndention);
	ofs << ApplicationSettingKey::MAP_STEP_LENGTH.key << " = " << kDefaultSetting.map_step_length << "  # " << ApplicationSettingKey::MAP_STEP_LENGTH.description << std::endl;
	outputIndention(ofs, kIndention);
	ofs << ApplicationSettingKey::MAP_SLOPE_ANGLE_DEG.key << " = " << kDefaultSetting.map_slope_angle_deg << "  # " << ApplicationSettingKey::MAP_SLOPE_ANGLE_DEG.description << std::endl;
	outputIndention(ofs, kIndention);
	ofs << ApplicationSettingKey::MAP_TILT_ANGLE_DEG.key << " = " << kDefaultSetting.map_tilt_angle_deg << "  # " << ApplicationSettingKey::MAP_TILT_ANGLE_DEG.description << std::endl;
	outputIndention(ofs, kIndention);
	ofs << ApplicationSettingKey::ROUGH_MAX_DIF.key << " = " << kDefaultSetting.rough_max_dif << "  # " << ApplicationSettingKey::ROUGH_MAX_DIF.description << std::endl;
	outputIndention(ofs, kIndention);
	ofs << ApplicationSettingKey::ROUGH_MIN_DIF.key << " = " << kDefaultSetting.rough_min_dif << "  # " << ApplicationSettingKey::ROUGH_MIN_DIF.description << std::endl;
	outputIndention(ofs, kBigIndention);

	//ファイルを閉じる
	ofs.close();
}

void ApplicationSettingReader::outputIndention(std::ofstream& ofs, int indention) const
{
	if (indention < 0)return;

	for (int i = 0; i < indention; i++)
	{
		ofs << std::endl;
	}
}

void ApplicationSettingReader::outputTable(std::ofstream& ofs, const SettingTableData& table) const
{
	ofs << " # " << table.description << std::endl;
	ofs << '[' << table.table_name << ']' << std::endl;
}
