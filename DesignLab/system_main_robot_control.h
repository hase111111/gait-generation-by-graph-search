
/// @file      system_main_robot_control.h
/// @author    hasegawa
/// @copyright © 埼玉大学 設計工学研究室 2024. All right reserved.

#ifndef DESIGNLAB_SYSTEM_MAIN_ROBOT_CONTROL_H_
#define DESIGNLAB_SYSTEM_MAIN_ROBOT_CONTROL_H_

#include "interface_system_main.h"

#include "graphic_data_broker.h"


namespace designlab
{

class SystemMainRobotControl final : public ISystemMain
{
public:
    SystemMainRobotControl();
    ~SystemMainRobotControl() = default;

    void Main() override;

};

}  // namespace designlab

#endif  // DESIGNLAB_SYSTEM_MAIN_ROBOT_CONTROL_H_
