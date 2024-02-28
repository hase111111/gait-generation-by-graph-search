
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory, Saitama University All right reserved.

#include "robot_operator_for_path.h"

#include <numbers>

#include "math_rot_converter.h"


namespace
{
//! @todo あとで消す．

// -π～πの範囲に収める．
float NormalizeAngle(float angle)
{
    while (angle > std::numbers::pi_v<float>)
    {
        angle -= 2 * std::numbers::pi_v<float>;
    }

    while (angle < -std::numbers::pi_v<float>)
    {
        angle += 2 * std::numbers::pi_v<float>;
    }

    return angle;
}

}  // namespace


namespace designlab
{

RobotOperatorForPath::RobotOperatorForPath(const std::vector<Vector3>& path) :
    global_route_(path),
    most_near_index_(0)
{
}

RobotOperation RobotOperatorForPath::Init() const
{
    RobotOperation operation;

    operation.operation_type = enums::RobotOperationType::kStraightMoveVector;
    operation.straight_move_vector_ = Vector3::GetFrontVec();

    return operation;
}

RobotOperation RobotOperatorForPath::Update(const RobotStateNode& node)
{
    using enum enums::RobotOperationType;

    if ((global_route_[most_near_index_].ProjectedXY() - node.center_of_mass_global_coord.ProjectedXY()).GetLength() < 150.f)
    {
        most_near_index_++;
    }

    if (most_near_index_ > 0 &&
        (global_route_[most_near_index_ - 1].ProjectedXY() - node.center_of_mass_global_coord.ProjectedXY()).GetLength() < 150.f)
    {
        // 旋回によって，most_near_index_の方を向く．
        const auto diff = global_route_[most_near_index_].ProjectedXY() -
            node.center_of_mass_global_coord.ProjectedXY();

        const float angle = atan2(diff.y, diff.x);
        const float euler_z_angle = ToEulerXYZ(node.posture).z_angle;
        const float rot_dif = NormalizeAngle(angle - euler_z_angle);

        if (abs(rot_dif) > kAllowableAngleError)
        {
            RobotOperation operation;
            operation.operation_type = kSpotTurnLastPosture;
            operation.spot_turn_last_posture_ = Quaternion::MakeByAngleAxis(angle, Vector3::GetUpVec());

            return operation;
        }
    }

    //if (most_near_index_ >= global_route_.size())
    //{
    //    most_near_index_ = 0;
    //}

    //const auto diff = global_route_[most_near_index_].ProjectedXY() -
    //    node.center_of_mass_global_coord.ProjectedXY();

    //const float angle = atan2(diff.y, diff.x);
    //const float euler_z_angle = NormalizeAngle(ToEulerXYZ(node.posture).z_angle);
    //const float rot_dif = angle - euler_z_angle;

    //if (abs(rot_dif) > kAllowableAngleError)
    //{
    //    RobotOperation operation;
    //    operation.operation_type = kSpotTurnLastPosture;
    //    operation.spot_turn_last_posture_ = Quaternion::MakeByAngleAxis(angle, Vector3::GetUpVec());

    //    return operation;
    //}

    RobotOperation operation_straight;

    operation_straight.operation_type = kStraightMoveVector;

    if (most_near_index_ == 0)
    {
        operation_straight.straight_move_vector_ = (global_route_[most_near_index_] - node.center_of_mass_global_coord).GetNormalized();
    }
    else
    {
        operation_straight.straight_move_vector_ = (global_route_[most_near_index_] - global_route_[most_near_index_ - 1]).GetNormalized();
    }

    return operation_straight;
}

}  // namespace designlab
