
//! @file xr_r1_parameter_record.h

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#ifndef DESIGNLAB_XR_R1_PARAMETER_RECORD_H_
#define DESIGNLAB_XR_R1_PARAMETER_RECORD_H_

#include "toml_serialize_macro.h"


namespace designlab {

struct XrR1ParameterRecord final {
    //!  地面から胴体を持ち上げる高さ[mm]．最小ここまで下げられる．
    float body_lifting_height_min{ 60.f };

    //!  地面から胴体を持ち上げる高さ[mm]．最大ここまで上げられる．
    float body_lifting_height_max{ 160.f };


    float movable_coxa_angle_min_deg{ -40.f };  //!< 脚の可動範囲の最小値[deg]
    float movable_coxa_angle_max_deg{ 40.f };   //!< 脚の可動範囲の最大値[deg]


    float min_leg_range{ 140.f };   //!< 脚の付け根から脚先までの最小距離[mm]
    float max_leg_range{ 195.f };   //!< 脚の付け根から脚先までの最大距離[mm]


    float free_leg_height{ -30.f };  //!< 重心から見た遊脚高さ[mm]．

    //! 静的安全余裕．15mm程度が妥当な値となる．(先行研究より，MAXで40mm程度)
    float stable_margin{ 15.f };
};


DESIGNLAB_TOML11_DESCRIPTION_CLASS(XrR1ParameterRecord) {
    DESIGNLAB_TOML11_FILE_NO_DESCRIPTION();

    DESIGNLAB_TOML11_TABLE_NO_DESCRIPTION();

    DESIGNLAB_TOML11_VARIABLE_ADD_DESCRIPTION(body_lifting_height_min, "Body",
        "Height at which the body is lifted from the ground [mm]. "
        "The minimum height can be lowered to this level.");
    DESIGNLAB_TOML11_VARIABLE_ADD_DESCRIPTION(body_lifting_height_max, "Body",
        "Height at which the body is lifted from the ground [mm]. "
        "The maximum height can be raised to this level.");

    DESIGNLAB_TOML11_VARIABLE_ADD_DESCRIPTION(
        movable_coxa_angle_min_deg, "CoxaRange",
        "Minimum leg movement range [deg].");
    DESIGNLAB_TOML11_VARIABLE_ADD_DESCRIPTION(
        movable_coxa_angle_max_deg, "CoxaRange",
        "Maximum leg range of motion [deg].");

    DESIGNLAB_TOML11_VARIABLE_ADD_DESCRIPTION(min_leg_range, "LegRange",
        "Minimum distance from the base of the leg to the tip of the leg [mm].");
    DESIGNLAB_TOML11_VARIABLE_ADD_DESCRIPTION(max_leg_range, "LegRange",
        "Maximum distance from the base of the leg to the tip of the leg [mm].");

    DESIGNLAB_TOML11_VARIABLE_ADD_DESCRIPTION(free_leg_height, "Other",
        "Height of the swing leg from the center of gravity [mm].");
    DESIGNLAB_TOML11_VARIABLE_ADD_DESCRIPTION(stable_margin, "Other",
        "Static safety margin: 15mm is a reasonable value. "
        "(Based on previous studies, max is about 40 mm.)");
};

}  // namespace designlab


DESIGNLAB_TOML11_SERIALIZE(
  designlab::XrR1ParameterRecord,
  body_lifting_height_min,
  body_lifting_height_max,
  movable_coxa_angle_min_deg,
  movable_coxa_angle_max_deg,
  min_leg_range,
  max_leg_range,
  free_leg_height,
  stable_margin);

#endif  // DESIGNLAB_XR_R1_PARAMETER_RECORD_H_
