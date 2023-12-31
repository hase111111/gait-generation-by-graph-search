
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

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
    const enums::OutputDetail output_detail = enums::OutputDetail::kSystem;

    CmdIOUtil::Output("起動モードを選択してください", output_detail);

    // 起動モードの名前を表示する．
    for (int i = 0; i < kBootModeNum; i++)
    {
        const enums::BootMode boot_mode = static_cast<enums::BootMode>(i);

        const std::string boot_mode_name = string_util::EnumToStringRemoveTopK(boot_mode);

        CmdIOUtil::Output(std::format("{} : {}", i, boot_mode_name),
                          output_detail);
    }


    std::string default_mode_name = string_util::EnumToStringRemoveTopK(default_mode_);

    CmdIOUtil::Output(std::format("other : デフォルトのモード ( {} )", default_mode_name),
                      output_detail);


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

        CmdIOUtil::Output("入力が不正です。デフォルトのモードを選択します。",
                          enums::OutputDetail::kSystem);

        return default_mode_;
    }
}

}  // namespace designlab
