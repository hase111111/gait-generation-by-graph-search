#pragma once

#include <string>


struct SettingKeyData
{
	std::string table_name;
	std::string key;
	std::string description;
};


struct SettingTableData
{
	std::string table_name;
	std::string description;
};



class ApplicationSettingKey final
{
public:
	//実体を生成しない
	ApplicationSettingKey() = delete;
	ApplicationSettingKey(ApplicationSettingKey& other) = delete;
	ApplicationSettingKey& operator=(ApplicationSettingKey& other) = delete;

	const static std::string FILE_TITLE_VALUE;	//!< ファイルタイトルの値
	const static SettingKeyData FILE_TITLE;

	const static SettingTableData VERSION_TABLE;
	const static SettingKeyData VERSION_MAJOR;
	const static SettingKeyData VERSION_MINOR;
	const static SettingKeyData VERSION_PATCH;

	const static SettingTableData MODE_TABLE;
	const static SettingKeyData ASK_ABOUT_MODES;
	const static SettingKeyData DEFAULT_MODE;
	const static SettingKeyData DO_STEP_EXECUTION;
	const static SettingKeyData DO_STEP_EXECUTION_EACH_GAIT;

	const static SettingTableData DISPLAY_TABLE;
	const static SettingKeyData CMD_OUTPUT;
	const static SettingKeyData CMD_PERMISSION;
	const static SettingKeyData GUI_DISPLAY;
	const static SettingKeyData GUI_DISPLAY_QUALITY;
	const static SettingKeyData WINDOW_SIZE_X;
	const static SettingKeyData WINDOW_SIZE_Y;
	const static SettingKeyData WINDOW_FPS;

	const static SettingTableData MAP_TABLE;
	const static SettingKeyData MAP_CREATE_MODE;
	const static SettingKeyData MAP_CREATE_OPTION;
	const static SettingKeyData DO_OUTPUT_MAP;
	const static SettingKeyData MAP_HOLE_RATE;
	const static SettingKeyData MAP_STEP_HEIGHT;
	const static SettingKeyData MAP_STEP_LENGTH;
	const static SettingKeyData MAP_SLOPE_ANGLE_DEG;
	const static SettingKeyData MAP_TILT_ANGLE_DEG;
	const static SettingKeyData ROUGH_MAX_DIF;
	const static SettingKeyData ROUGH_MIN_DIF;

};

