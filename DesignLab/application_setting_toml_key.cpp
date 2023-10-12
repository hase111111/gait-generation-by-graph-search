#include "application_setting_toml_key.h"


const std::string ApplicationSettingTomlKey::kFileTitleValue = "graph_search_settings";

const TomlSettingKeyData ApplicationSettingTomlKey::kFileTitle = { "" ,"title", u8"タイトルが\"" + ApplicationSettingTomlKey::kFileTitleValue + u8"\"の物のみを読み込みます" };


const TomlSettingTableData ApplicationSettingTomlKey::kVersionTable = { "version" ,u8"ver major.minor.patch の順で記述されます．特に使う予定はないデータだけれど一応用意しておく" };

const TomlSettingTableData ApplicationSettingTomlKey::kMoveTable = { "mode" ,u8"プログラムの起動モードの設定です" };

const TomlSettingTableData ApplicationSettingTomlKey::kDisplayTable = { "display" ,u8"表示に関する設定です．グラフ探索をする際の設定で，viewerモードで実行した場合は無視されます．" };


const TomlSettingKeyData ApplicationSettingTomlKey::kVersionMajor = { ApplicationSettingTomlKey::kVersionTable.table_name ,"major",u8"" };

const TomlSettingKeyData ApplicationSettingTomlKey::kVersionMinor = { ApplicationSettingTomlKey::kVersionTable.table_name ,"minor",u8"" };

const TomlSettingKeyData ApplicationSettingTomlKey::kVersionPatch = { ApplicationSettingTomlKey::kVersionTable.table_name ,"patch",u8"" };


const TomlSettingKeyData ApplicationSettingTomlKey::kAskAboutBootMode = { ApplicationSettingTomlKey::kMoveTable.table_name ,"ask_about_modes",u8"起動時に実行モードについて質問をするようにします (true/false)" };

const TomlSettingKeyData ApplicationSettingTomlKey::kDefaultMode = { ApplicationSettingTomlKey::kMoveTable.table_name ,"default_mode",u8"デフォルトの実行モードを設定します．(simulation/viewer/display_test/result_viewer)" };

const TomlSettingKeyData ApplicationSettingTomlKey::kDoStepExecution = { ApplicationSettingTomlKey::kMoveTable.table_name ,"do_step_execution",u8"シミュレーションをステップ実行するかどうかを設定します (true/false)" };

const TomlSettingKeyData ApplicationSettingTomlKey::kDoStepEexcutionEachGait = { ApplicationSettingTomlKey::kMoveTable.table_name ,"do_step_execution_each_gait",u8"各歩容生成ごとにステップ実行するかどうかを設定します (true/false)" };


const TomlSettingKeyData ApplicationSettingTomlKey::kOutputCmd = { ApplicationSettingTomlKey::kDisplayTable.table_name ,"cmd_output",u8"コマンドラインへの出力を行います (true/false)" };

const TomlSettingKeyData ApplicationSettingTomlKey::kCmdPermission = { ApplicationSettingTomlKey::kDisplayTable.table_name ,"cmd_permission",u8"コマンドラインに出力する文字列をどこまで許可するか，(debug,info,warning,error,system)の順に優先順位が高い" };

const TomlSettingKeyData ApplicationSettingTomlKey::kDisplayGui = { ApplicationSettingTomlKey::kDisplayTable.table_name ,"gui_display",u8"GUI(dxlibによる表示)での表示を行います (true/false)" };

const TomlSettingKeyData ApplicationSettingTomlKey::kGuiDisplayQuality = { ApplicationSettingTomlKey::kDisplayTable.table_name ,"gui_display_quality",u8"GUIでの表示の品質を設定します．(low,medium,high)の順に品質が高くなります．" };

const TomlSettingKeyData ApplicationSettingTomlKey::kWindowSizeX = { ApplicationSettingTomlKey::kDisplayTable.table_name ,"window_size_x",u8"GUIのウィンドウのサイズの横幅を設定します．xとyが16：9 になるように設定してください．" };

const TomlSettingKeyData ApplicationSettingTomlKey::kWindowSizeY = { ApplicationSettingTomlKey::kDisplayTable.table_name ,"window_size_y",u8"GUIのウィンドウのサイズの縦幅を設定します．推奨値は (x, y) = (960, 540) (1280, 720) (1600, 900) (1920, 1080) などです．" };

const TomlSettingKeyData ApplicationSettingTomlKey::kWindowFps = { ApplicationSettingTomlKey::kDisplayTable.table_name ,"window_pos_x",u8"ウィンドウのフレームレートを設定します．推奨値は60か30です．" };