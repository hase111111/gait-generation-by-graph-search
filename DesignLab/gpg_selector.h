
//! @file gog_selector.h

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#pragma once

#include <memory>

#include "gpg_type.h"
#include "interface_gait_pattern_generator.h"
#include "interface_hexapod_coordinate_converter.h"
#include "interface_hexapod_posture_validator.h"
#include "interface_hexapod_state_presenter.h"

namespace gaitgen {

class GpgSelector final {
 public:
  GpgSelector(
      const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
      const std::shared_ptr<const IHexapodStatePresenter>& presenter_ptr,
      const std::shared_ptr<const IHexapodPostureValidator>& checker_ptr,
      int gpg_depth, int gpg_memory_limit);
  ~GpgSelector() = default;

  std::unique_ptr<IGaitPatternGenerator> Select(GpgType type) const;

 private:
  std::unique_ptr<IGaitPatternGenerator> MakeGpgFlat() const;
  std::unique_ptr<IGaitPatternGenerator> MakeGpgFlatLegacyAlgorithm() const;
  std::unique_ptr<IGaitPatternGenerator> MakeGpgPitchRot() const;
  std::unique_ptr<IGaitPatternGenerator> MakeGpgRollRot() const;
  std::unique_ptr<IGaitPatternGenerator> MakeGpgSwitchByMap() const;
  std::unique_ptr<IGaitPatternGenerator> MakeGpgWall() const;
  std::unique_ptr<IGaitPatternGenerator> MakeGpgPruningBranch() const;
  std::unique_ptr<IGaitPatternGenerator> MakeGpgPruningBranchLegacy() const;

  const std::shared_ptr<const IHexapodCoordinateConverter> converter_ptr_;
  const std::shared_ptr<const IHexapodStatePresenter> presenter_ptr_;
  const std::shared_ptr<const IHexapodPostureValidator> checker_ptr_;
  const int gpg_depth_;
  const int gpg_memory_limit_;
};

}  // namespace gaitgen
