//! @file application_setting_key.h
//! @brief 設定ファイルのキーをまとめた定数クラス

#ifndef DESIGNLAB_APPLICATION_SETTING_KEY_H_
#define DESIGNLAB_APPLICATION_SETTING_KEY_H_


#include <string>


//! @struct SettingKeyData
//! @brief 設定ファイルのデータのキーをまとめた構造体
struct SettingKeyData
{
	std::string table_name;		//!< データが入っているテーブル名
	std::string key;			//!< キー名
	std::string description;	//!< 説明
};


//! @struct SettingTableData
//! @brief 設定ファイルのテーブルのデータをまとめた構造体
struct SettingTableData
{
	std::string table_name;		//!< テーブル名
	std::string description;	//!< 説明
};


//! @class ApplicationSettingKey
//! @brief 設定ファイルのキーをまとめた定数クラス
class ApplicationSettingKey final
{
public:

	//コンストラクタは削除し，実体を生成しない
	ApplicationSettingKey() = delete;
	ApplicationSettingKey(ApplicationSettingKey& other) = delete;
	ApplicationSettingKey(ApplicationSettingKey&& other) = delete;
	ApplicationSettingKey& operator=(ApplicationSettingKey& other) = delete;

	const static std::string FILE_TITLE_VALUE;
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
};


#endif // !DESIGNLAB_APPLICATION_SETTING_KEY_H_