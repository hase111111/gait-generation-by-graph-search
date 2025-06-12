
//! @file boot_mode_selector.cpp

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#include "boot_mode_selector.h"

#include <format>
#include <magic_enum.hpp>
#include <string>

#include "cmdio_util.h"
#include "string_util.h"

namespace designlab {

BootModeSelector::BootModeSelector()
    : kBootModeNum(static_cast<int>(magic_enum::enum_count<BootMode>())),
      default_mode_(BootMode::kSimulation) {}

BootMode BootModeSelector::SelectBootMode() {
  // kSystem にすると、設定にかかわらず必ず表示される.
  const OutputDetail output_detail = OutputDetail::kSystem;

  cmdio::Output("Select the mode to run.", output_detail);

  // 起動モードの名前を表示する.
  for (int i = 0; i < kBootModeNum; i++) {
    const BootMode boot_mode = static_cast<BootMode>(i);

    const std::string boot_mode_name =
        string_util::EnumToStringRemoveTopK(boot_mode);

    cmdio::OutputF(output_detail, "{} : {}", i, boot_mode_name);
  }

  const std::string default_mode_name =
      string_util::EnumToStringRemoveTopK(default_mode_);

  cmdio::OutputF(output_detail, "other : default mode ( {} )",
                 default_mode_name);

  cmdio::OutputNewLine(1, output_detail);

  // 0 から BootMode の数までの整数を入力させる.
  int default_mode_num = static_cast<int>(default_mode_);

  const int input = cmdio::InputInt(0, kBootModeNum - 1, default_mode_num);

  // 受け取った値を magic_enum の enum_cast で BootMode に変換する.
  if (magic_enum::enum_cast<BootMode>(input).has_value()) {
    return magic_enum::enum_cast<BootMode>(input).value();
  } else {
    // 入力が不正な場合はデフォルトのモードを返す.

    cmdio::Output("Input value is invalid. Run default mode.", output_detail);

    return default_mode_;
  }
}

}  // namespace designlab
