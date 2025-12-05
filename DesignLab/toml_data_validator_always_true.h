
//! @file toml_data_validator_always_true.h

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#pragma once

#include <string>
#include <tuple>

#include "cmdio_util.h"
#include "interface_toml_data_validator.h"

namespace gaitgen {

//! @class TomlDataValidatorAlwaysTrue
//! @brief 常に trueを返す ITomlDataValidator の実装クラス.
//! @tparam T 検証対象の型.
template <typename T>
class TomlDataValidatorAlwaysTrue final : public ITomlDataValidator<T> {
 public:
  std::tuple<bool, std::string> Validate(
      [[maybe_unused]] const T& toml_data) const override {
    cmdio::InfoOutput(
        "(No validation is performed in the current configuration.)");
    return {true, ""};
  }
};

}  // namespace gaitgen
