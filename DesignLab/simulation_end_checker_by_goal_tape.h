//! @file simulation_end_checker_by_goal_tape.h
//! @brief シミュレーションの終了を判定するクラス．


#ifndef DESIGNLAB_SIMULATION_END_CHECKER_BY_GOAL_TAPE_H_
#define DESIGNLAB_SIMULATION_END_CHECKER_BY_GOAL_TAPE_H_

#include "interface_simulation_end_checker.h"

#include "robot_state_node.h"


//! @class SimulationEndCheckerByGoalTape
//! @brief ゴールテープによるシミュレーション終了判定クラス．
class SimulationEndCheckerByGoalTape final : public ISimulationEndChecker
{
public:

	SimulationEndCheckerByGoalTape(float goal_tape_x);

	bool IsEnd(const RobotStateNode& node) const override;

private:

	const float kGoalTape;	//!< 直進のときに、Y方向にこの値だけ進めたら1シミュレーション終了
};


#endif	// DESIGNLAB_SIMULATION_END_CHECKER_BY_GOAL_TAPE_H_