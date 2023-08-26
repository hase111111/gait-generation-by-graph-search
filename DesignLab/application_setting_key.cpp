#include "application_setting_key.h"


const std::string ApplicationSettingKey::FILE_TITLE_VALUE = "graph_search_settings";

const SettingKeyData ApplicationSettingKey::FILE_TITLE = { "" ,"title", "タイトルが\"" + ApplicationSettingKey::FILE_TITLE_VALUE + "\"の物のみを読み込みます" };


const SettingTableData ApplicationSettingKey::VERSION_TABLE = { "version" ,"ver major.minor.patch の順で記述されます．特に使う予定はないデータだけれど一応用意しておく" };

const SettingKeyData ApplicationSettingKey::VERSION_MAJOR = { ApplicationSettingKey::VERSION_TABLE.table_name ,"major ","" };

const SettingKeyData ApplicationSettingKey::VERSION_MINOR = { ApplicationSettingKey::VERSION_TABLE.table_name ,"minor ","" };

const SettingKeyData ApplicationSettingKey::VERSION_PATCH = { ApplicationSettingKey::VERSION_TABLE.table_name ,"patch ","" };


const SettingTableData ApplicationSettingKey::MODE_TABLE = { "mode" ,"プログラムの起動モードの設定です" };

const SettingKeyData ApplicationSettingKey::ASK_ABOUT_MODES = { ApplicationSettingKey::MODE_TABLE.table_name ,"ask_about_modes ","起動時に実行モードについて質問をするようにします (true/false)" };

const SettingKeyData ApplicationSettingKey::DEFAULT_MODE = { ApplicationSettingKey::MODE_TABLE.table_name ,"default_mode  ","デフォルトの実行モードを設定します．(graph/viewer)" };


const SettingTableData ApplicationSettingKey::DISPLAY_TABLE = { "display" ,"表示に関する設定です．グラフ探索をする際の設定で，viewerモードで実行した場合は無視されます．" };

const SettingKeyData ApplicationSettingKey::CMD_OUTPUT = { ApplicationSettingKey::DISPLAY_TABLE.table_name ,"cmd_output","コマンドラインへの出力を行います (true/false)" };

const SettingKeyData ApplicationSettingKey::CMD_PERMISSION = { ApplicationSettingKey::DISPLAY_TABLE.table_name ,"cmd_permission","コマンドラインに出力する文字列をどこまで許可するか，(debug,info,error,system)の順に優先順位が高い" };

const SettingKeyData ApplicationSettingKey::GUI_DISPLAY = { ApplicationSettingKey::DISPLAY_TABLE.table_name ,"gui_display","GUI(dxlibによる表示)での表示を行います (true/false)" };

const SettingKeyData ApplicationSettingKey::GUI_DISPLAY_QUALITY = { ApplicationSettingKey::DISPLAY_TABLE.table_name ,"gui_display_quality","GUIでの表示の品質を設定します．(low,medium,high)の順に品質が高くなります．" };

const SettingKeyData ApplicationSettingKey::WINDOW_SIZE_X = { ApplicationSettingKey::DISPLAY_TABLE.table_name ,"window_size_x","GUIのウィンドウのサイズの横幅を設定します．xとyが16：9 になるように設定してください．" };

const SettingKeyData ApplicationSettingKey::WINDOW_SIZE_Y = { ApplicationSettingKey::DISPLAY_TABLE.table_name ,"window_size_y","GUIのウィンドウのサイズの縦幅を設定します．推奨値は (x, y) = (960, 540) (1280, 720) (1600, 900) (1920, 1080) などです．" };

const SettingKeyData ApplicationSettingKey::WINDOW_FPS = { ApplicationSettingKey::DISPLAY_TABLE.table_name ,"window_pos_x","ウィンドウのフレームレートを設定します．推奨値は60か30です．" };


const SettingTableData ApplicationSettingKey::MAP_TABLE = { "map" ,"マップ生成に関する設定です．グラフ探索をする際の設定で，viewerモードで実行した場合は無視されます．" };

const SettingKeyData ApplicationSettingKey::MAP_CREATE_MODE = { ApplicationSettingKey::MAP_TABLE.table_name ,"map_create_mode","マップの生成方法を設定します．整数で指定します" };

const SettingKeyData ApplicationSettingKey::MAP_CREATE_OPTION = { ApplicationSettingKey::MAP_TABLE.table_name ,"map_create_option","マップの生成方法のオプションを設定します．整数で指定します" };

const SettingKeyData ApplicationSettingKey::DO_OUTPUT_MAP = { ApplicationSettingKey::MAP_TABLE.table_name ,"do_output_map","マップの生成時にマップを出力するかどうかを設定します．" };

const SettingKeyData ApplicationSettingKey::MAP_HOLE_RATE = { ApplicationSettingKey::MAP_TABLE.table_name ,"map_hole_rate","マップの生成時に穴を生成する確率を設定します．0 ~ 100の範囲で指定します．" };

const SettingKeyData ApplicationSettingKey::MAP_STEP_HEIGHT = { ApplicationSettingKey::MAP_TABLE.table_name ,"map_step_height","マップの生成時に段差を生成する高さを設定します．小数で指定します" };

const SettingKeyData ApplicationSettingKey::MAP_STEP_LENGTH = { ApplicationSettingKey::MAP_TABLE.table_name ,"map_step_length","マップの生成時に段差を生成する長さを設定します．小数で指定します" };

const SettingKeyData ApplicationSettingKey::MAP_SLOPE_ANGLE_DEG = { ApplicationSettingKey::MAP_TABLE.table_name ,"map_slope_angle_deg","マップの生成時に斜面を生成する角度[deg]を設定します．小数で指定します" };

const SettingKeyData ApplicationSettingKey::MAP_TILT_ANGLE_DEG = { ApplicationSettingKey::MAP_TABLE.table_name ,"map_tilt_angle_deg","マップの生成時に傾斜を生成する角度[deg]を設定します．小数で指定します" };

const SettingKeyData ApplicationSettingKey::ROUGH_MAX_DIF = { ApplicationSettingKey::MAP_TABLE.table_name ,"rough_max_dif","マップの生成時に段差を生成する際の最大の高さの差を設定します．小数で指定します" };

const SettingKeyData ApplicationSettingKey::ROUGH_MIN_DIF = { ApplicationSettingKey::MAP_TABLE.table_name ,"rough_min_dif","マップの生成時に段差を生成する際の最小の高さの差を設定します．小数で指定します" };

