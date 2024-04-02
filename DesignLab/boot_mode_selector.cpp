
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory,
//! Saitama University All right reserved.

#include "boot_mode_selector.h"

#include <format>
#include <string>

#include <magic_enum.hpp>

#include "cmdio_util.h"
#include "string_util.h"


namespace designlab
{

BootModeSelector::BootModeSelector() :
    kBootModeNum(static_cast<int>(magic_enum::enum_count<enums::BootMode>())),
    default_mode_(enums::BootMode::kSimulation)
{
}


enums::BootMode BootModeSelector::SelectBootMode()
{
    // kSystem にすると、設定にかかわらず必ず表示される．
    const OutputDetail output_detail = OutputDetail::kSystem;

    CmdIOUtil::Output("Select the mode to run.", output_detail);

    // 起動モードの名前を表示する．
    for (int i = 0; i < kBootModeNum; i++)
    {
        const enums::BootMode boot_mode = static_cast<enums::BootMode>(i);

        const std::string boot_mode_name =
            string_util::EnumToStringRemoveTopK(boot_mode);

        CmdIOUtil::FormatOutput(output_detail, "{} : {}", i, boot_mode_name);
    }


    const std::string default_mode_name =
        string_util::EnumToStringRemoveTopK(default_mode_);

    CmdIOUtil::FormatOutput(output_detail, "other : default mode ( {} )",
                            default_mode_name);


    CmdIOUtil::OutputNewLine(1, output_detail);

    // 0 から BootMode の数までの整数を入力させる．
    int default_mode_num = static_cast<int>(default_mode_);

    const int input = CmdIOUtil::InputInt(0, kBootModeNum - 1, default_mode_num);

    // 受け取った値を magic_enum の enum_cast で BootMode に変換する．
    if (magic_enum::enum_cast<enums::BootMode>(input).has_value())
    {
        return magic_enum::enum_cast<enums::BootMode>(input).value();
    }
    else
    {
        // 入力が不正な場合はデフォルトのモードを返す．

        CmdIOUtil::Output("Input value is invalid. Run default mode.",
                          output_detail);

        return default_mode_;
    }
}

}  // namespace designlab
