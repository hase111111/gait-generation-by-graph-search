
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory, Saitama University All right reserved.

#include "robot_operator_for_gpg.h"

#include <format>
#include <iostream>
#include <numbers>

#include "cmdio_util.h"
#include "math_rot_converter.h"


namespace
{

// -π～πの範囲に収める
float NormalizeAngle(float angle)
{
    while (angle > std::numbers::pi_v<float>)
    {
        angle -= std::numbers::pi_v<float>;
    }

    while (angle < -std::numbers::pi_v<float>)
    {
        angle += std::numbers::pi_v<float>;
    }

    return angle;
}

}  // namespace


namespace designlab
{

RobotOperatorForGpg::RobotOperatorForGpg() : global_route_{
    {4910,630,40},
{4890,630,40},
{4870,630,40},
{4850,630,40},
{4830,650,40},
{4810,650,40},
{4790,630,40},
{4770,630,40},
{4750,650,40},
{4730,650,40},
{4710,630,40},
{4690,650,40},
{4670,650,40},
{4650,650,40},
{4630,650,40},
{4610,650,40},
{4590,650,40},
{4570,650,40},
{4550,650,40},
{4530,650,40},
{4510,650,40},
{4490,650,40},
{4470,650,40},
{4450,650,40},
{4430,650,40},
{4410,650,40},
{4390,650,40},
{4370,650,40},
{4350,630,40},
{4330,630,40},
{4310,630,40},
{4290,630,40},
{4270,630,40},
{4250,650,40},
{4230,650,40},
{4210,650,40},
{4190,650,40},
{4170,650,40},
{4150,630,40},
{4130,630,40},
{4110,630,40},
{4090,630,40},
{4070,650,40},
{4050,650,40},
{4030,650,40},
{4010,650,40},
{3990,650,40},
{3970,650,40},
{3950,650,40},
{3930,650,40},
{3910,650,40},
{3890,650,40},
{3870,650,40},
{3850,630,40},
{3830,630,40},
{3810,630,40},
{3790,630,40},
{3770,630,40},
{3750,630,40},
{3730,650,40},
{3710,650,40},
{3690,650,40},
{3670,650,40},
{3650,650,40},
{3630,650,40},
{3610,650,40},
{3590,650,40},
{3570,650,40},
{3550,650,40},
{3530,650,40},
{3510,630,40},
{3490,630,40},
{3470,630,40},
{3450,630,40},
{3430,630,40},
{3410,630,40},
{3390,630,40},
{3370,630,40},
{3350,630,40},
{3330,630,40},
{3310,630,40},
{3290,630,40},
{3270,630,40},
{3250,630,40},
{3230,630,40},
{3210,630,40},
{3190,630,40},
{3170,630,40},
{3150,610,40},
{3130,590,40},
{3110,570,40},
{3090,570,40},
{3070,570,40},
{3050,550,40},
{3030,530,40},
{3010,530,40},
{2990,510,40},
{2970,490,40},
{2950,470,40},
{2930,450,40},
{2910,430,40},

}
{
}

RobotOperation RobotOperatorForGpg::Init() const
{
    return RobotOperation();
}

RobotOperation RobotOperatorForGpg::Update(const RobotStateNode& node)
{
    using enum OutputDetail;
    using enum enums::RobotOperationType;

    // まず，現在の重心位置から最も近い点を探す．
    int most_near_index = 0;
    float min_distance = 100000.f;

    for (int i = 0; i < global_route_.size(); ++i)
    {
        const float now_distance = global_route_[i].ProjectedXY().GetDistanceFrom(
            node.center_of_mass_global_coord.ProjectedXY());

        if (min_distance > now_distance)
        {
            min_distance = now_distance;

            most_near_index = i;
        }
    }

    CmdIOUtil::Output(std::format("most_near_index : {}", most_near_index), kDebug);

    // 次の地点への角度を計算する．
    const float euler_z_angle = NormalizeAngle(ToEulerXYZ(node.posture).z_angle);
    const Vector3 diff_vector = global_route_[most_near_index - 1] - global_route_[most_near_index];
    const float target_angle = NormalizeAngle(atan2(diff_vector.y, diff_vector.x));
    const float rot_dif = target_angle - euler_z_angle;

    CmdIOUtil::Output(std::format("target_angle : {} / now_angle : {}",
                      math_util::ConvertRadToDeg(target_angle),
                      math_util::ConvertRadToDeg(euler_z_angle)),
                      kDebug);

    if (abs(rot_dif) > kAllowableAngleError)
    {
        RobotOperation operation;
        operation.operation_type = kSpotTurnLastPosture;
        operation.spot_turn_last_posture_ =
            Quaternion::MakeByAngleAxis(target_angle, Vector3::GetUpVec());

        CmdIOUtil::Output(std::format("target_angle : {} / now_angle : {}",
                          math_util::ConvertRadToDeg(target_angle),
                          math_util::ConvertRadToDeg(euler_z_angle)),
                          kDebug);

        return operation;
    }

    const int loop_num = 10;
    Vector3 target_vector;

    for (int i = 0; i < loop_num; i++)
    {
        if (most_near_index - i < 0)
        {
            break;
        }

        target_vector += (global_route_[most_near_index - i] - node.center_of_mass_global_coord) *
            static_cast<float>(i + 1);
    }

    CmdIOUtil::Output(std::format("target_vector : {}", target_vector.ToString()), kDebug);

    RobotOperation operation;
    operation.operation_type = kStraightMoveVector;
    operation.straight_move_vector_ = Vector3(target_vector.x, target_vector.y, 0).GetNormalized();

    return operation;
}

}  // namespace designlab
