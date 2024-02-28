
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory, Saitama University All right reserved.

#include "interpolated_node_creator.h"

#include "cassert_define.h"
#include "math_util.h"
#include "hexapod_const.h"


namespace designlab
{

InterpolatedNodeCreator::InterpolatedNodeCreator(
    const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr) :
    converter_ptr_(converter_ptr)
{
}

std::vector<RobotStateNode>  InterpolatedNodeCreator::CreateInterpolatedNode(
    const RobotStateNode& current_node,
    const RobotStateNode& next_node) const
{
    if (IsNoChange(current_node, next_node))
    {
        return { };  // 空のvectorを返す．
    }

    if (IsBodyRot(current_node, next_node))
    {
        return CreateBodyRotInterpolatedNode(current_node, next_node);
    }

    if (IsBodyMove(current_node, next_node))
    {
        return CreateBodyMoveInterpolatedNode(current_node, next_node);
    }

    return CreateLegMoveInterpolatedNode(current_node, next_node);
}

bool InterpolatedNodeCreator::IsNoChange(const RobotStateNode& current_node,
                                         const RobotStateNode& next_node) const
{
    bool res = true;

    for (int i = 0; i < HexapodConst::kLegNum; i++)
    {
        if (current_node.leg_pos[i] != next_node.leg_pos[i])
        {
            res = false;
            break;
        }
    }

    if (current_node.center_of_mass_global_coord != next_node.center_of_mass_global_coord)
    {
        res = false;
    }

    if (current_node.posture != next_node.posture)
    {
        res = false;
    }

    return res;
}

bool InterpolatedNodeCreator::IsBodyMove(const RobotStateNode& current_node,
                                         const RobotStateNode& next_node) const
{
    return current_node.center_of_mass_global_coord != next_node.center_of_mass_global_coord;
}

bool InterpolatedNodeCreator::IsBodyRot(const RobotStateNode& current_node,
                                        const RobotStateNode& next_node) const
{
    return current_node.posture.GetNormalized() != next_node.posture.GetNormalized();
}

std::vector<RobotStateNode> InterpolatedNodeCreator::CreateBodyMoveInterpolatedNode(
    const RobotStateNode& current_node, const RobotStateNode& next_node) const
{
    std::vector<RobotStateNode> res;

    const Vector3 dif = next_node.center_of_mass_global_coord - current_node.center_of_mass_global_coord;

    if (dif.GetLength() < kInterpolatedDistance) { return {}; }

    if (dif.GetLength() > kBodyMoveMaxInterpolatedDistance) { return {}; }

    int cnt = 1;

    while (kInterpolatedDistance * static_cast<float>(cnt) < dif.GetLength())
    {
        RobotStateNode temp_node = current_node;

        temp_node.ChangeGlobalCenterOfMass(
          temp_node.center_of_mass_global_coord + dif.GetNormalized() * kInterpolatedDistance * static_cast<float>(cnt),
          true);

        res.push_back(temp_node);

        cnt++;
    }

    return res;
}

std::vector<RobotStateNode> InterpolatedNodeCreator::CreateBodyRotInterpolatedNode(
    const RobotStateNode& current_node, const RobotStateNode& next_node) const
{
    std::vector<RobotStateNode> res;

    constexpr int kBodyMoveInterpolatedNodeNum = 100;  //!< 胴体が移動する際の補間ノード数．

    for (int i = 0; i < kBodyMoveInterpolatedNodeNum; i++)
    {
        RobotStateNode temp_node = current_node;
        const float ex = (static_cast<float>(i) + 1.0f) /
            (static_cast<float>(kBodyMoveInterpolatedNodeNum));

        const Quaternion quat =
            SlerpQuaternion(current_node.posture, next_node.posture, ex).GetNormalized();

        temp_node.ChangePosture(converter_ptr_, quat);

        res.push_back(temp_node);
    }

    return res;
}

std::vector<RobotStateNode> InterpolatedNodeCreator::CreateLegMoveInterpolatedNode(
    const RobotStateNode& current_node, const RobotStateNode& next_node) const
{
    std::vector<RobotStateNode> res;

    // 接地脚の平行移動，下降動作，遊脚の上昇動作，平行移動を行う．

    // 接地．
    {
        std::vector<int> ground_move_index = GetGroundMoveIndex(current_node, next_node);

        // 接地脚の平行移動．
        while (true)
        {
            RobotStateNode temp_node = res.empty() ? current_node : res.back();

            bool is_end = true;

            for (const auto& i : ground_move_index)
            {
                // 角度方向に移動．
                const float angle_current = atan2(temp_node.leg_pos[i].y, temp_node.leg_pos[i].x);
                const float angle_next = atan2(next_node.leg_pos[i].y, next_node.leg_pos[i].x);

                if (angle_current > angle_next + kInterpolatedAngle)
                {
                    const float length = temp_node.leg_pos[i].ProjectedXY().GetLength();
                    temp_node.leg_pos[i].x = length * cos(angle_current - kInterpolatedAngle);
                    temp_node.leg_pos[i].y = length * sin(angle_current - kInterpolatedAngle);
                    is_end = false;
                }
                else if (angle_current < angle_next - kInterpolatedAngle)
                {
                    const float length = temp_node.leg_pos[i].ProjectedXY().GetLength();
                    temp_node.leg_pos[i].x = length * cos(angle_current + kInterpolatedAngle);
                    temp_node.leg_pos[i].y = length * sin(angle_current + kInterpolatedAngle);
                    is_end = false;
                }
                else
                {
                    const float length = temp_node.leg_pos[i].ProjectedXY().GetLength();
                    temp_node.leg_pos[i].x = length * cos(angle_next);
                    temp_node.leg_pos[i].y = length * sin(angle_next);
                }

                // 半径方向に移動．
                const float length_current = temp_node.leg_pos[i].ProjectedXY().GetLength();
                const float length_next = next_node.leg_pos[i].ProjectedXY().GetLength();

                if (length_current > length_next + kInterpolatedDistance)
                {
                    const float angle = atan2(temp_node.leg_pos[i].y, temp_node.leg_pos[i].x);
                    temp_node.leg_pos[i].x = (length_current - kInterpolatedDistance) * cos(angle);
                    temp_node.leg_pos[i].y = (length_current - kInterpolatedDistance) * sin(angle);
                    is_end = false;
                }
                else if (length_current < length_next - kInterpolatedDistance)
                {
                    const float angle = atan2(temp_node.leg_pos[i].y, temp_node.leg_pos[i].x);
                    temp_node.leg_pos[i].x = (length_current + kInterpolatedDistance) * cos(angle);
                    temp_node.leg_pos[i].y = (length_current + kInterpolatedDistance) * sin(angle);
                    is_end = false;
                }
                else
                {
                    const float angle = atan2(temp_node.leg_pos[i].y, temp_node.leg_pos[i].x);
                    temp_node.leg_pos[i].x = length_next * cos(angle);
                    temp_node.leg_pos[i].y = length_next * sin(angle);
                }
            }

            res.push_back(temp_node);

            if (is_end) { break; }
        }

        // 接地脚の下降．
        // 全ての脚先を kInterpolatedDistance づつ下げていき，距離が kInterpolatedDistance 以下になったら next_node と同じz座標にする．
        while (true)
        {
            RobotStateNode temp_node = res.back();

            bool is_end = true;

            for (const auto& i : ground_move_index)
            {
                if (temp_node.leg_pos[i].z > next_node.leg_pos[i].z + kInterpolatedDistance)
                {
                    temp_node.leg_pos[i].z -= kInterpolatedDistance;
                    is_end = false;
                }
                else if (temp_node.leg_pos[i].z < next_node.leg_pos[i].z - kInterpolatedDistance)
                {
                    temp_node.leg_pos[i].z += kInterpolatedDistance;
                    is_end = false;
                }
                else
                {
                    temp_node.leg_pos[i].z = next_node.leg_pos[i].z;
                    leg_func::ChangeGround(i, true, &temp_node.leg_state);
                }
            }

            res.push_back(temp_node);

            if (is_end) { break; }
        }

    }

    // 遊脚．
    {
        std::vector<int> free_move_index = GetFreeMoveIndex(current_node, next_node);

        // 接地脚の下降．
        // 全ての脚先を kInterpolatedDistance づつ下げていき，距離が kInterpolatedDistance 以下になったら next_node と同じz座標にする．
        while (true)
        {
            RobotStateNode temp_node = res.back();

            bool is_end = true;

            for (const auto& i : free_move_index)
            {
                if (temp_node.leg_pos[i].z > next_node.leg_pos[i].z + kInterpolatedDistance)
                {
                    temp_node.leg_pos[i].z -= kInterpolatedDistance;
                    is_end = false;
                }
                else if (temp_node.leg_pos[i].z < next_node.leg_pos[i].z - kInterpolatedDistance)
                {
                    temp_node.leg_pos[i].z += kInterpolatedDistance;
                    is_end = false;
                }
                else
                {
                    temp_node.leg_pos[i].z = next_node.leg_pos[i].z;
                    leg_func::ChangeGround(i, false, &temp_node.leg_state);
                }
            }

            res.push_back(temp_node);

            if (is_end) { break; }
        }

        // 接地脚の平行移動．
        while (true)
        {
            RobotStateNode temp_node = res.empty() ? current_node : res.back();

            bool is_end = true;

            for (const auto& i : free_move_index)
            {
                // 角度方向に移動．
                const float angle_current = atan2(temp_node.leg_pos[i].y, temp_node.leg_pos[i].x);
                const float angle_next = atan2(next_node.leg_pos[i].y, next_node.leg_pos[i].x);

                if (angle_current > angle_next + kInterpolatedAngle)
                {
                    const float length = temp_node.leg_pos[i].ProjectedXY().GetLength();
                    temp_node.leg_pos[i].x = length * cos(angle_current - kInterpolatedAngle);
                    temp_node.leg_pos[i].y = length * sin(angle_current - kInterpolatedAngle);
                    is_end = false;
                }
                else if (angle_current < angle_next - kInterpolatedAngle)
                {
                    const float length = temp_node.leg_pos[i].ProjectedXY().GetLength();
                    temp_node.leg_pos[i].x = length * cos(angle_current + kInterpolatedAngle);
                    temp_node.leg_pos[i].y = length * sin(angle_current + kInterpolatedAngle);
                    is_end = false;
                }
                else
                {
                    const float length = temp_node.leg_pos[i].ProjectedXY().GetLength();
                    temp_node.leg_pos[i].x = length * cos(angle_next);
                    temp_node.leg_pos[i].y = length * sin(angle_next);
                }

                // 半径方向に移動．
                const float length_current = temp_node.leg_pos[i].ProjectedXY().GetLength();
                const float length_next = next_node.leg_pos[i].ProjectedXY().GetLength();

                if (length_current > length_next + kInterpolatedDistance)
                {
                    const float angle = atan2(temp_node.leg_pos[i].y, temp_node.leg_pos[i].x);
                    temp_node.leg_pos[i].x = (length_current - kInterpolatedDistance) * cos(angle);
                    temp_node.leg_pos[i].y = (length_current - kInterpolatedDistance) * sin(angle);
                    is_end = false;
                }
                else if (length_current < length_next - kInterpolatedDistance)
                {
                    const float angle = atan2(temp_node.leg_pos[i].y, temp_node.leg_pos[i].x);
                    temp_node.leg_pos[i].x = (length_current + kInterpolatedDistance) * cos(angle);
                    temp_node.leg_pos[i].y = (length_current + kInterpolatedDistance) * sin(angle);
                    is_end = false;
                }
                else
                {
                    const float angle = atan2(temp_node.leg_pos[i].y, temp_node.leg_pos[i].x);
                    temp_node.leg_pos[i].x = length_next * cos(angle);
                    temp_node.leg_pos[i].y = length_next * sin(angle);
                }
            }

            res.push_back(temp_node);

            if (is_end) { break; }
        }
    }

    return res;
}

std::vector<int> InterpolatedNodeCreator::GetGroundMoveIndex(
    const RobotStateNode& current_node, const RobotStateNode& next_node) const
{
    // 脚先座標の差分を計算．
    std::array<Vector3, HexapodConst::kLegNum> dif;

    for (int i = 0; i < HexapodConst::kLegNum; i++)
    {
        dif[i] = next_node.leg_pos[i] - current_node.leg_pos[i];
    }

    // indexを返す．
    std::vector<int> res_index;

    for (int i = 0; i < HexapodConst::kLegNum; i++)
    {
        if (dif[i].z < 0)
        {
            res_index.push_back(i);
        }
    }

    return res_index;
}

std::vector<int> InterpolatedNodeCreator::GetFreeMoveIndex(
    const RobotStateNode& current_node, const RobotStateNode& next_node) const
{
    // 脚先座標の差分を計算．
    std::array<Vector3, HexapodConst::kLegNum> dif;

    for (int i = 0; i < HexapodConst::kLegNum; i++)
    {
        dif[i] = next_node.leg_pos[i] - current_node.leg_pos[i];
    }

    // indexを返す．
    std::vector<int> res_index;

    for (int i = 0; i < HexapodConst::kLegNum; i++)
    {
        if (dif[i].z > 0)
        {
            res_index.push_back(i);
        }
    }

    return res_index;
}

}  // namespace designlab
