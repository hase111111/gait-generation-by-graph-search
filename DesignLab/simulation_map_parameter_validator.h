
//! @file simulation_map_parameter_validator.h

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#pragma once

#include <string>
#include <tuple>

#include "interface_toml_data_validator.h"
#include "simulation_map_parameter.h"

namespace gaitgen {

//! @class SimulationMapParameterValidator
//! @brief SimulationMapParameter のバリデータ.
class SimulationMapParameterValidator final
    : public ITomlDataValidator<SimulationMapParameter> {
 public:
  std::tuple<bool, std::string> Validate(
      const SimulationMapParameter& toml_data) const override;

 private:
  const std::string kErrMesPerforatedRange{
      "確率は0 ～ 100 %の範囲にする必要があります."};
  const std::string kErrMesRoughnessMaxMin{
      "粗さの最大値は最小値以上にする必要があります."};
  const std::string kErrMesStripeGtZero{
      "ストライプの数は0より大きい必要があります."};
  const std::string kErrMesStepLengthGtZero{
      "階段の奥行は0より大きい必要があります."};
};

}  // namespace gaitgen
