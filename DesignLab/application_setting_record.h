
//! @file      application_setting_record.h
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#ifndef DESIGNLAB_APPLICATION_SETTING_RECORD_H_
#define DESIGNLAB_APPLICATION_SETTING_RECORD_H_

#include <format>
#include <map>
#include <string>
#include <vector>

#include "boot_mode.h"
#include "display_quality.h"
#include "output_detail.h"
#include "string_util.h"
#include "toml_serialize_macro.h"


namespace designlab
{

//! @struct ApplicationSettingRecord
//! @brief アプリの設定を記録する構造体．
struct ApplicationSettingRecord final
{
    static constexpr int kWindowWidthMin{ 512 };
    static constexpr int kWindowWidthMax{ 1920 };
    static constexpr int kWindowHeightMin{ 288 };
    static constexpr int kWindowHeightMax{ 1080 };
    static constexpr int kFpsMin{ 15 };
    static constexpr int kFpsMax{ 60 };


    int version_major{ 0 };  //!< バージョン番号(メジャー)
    int version_minor{ 5 };  //!< バージョン番号(マイナー)
    int version_patch{ 0 };  //!< バージョン番号(パッチ)


    bool ask_about_modes{ true };  //!< 起動時にモード選択の確認をするかどうか．
    enums::BootMode default_mode{ enums::BootMode::kSimulation };  //!< デフォルトの起動モード．

    //! 1シミュレーションごとにステップ実行をするかどうか．
    bool do_step_execution_each_simulation{ true };
    bool do_step_execution_each_gait{ false };  //!< 1動作ごとにステップ実行をするかどうか．


    bool do_cmd_output{ true };  //!< コマンドラインに出力するかどうか．

    //! コマンドラインに出力する際，どこまで許可するか．
    enums::OutputDetail cmd_output_detail{ enums::OutputDetail::kInfo };

    bool do_gui_display{ true };  //!< GUIを表示するかどうか．

    //! GUIを表示する際，どこまで許可するか．
    enums::DisplayQuality gui_display_quality{ enums::DisplayQuality::kHigh };
    int window_size_x{ 1600 };  //!< グラフィカルウィンドウの横幅．
    int window_size_y{ 900 };   //!< グラフィカルウィンドウの縦幅．
    int window_fps{ 60 };       //!< グラフィカルウィンドウのFPS．
};


DESIGNLAB_TOML11_DESCRIPTION_CLASS(ApplicationSettingRecord)
{
    const std::vector<std::string> kFileDescription{
        "If this file is garbled, the problem is most likely due to character encoding.",
            "This file is written in utf - 8, and can be read by installing VS Code and "
            "configuring it to automatically detect the character encoding.",
            "This file is written in the TOML format. Just google it and you'll find easy "
            "to understand information on the wiki, so try looking it up.",
            "This file is a file for describing program settings.",
            "This file can also be edited with a text editor such as Notepad.",
            "If you want to change the simulation conditions, try changing them from here."
            "Lines starting with a sharp are comments. They do not affect the program."
            "Following description is written in Japanese. ",
            "",
            "このファイルが文字化けしている場合は，文字コードの問題が考えられます．",
            "このファイルは utf-8 で記述されており，VS Code をインストールし，"
            "文字コードを自動で検出するように設定することで読み込むことができます．",
            "このファイルはTOML形式で記述されています．"
            "ググれば wiki に分かりやすい情報が載っているので，調べてみてください．",
            "このファイルはプログラムの設定を記述するファイルです．",
            "このファイルはメモ帳などのテキストエディタでも編集することができます．",
            "シミュレーション条件を変更したい場合は，ここから変更してみてください．",
            "シャープから始まる行はコメントです．プログラムに影響を与えることはありません．",
    };

    DESIGNLAB_TOML11_FILE_ADD_DESCRIPTION_MULTI_LINE(kFileDescription);

    DESIGNLAB_TOML11_TABLE_ADD_DESCRIPTION(
        "Verion",
        "ver major.minor.patch の順で記述されます．特に使う予定はないデータだが一応用意しておく．",
        "Mode", "プログラムの起動モードの設定です",
        "Output", "プログラムの出力の設定です");

    DESIGNLAB_TOML11_VARIABLE_ADD_DESCRIPTION(
        version_major, "Verion",
        "メジャーバージョンは大きな更新があったときに変えます．");
    DESIGNLAB_TOML11_VARIABLE_NO_DESCRIPTION(version_minor, "Verion");
    DESIGNLAB_TOML11_VARIABLE_NO_DESCRIPTION(version_patch, "Verion");

    DESIGNLAB_TOML11_VARIABLE_ADD_DESCRIPTION(
        ask_about_modes, "Mode",
        "起動時に実行モードについて質問をするようにします．( true / false )");
    DESIGNLAB_TOML11_VARIABLE_ADD_DESCRIPTION(
        default_mode, "Mode",
        std::format("起動時のデフォルトの実行モードを設定します．( \"{}\" )",
        string_util::EnumValuesToString<enums::BootMode>("\" / \"")));
    DESIGNLAB_TOML11_VARIABLE_ADD_DESCRIPTION(
        do_step_execution_each_simulation, "Mode",
        "1シミュレーションごとにステップ実行をするかどうかを設定します．( true / false )");
    DESIGNLAB_TOML11_VARIABLE_ADD_DESCRIPTION(
        do_step_execution_each_gait, "Mode",
        "1動作ごとにステップ実行をするかどうかを設定します．( true / false )");

    DESIGNLAB_TOML11_VARIABLE_ADD_DESCRIPTION(
        do_cmd_output, "Output",
        "コマンドラインに出力するかどうかを設定します．( true / false )");
    DESIGNLAB_TOML11_VARIABLE_ADD_DESCRIPTION(
        cmd_output_detail, "Output",
        std::format("コマンドラインに出力する際，どこまで許可するかを設定します．( \"{}\" )",
        string_util::EnumValuesToString<enums::OutputDetail>("\" / \"")));
    DESIGNLAB_TOML11_VARIABLE_ADD_DESCRIPTION(
        do_gui_display, "Output",
        "GUIを表示するかどうかを設定します．( true / false )");
    DESIGNLAB_TOML11_VARIABLE_ADD_DESCRIPTION(
        gui_display_quality, "Output",
        std::format("GUIを表示する際，どこまで許可するかを設定します．( \"{}\" )",
        string_util::EnumValuesToString<enums::DisplayQuality>("\" / \"")));
    DESIGNLAB_TOML11_VARIABLE_ADD_DESCRIPTION(
        window_size_x, "Output",
        std::format("グラフィカルウィンドウの横幅を設定します．{} ～ {}の範囲．推奨値は1600です．",
        ApplicationSettingRecord::kWindowWidthMin,
        ApplicationSettingRecord::kWindowWidthMax));
    DESIGNLAB_TOML11_VARIABLE_ADD_DESCRIPTION(
        window_size_y, "Output",
        std::format("グラフィカルウィンドウの縦幅を設定します．{} ～ {}の範囲．推奨値は900です．",
        ApplicationSettingRecord::kWindowHeightMin,
        ApplicationSettingRecord::kWindowHeightMax));
    DESIGNLAB_TOML11_VARIABLE_ADD_DESCRIPTION(
        window_fps, "Output",
        std::format("グラフィカルウィンドウのFPSを設定します．{} ～ {}の範囲．推奨値は60です．",
        ApplicationSettingRecord::kFpsMin,
        ApplicationSettingRecord::kFpsMax));
};

}  // namespace designlab


DESIGNLAB_TOML11_SERIALIZE(
    designlab::ApplicationSettingRecord,
    version_major, version_minor, version_patch,
    ask_about_modes, default_mode, do_step_execution_each_simulation,
    do_step_execution_each_gait,
    do_cmd_output, cmd_output_detail, do_gui_display,
    gui_display_quality, window_size_x, window_size_y, window_fps);


#endif  // DESIGNLAB_APPLICATION_SETTING_RECORD_H_
