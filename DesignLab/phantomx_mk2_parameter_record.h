
//! @file      phantomx_mk2_parameter_record.h
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory, Saitama University All right reserved.

#ifndef DESIGNLAB_PHANTOMX_MK2_PARAMETER_RECORD_H_
#define DESIGNLAB_PHANTOMX_MK2_PARAMETER_RECORD_H_

#include "toml_serialize_macro.h"


namespace designlab
{

struct PhantomXMkIIParameterRecord final
{
    //!  地面から胴体を持ち上げる高さ[mm]．最小ここまで下げられる．
    float body_lifting_height_min{ 30.f };

    //!  地面から胴体を持ち上げる高さ[mm]．最大ここまで上げられる．
    float body_lifting_height_max{ 160.f };


    float movable_coxa_angle_min_deg{ -40.f };  //!< 脚の可動範囲の最小値[deg]
    float movable_coxa_angle_max_deg{ 40.f };   //!< 脚の可動範囲の最大値[deg]


    float min_leg_range{ 140.f };   //!< 脚の付け根から脚先までの最小距離[mm]
    float max_leg_range{ 200.f };   //!< 脚の付け根から脚先までの最大距離[mm]


    float free_leg_height{ -20.f };  //!< 重心から見た遊脚高さ[mm]．

    //! 静的安全余裕．15mm程度が妥当な値となる．(先行研究より，MAXで40mm程度)
    float stable_margin{ 15.f };
};


DESIGNLAB_TOML11_DESCRIPTION_CLASS(PhantomXMkIIParameterRecord)
{
    DESIGNLAB_TOML11_FILE_NO_DESCRIPTION();

    DESIGNLAB_TOML11_TABLE_NO_DESCRIPTION();

    DESIGNLAB_TOML11_VARIABLE_ADD_DESCRIPTION(body_lifting_height_min, "Body",
        "地面から胴体を持ち上げる高さ[mm]．最小ここまで下げられる．");
    DESIGNLAB_TOML11_VARIABLE_ADD_DESCRIPTION(body_lifting_height_max, "Body",
        "地面から胴体を持ち上げる高さ[mm]．最大ここまで上げられる．");

    DESIGNLAB_TOML11_VARIABLE_ADD_DESCRIPTION(movable_coxa_angle_min_deg, "CoxaRange",
        "脚の可動範囲の最小値[deg]");
    DESIGNLAB_TOML11_VARIABLE_ADD_DESCRIPTION(movable_coxa_angle_max_deg, "CoxaRange",
        "脚の可動範囲の最大値[deg]");

    DESIGNLAB_TOML11_VARIABLE_ADD_DESCRIPTION(min_leg_range, "LegRange",
        "脚の付け根から脚先までの最小距離[mm]");
    DESIGNLAB_TOML11_VARIABLE_ADD_DESCRIPTION(max_leg_range, "LegRange",
        "脚の付け根から脚先までの最大距離[mm]");

    DESIGNLAB_TOML11_VARIABLE_ADD_DESCRIPTION(free_leg_height, "Other",
        "重心から見た遊脚高さ[mm]．");
    DESIGNLAB_TOML11_VARIABLE_ADD_DESCRIPTION(stable_margin, "Other",
        "静的安全余裕．15mm程度が妥当な値となる．(先行研究より，MAXで40mm程度)");
};

}  // namespace designlab


DESIGNLAB_TOML11_SERIALIZE(
  designlab::PhantomXMkIIParameterRecord,
  body_lifting_height_min,
  body_lifting_height_max,
  movable_coxa_angle_min_deg,
  movable_coxa_angle_max_deg,
  min_leg_range,
  max_leg_range,
  free_leg_height,
  stable_margin);

#endif  // DESIGNLAB_PHANTOMX_MK2_PARAMETER_RECORD_H_
