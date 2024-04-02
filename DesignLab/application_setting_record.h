
//! @file      application_setting_record.h
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory,
//! Saitama University All right reserved.

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

    //! 起動時にモード選択の確認をするかどうか．
    bool ask_about_modes{ true };

    //! デフォルトの起動モード．
    enums::BootMode default_mode{ enums::BootMode::kSimulation };

    //! 1シミュレーションごとにステップ実行をするかどうか．
    bool do_step_execution_each_simulation{ true };

    //! 1動作ごとにステップ実行をするかどうか．
    bool do_step_execution_each_gait{ false };


    bool do_cmd_output{ true };  //!< コマンドラインに出力するかどうか．

    //! コマンドラインに出力する際，どこまで許可するか．
    OutputDetail cmd_output_detail{ OutputDetail::kInfo };

    bool do_gui_display{ true };  //!< GUIを表示するかどうか．

    //! GUIを表示する際，どこまで許可するか．
    enums::DisplayQuality gui_display_quality{ enums::DisplayQuality::kHigh };

    int window_size_x{ 1600 };  //!< グラフィカルウィンドウの横幅．
    int window_size_y{ 900 };   //!< グラフィカルウィンドウの縦幅．
    int window_fps{ 60 };       //!< グラフィカルウィンドウのFPS．
};


DESIGNLAB_TOML11_DESCRIPTION_CLASS(ApplicationSettingRecord)
{
    const std::vector<std::string> kFileDescription = {
        "If this file is garbled, "
        "the problem is most likely due to character encoding.",
        "This file is written in utf - 8, and can be read by installing VS Code and "
        "configuring it to automatically detect the character encoding.",
        "This file is written in the TOML format. "
        "Just google it and you'll find easy "
        "to understand information on the wiki, so try looking it up.",
        "This file is a file for describing program settings.",
        "This file can also be edited with a text editor such as Notepad.",
        "If you want to change the simulation conditions, "
        "try changing them from here."
        "Lines starting with a sharp are comments. They do not affect the program."
    };

    DESIGNLAB_TOML11_FILE_ADD_DESCRIPTION_MULTI_LINE(kFileDescription);

    DESIGNLAB_TOML11_TABLE_ADD_DESCRIPTION(
        "Verion",
        "The order is major.minor.patch.",
        "Mode", "Setting the mode in which the program is to be run",
        "Output", "Program output settings.");

    DESIGNLAB_TOML11_VARIABLE_NO_DESCRIPTION(version_major, "Verion");
    DESIGNLAB_TOML11_VARIABLE_NO_DESCRIPTION(version_minor, "Verion");
    DESIGNLAB_TOML11_VARIABLE_NO_DESCRIPTION(version_patch, "Verion");

    DESIGNLAB_TOML11_VARIABLE_ADD_DESCRIPTION(
        ask_about_modes, "Mode",
        "Ask the user about the run mode at startup. ( true / false )");
    DESIGNLAB_TOML11_VARIABLE_ADD_DESCRIPTION(
        default_mode, "Mode",
        std::format("Sets the default run mode at startup. ( \"{}\" )",
        string_util::EnumValuesToString<enums::BootMode>("\" / \"")));
    DESIGNLAB_TOML11_VARIABLE_ADD_DESCRIPTION(
        do_step_execution_each_simulation, "Mode",
        "Ask if you want to continue with each simulation. ( true / false )");
    DESIGNLAB_TOML11_VARIABLE_ADD_DESCRIPTION(
        do_step_execution_each_gait, "Mode",
        "Ask if you want to continue with each move. ( true / false )");

    DESIGNLAB_TOML11_VARIABLE_ADD_DESCRIPTION(
        do_cmd_output, "Output",
        "Output characters to the command line. ( true / false )");
    DESIGNLAB_TOML11_VARIABLE_ADD_DESCRIPTION(
        cmd_output_detail, "Output",
        std::format("How much output is allowed on the command line. ( \"{}\" )",
        string_util::EnumValuesToString<OutputDetail>("\" / \"")));
    DESIGNLAB_TOML11_VARIABLE_ADD_DESCRIPTION(
        do_gui_display, "Output", "Display GUI. ( true / false )");
    DESIGNLAB_TOML11_VARIABLE_ADD_DESCRIPTION(
        gui_display_quality, "Output",
        std::format("GUI Image Quality．( \"{}\" )",
        string_util::EnumValuesToString<enums::DisplayQuality>("\" / \"")));
    DESIGNLAB_TOML11_VARIABLE_ADD_DESCRIPTION(
        window_size_x, "Output",
        std::format("Sets the width of the GUI. The range is from {} to {}. "
        "The recommended value is 1600.",
        ApplicationSettingRecord::kWindowWidthMin,
        ApplicationSettingRecord::kWindowWidthMax));
    DESIGNLAB_TOML11_VARIABLE_ADD_DESCRIPTION(
        window_size_y, "Output",
        std::format("Sets the height of the GUI. The range is from {} to {}. "
        "The recommended value is 900.",
        ApplicationSettingRecord::kWindowHeightMin,
        ApplicationSettingRecord::kWindowHeightMax));
    DESIGNLAB_TOML11_VARIABLE_ADD_DESCRIPTION(
        window_fps, "Output",
        std::format("Sets FPS. The range is from {} to {}. "
        "The recommended value is 60.",
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
