#include "application_setting_key.h"


const std::string ApplicationSettingKey::FILE_TITLE_VALUE = "graph_search_settings";

const SettingKeyData ApplicationSettingKey::FILE_TITLE = { "" ,"title", u8"タイトルが\"" + ApplicationSettingKey::FILE_TITLE_VALUE + u8"\"の物のみを読み込みます" };


const SettingTableData ApplicationSettingKey::VERSION_TABLE = { "version" ,u8"ver major.minor.patch の順で記述されます．特に使う予定はないデータだけれど一応用意しておく" };

const SettingKeyData ApplicationSettingKey::VERSION_MAJOR = { ApplicationSettingKey::VERSION_TABLE.table_name ,"major",u8"" };

const SettingKeyData ApplicationSettingKey::VERSION_MINOR = { ApplicationSettingKey::VERSION_TABLE.table_name ,"minor",u8"" };

const SettingKeyData ApplicationSettingKey::VERSION_PATCH = { ApplicationSettingKey::VERSION_TABLE.table_name ,"patch",u8"" };


const SettingTableData ApplicationSettingKey::MODE_TABLE = { "mode" ,u8"プログラムの起動モードの設定です" };

const SettingKeyData ApplicationSettingKey::ASK_ABOUT_MODES = { ApplicationSettingKey::MODE_TABLE.table_name ,"ask_about_modes",u8"起動時に実行モードについて質問をするようにします (true/false)" };

const SettingKeyData ApplicationSettingKey::DEFAULT_MODE = { ApplicationSettingKey::MODE_TABLE.table_name ,"default_mode",u8"デフォルトの実行モードを設定します．(simulation/viewer/display_test/result_viewer)" };

const SettingKeyData ApplicationSettingKey::DO_STEP_EXECUTION = { ApplicationSettingKey::MODE_TABLE.table_name ,"do_step_execution",u8"シミュレーションをステップ実行するかどうかを設定します (true/false)" };

const SettingKeyData ApplicationSettingKey::DO_STEP_EXECUTION_EACH_GAIT = { ApplicationSettingKey::MODE_TABLE.table_name ,"do_step_execution_each_gait",u8"各歩容生成ごとにステップ実行するかどうかを設定します (true/false)" };



const SettingTableData ApplicationSettingKey::DISPLAY_TABLE = { "display" ,u8"表示に関する設定です．グラフ探索をする際の設定で，viewerモードで実行した場合は無視されます．" };

const SettingKeyData ApplicationSettingKey::CMD_OUTPUT = { ApplicationSettingKey::DISPLAY_TABLE.table_name ,"cmd_output",u8"コマンドラインへの出力を行います (true/false)" };

const SettingKeyData ApplicationSettingKey::CMD_PERMISSION = { ApplicationSettingKey::DISPLAY_TABLE.table_name ,"cmd_permission",u8"コマンドラインに出力する文字列をどこまで許可するか，(debug,info,warning,error,system)の順に優先順位が高い" };

const SettingKeyData ApplicationSettingKey::GUI_DISPLAY = { ApplicationSettingKey::DISPLAY_TABLE.table_name ,"gui_display",u8"GUI(dxlibによる表示)での表示を行います (true/false)" };

const SettingKeyData ApplicationSettingKey::GUI_DISPLAY_QUALITY = { ApplicationSettingKey::DISPLAY_TABLE.table_name ,"gui_display_quality",u8"GUIでの表示の品質を設定します．(low,medium,high)の順に品質が高くなります．" };

const SettingKeyData ApplicationSettingKey::WINDOW_SIZE_X = { ApplicationSettingKey::DISPLAY_TABLE.table_name ,"window_size_x",u8"GUIのウィンドウのサイズの横幅を設定します．xとyが16：9 になるように設定してください．" };

const SettingKeyData ApplicationSettingKey::WINDOW_SIZE_Y = { ApplicationSettingKey::DISPLAY_TABLE.table_name ,"window_size_y",u8"GUIのウィンドウのサイズの縦幅を設定します．推奨値は (x, y) = (960, 540) (1280, 720) (1600, 900) (1920, 1080) などです．" };

const SettingKeyData ApplicationSettingKey::WINDOW_FPS = { ApplicationSettingKey::DISPLAY_TABLE.table_name ,"window_pos_x",u8"ウィンドウのフレームレートを設定します．推奨値は60か30です．" };