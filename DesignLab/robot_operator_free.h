#ifndef DESIGNLAB_ROBOT_OPERATOR_FREE_H_
#define DESIGNLAB_ROBOT_OPERATOR_FREE_H_

#include <vector>

#include "designlab_math_util.h"
#include "interface_robot_operator.h"


//! @class RobotOperatorFree
//! @brief global path generator を行うための仮置きクラス．
class RobotOperatorFree final : public IRobotOperator
{
public:

	RobotOperatorFree();

	RobotOperation Init() const override;

	RobotOperation Update(const RobotStateNode& node) override;

private:

	static constexpr float kAllowableAngleError{ ::designlab::math_util::ConvertDegToRad(2.0f) };	//!< 目標角度と現在の角度の許容誤差．

	std::vector<::designlab::Vector3> global_route_;	//!< グローバルパス．
	int most_near_index_{ 0 };
};

#endif  // DESIGNLAB_ROBOT_OPERATOR_FREE_H_