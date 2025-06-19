
//! @file gog_selector.h

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#ifndef DESIGNLAB_GPG_SELECTOR_H_
#define DESIGNLAB_GPG_SELECTOR_H_

#include <memory>

#include "interface_gait_pattern_generator.h"
#include "interface_hexapod_coordinate_converter.h"
#include "interface_hexapod_posture_validator.h"
#include "interface_hexapod_state_presenter.h"

namespace designlab {

enum class GpgType {
  kFlat,         //!< 平坦な地面を歩く.
  kPitchRot,     //!< ピッチ回転を行う.
  kBodyRot,      //!< ボディ回転を行う.
  kStraightMove  //!< 直進移動を行う.
};

class GpgSelector final {
 public:
  GpgSelector(
      const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
      const std::shared_ptr<const IHexapodStatePresenter>& presenter_ptr,
      const std::shared_ptr<const IHexapodPostureValidator>& checker_ptr);
  ~GpgSelector() = default;

  std::unique_ptr<IGaitPatternGenerator> Select(GpgType type) const;

 private:
  std::unique_ptr<IGaitPatternGenerator> MakeGpgFlat() const;
  std::unique_ptr<IGaitPatternGenerator> MakeGpgPitchRot() const;

  const std::shared_ptr<const IHexapodCoordinateConverter> converter_ptr_;
  const std::shared_ptr<const IHexapodStatePresenter> presenter_ptr_;
  const std::shared_ptr<const IHexapodPostureValidator> checker_ptr_;
};

}  // namespace designlab

#endif  // DESIGNLAB_GPG_SELECTOR_H_
