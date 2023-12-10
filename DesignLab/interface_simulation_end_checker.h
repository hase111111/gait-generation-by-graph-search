//! @file interface_simulation_end_checker.h
//! @brief シミュレーションの終了を判定するクラスのインターフェース．

#ifndef DESIGNLAB_INTERFACE_SIMULATION_END_CHECKER_H_
#define DESIGNLAB_INTERFACE_SIMULATION_END_CHECKER_H_

#include "robot_state_node.h"

//! @class ISimulationEndChecker
//! @brief シミュレーションの終了を判定するクラスのインターフェース．
class ISimulationEndChecker
{
public:
	virtual ~ISimulationEndChecker() = default;

	//! @brief シミュレーションの終了を判定する．
	//! @param[in] node ロボットの状態
	//! @return シミュレーションを終了するならtrue．
	virtual bool IsEnd(const RobotStateNode& node) const = 0;
};

#endif	// DESIGNLAB_INTERFACE_SIMULATION_END_CHECKER_H_