
//! @file      robot_operator_for_path.h
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#ifndef DESIGNLAB_ROBOT_OPERATOR_FOR_PATH_H_
#define DESIGNLAB_ROBOT_OPERATOR_FOR_PATH_H_

#include <vector>

#include "math_util.h"
#include "interface_robot_operator.h"


namespace designlab
{

//! @class RobotOperatorForPath
//! @brief 指定した中継地点を通るようにロボットを動かすクラス．
class RobotOperatorForPath final : public IRobotOperator
{
public:
    explicit RobotOperatorForPath(const std::vector<Vector3>& path);

    RobotOperation Init() const override;

    RobotOperation Update(const RobotStateNode& node) override;

private:
    //! 目標角度と現在の角度の許容誤差．
    static constexpr float kAllowableAngleError{ math_util::ConvertDegToRad(1.2f) };

    std::vector<Vector3> global_route_;  //!< グローバルパス．
    int most_near_index_{ 0 };
};

}  // namespace designlab


#endif  // DESIGNLAB_ROBOT_OPERATOR_FOR_PATH_H_
