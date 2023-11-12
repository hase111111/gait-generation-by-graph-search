//! @file simulation_end_checker.h
//! @brief シミュレーションの終了を判定するクラス


#ifndef DESIGNLAB_SIMULATION_END_CHECKER_H_
#define DESIGNLAB_SIMULATION_END_CHECKER_H_


#include "robot_state_node.h"


class SimulationEndChecker final
{
public:

	bool IsEnd(const RobotStateNode& node) const;

private:

	float kGoalTape = 1200.0f;	//!< 直進のときに、Y方向にこの値だけ進めたら1シミュレーション終了
};


#endif	// !DESIGNLAB_SIMULATION_END_CHECKER_H_