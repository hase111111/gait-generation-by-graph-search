
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#include "node_initializer.h"

#include "math_rot_converter.h"
#include "math_util.h"
#include "leg_state.h"
#include "phantomx_mk2_const.h"


namespace designlab
{

NodeInitializer::NodeInitializer(const Vector3& pos, const EulerXYZ& posture, enums::HexapodMove move) :
    pos_(pos),
    posture_(posture),
    move_(move)
{
}

RobotStateNode NodeInitializer::InitNode() const
{
    RobotStateNode res;

    // 脚状態．
    res.leg_state = leg_func::MakeLegStateBit(
      enums::DiscreteComPos::kCenterBack,
      { true, true, true, true, true, true },
      { enums::DiscreteLegPos::kCenter, enums::DiscreteLegPos::kCenter,
        enums::DiscreteLegPos::kCenter, enums::DiscreteLegPos::kCenter,
        enums::DiscreteLegPos::kCenter, enums::DiscreteLegPos::kCenter });


    const float base_z = 0.0f;  // 地面のZ座標．
    const float com_z = pos_.z + base_z;  // ロボットの重心のZ座標．

    for (int i = 0; i < HexapodConst::kLegNum; i++)
    {
        res.leg_pos[i] = res.leg_reference_pos[i] = {
          160.f * cos(PhantomXMkIIConst::kCoxaDefaultAngle[i]),
          160.f * sin(PhantomXMkIIConst::kCoxaDefaultAngle[i]),
          -com_z
        };
    }

    res.center_of_mass_global_coord = pos_;

    // ロールピッチヨーで回転を表現する．ロボットの重心を中心にして回転する．
    res.posture = ToQuaternion(posture_);

    res.next_move = move_;
    res.parent_index = -1;
    res.depth = 0;

    return res;
}

}  // namespace designlab
