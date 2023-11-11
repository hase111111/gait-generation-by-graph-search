//! @file application_setting_key.h
//! @brief 設定ファイル(tomlファイル)のキーをまとめた定数クラス

#ifndef DESIGNLAB_APPLICATION_SETTING_TOML_KEY_H_
#define DESIGNLAB_APPLICATION_SETTING_TOML_KEY_H_


#include <string>


//! @struct TomlSettingKeyData
//! @brief 設定ファイルのデータのキーをまとめた構造体
struct TomlSettingKeyData final
{
	std::string table_name;		//!< データが入っているテーブル名
	std::string key;			//!< キー名
	std::string description;	//!< 説明
};


//! @struct TomlSettingTableData
//! @brief 設定ファイルのテーブルのデータをまとめた構造体
struct TomlSettingTableData final
{
	std::string table_name;		//!< テーブル名
	std::string description;	//!< 説明
};


//! @class ApplicationSettingTomlKey
//! @brief toml形式の設定ファイルのキーをまとめた定数クラス
class ApplicationSettingTomlKey final
{
public:

	//コンストラクタは削除し，実体を生成しない
	ApplicationSettingTomlKey() = delete;
	ApplicationSettingTomlKey(ApplicationSettingTomlKey& other) = delete;
	ApplicationSettingTomlKey(ApplicationSettingTomlKey&& other) = delete;
	ApplicationSettingTomlKey& operator=(ApplicationSettingTomlKey& other) = delete;

	const static std::string kFileTitleValue;	//!< tomlファイルの名前ではなく，Titleというキーに格納される値
	const static TomlSettingKeyData kFileTitle;	

	const static TomlSettingTableData kVersionTable;
	const static TomlSettingTableData kMoveTable;
	const static TomlSettingTableData kDisplayTable;

	const static TomlSettingKeyData kVersionMajor;
	const static TomlSettingKeyData kVersionMinor;
	const static TomlSettingKeyData kVersionPatch;

	const static TomlSettingKeyData kAskAboutBootMode;
	const static TomlSettingKeyData kDefaultMode;
	const static TomlSettingKeyData kDoStepExecution;
	const static TomlSettingKeyData kDoStepEexcutionEachGait;

	const static TomlSettingKeyData kOutputCmd;
	const static TomlSettingKeyData kCmdPermission;
	const static TomlSettingKeyData kDisplayGui;
	const static TomlSettingKeyData kGuiDisplayQuality;
	const static TomlSettingKeyData kWindowSizeX;
	const static TomlSettingKeyData kWindowSizeY;
	const static TomlSettingKeyData kWindowFps;
};


#endif // DESIGNLAB_APPLICATION_SETTING_KEY_H_