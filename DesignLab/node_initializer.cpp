
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#include "node_initializer.h"

#include "divided_map_state.h"
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
    [[maybe_unused]] const float com_z = pos_.z + base_z;  // ロボットの重心のZ座標．

    for (int i = 0; i < HexapodConst::kLegNum; i++)
    {
        res.leg_pos[i] = res.leg_reference_pos[i] = {
          160.f * cos(PhantomXMkIIConst::kCoxaDefaultAngle[i]),
          160.f * sin(PhantomXMkIIConst::kCoxaDefaultAngle[i]),
          -30.0f //-com_z
        };
    }

    res.center_of_mass_global_coord = pos_;

    // 座標をランダマイズする．
    res.center_of_mass_global_coord.x += math_util::GenerateRandomNumber(-30.f, 0.f);
    //res.center_of_mass_global_coord.y += math_util::GenerateRandomNumber(-30.f, 30.f);

    // 15 deg
    //res.leg_pos[0].z -= 20.0f;
    //res.leg_pos[5].z -= 20.0f;
    //res.leg_pos[1].z -= 80.0f;
    //res.leg_pos[4].z -= 80.0f;
    //res.leg_pos[2].z -= 135.0f;
    //res.leg_pos[3].z -= 135.0f;

    // 10 deg
    //res.leg_pos[0].z -= 32.0f;
    //res.leg_pos[5].z -= 32.0f;
    //res.leg_pos[1].z -= 74.0f;
    //res.leg_pos[4].z -= 74.0f;
    //res.leg_pos[2].z -= 116.0f;
    //res.leg_pos[3].z -= 116.0f;

    // 5 deg
    //res.leg_pos[0].z -= 10.0f;
    //res.leg_pos[5].z -= 10.0f;
    //res.leg_pos[1].z -= 34.0f;
    //res.leg_pos[4].z -= 34.0f;
    //res.leg_pos[2].z -= 57.0f;
    //res.leg_pos[3].z -= 57.0f;

    // ロールピッチヨーで回転を表現する．ロボットの重心を中心にして回転する．
    res.posture = ToQuaternion(posture_);

    res.next_move = move_;
    res.parent_index = -1;
    res.depth = 0;

    return res;
}

RobotStateNode NodeInitializer::InitNodeForTerrain(const RobotStateNode& node, const MapState map) const
{
    DividedMapState divided_map_state;
    divided_map_state.Init(map, node.center_of_mass_global_coord);

    // ロボットの重心
    const float map_z = divided_map_state.GetTopZ(divided_map_state.GetDividedMapIndexX(node.center_of_mass_global_coord.x), divided_map_state.GetDividedMapIndexY(node.center_of_mass_global_coord.y));

    RobotStateNode res = node;
    res.ChangeGlobalCenterOfMass(Vector3(node.center_of_mass_global_coord.x, node.center_of_mass_global_coord.y, map_z + 30), true);


    return res;
}

}  // namespace designlab
