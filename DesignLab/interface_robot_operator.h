
//! @file      interface_robot_operator.h
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#ifndef DESIGNLAB_INTERFACE_ROBOT_OPERATOR_H_
#define DESIGNLAB_INTERFACE_ROBOT_OPERATOR_H_

#include "robot_state_node.h"
#include "robot_operation.h"


namespace designlab
{

//! @class IRobotOperator
//! @brief ロボットの動作を決定する処理のインターフェース．
class IRobotOperator
{
public:
    virtual ~IRobotOperator() = default;

    //! @brief ロボットの動作を初期化する．
    //! @return ロボットの動作．
    virtual RobotOperation Init() const = 0;

    //! @brief ロボットの動作を更新する．
    //! @param[in] state ロボットの現在の状態．
    //! @return ロボットの動作．
    virtual RobotOperation Update(const RobotStateNode& state) = 0;
};

}  // namespace designlab


#endif  // DESIGNLAB_INTERFACE_ROBOT_OPERATOR_H_
