
//! @file gpg_builder_change_posture.h

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#ifndef DESIGNLAB_GPG_BUILDER_CHANGE_POSTURE_H_
#define DESIGNLAB_GPG_BUILDER_CHANGE_POSTURE_H_

#include <memory>

#include "interface_gait_pattern_generator.h"
#include "interface_hexapod_coordinate_converter.h"
#include "interface_hexapod_posture_validator.h"
#include "interface_hexapod_state_presenter.h"

namespace designlab {

class GpgBuilderChangePosture final {
public:
    GpgBuilderChangePosture(
        const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
        const std::shared_ptr<const IHexapodStatePresenter>& presenter_ptr,
        const std::shared_ptr<const IHexapodPostureValidator>& checker_ptr);
    ~GpgBuilderChangePosture() = default;

    std::unique_ptr<IGaitPatternGenerator> Build() const;

private:
    const std::shared_ptr<const IHexapodCoordinateConverter> converter_ptr_;
    const std::shared_ptr<const IHexapodStatePresenter> presenter_ptr_;
    const std::shared_ptr<const IHexapodPostureValidator> checker_ptr_;
};

}  // namespace designlab

#endif  // DESIGNLAB_GPG_BUILDER_CHANGE_POSTURE_H_
