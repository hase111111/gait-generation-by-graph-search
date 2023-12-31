
//! @file      interface_simulation_end_checker.h
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#ifndef DESIGNLAB_INTERFACE_SIMULATION_END_CHECKER_H_
#define DESIGNLAB_INTERFACE_SIMULATION_END_CHECKER_H_

#include "robot_state_node.h"


namespace designlab
{

//! @class ISimulationEndChecker
//! @brief シミュレーションの終了を判定するクラスのインターフェース．
class ISimulationEndChecker
{
public:
    virtual ~ISimulationEndChecker() = default;

    //! @brief シミュレーションの終了を判定する．
    //! @param[in] node ロボットの状態．
    //! @return シミュレーションを終了するなら true．
    virtual bool IsEnd(const RobotStateNode& node) const = 0;
};

}  // namespace designlab


#endif  // DESIGNLAB_INTERFACE_SIMULATION_END_CHECKER_H_
