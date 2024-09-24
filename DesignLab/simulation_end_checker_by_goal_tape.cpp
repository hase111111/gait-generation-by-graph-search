
//! @file simulation_end_checker_by_goal_tape.cpp

// Copyright(c) 2023 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#include "simulation_end_checker_by_goal_tape.h"


namespace designlab
{

SimulationEndCheckerByGoalTape::SimulationEndCheckerByGoalTape(const float goal_tape_x) :
    kGoalTape(goal_tape_x)
{
}

bool SimulationEndCheckerByGoalTape::IsEnd(const RobotStateNode& node) const
{
    if (node.center_of_mass_global_coord.x > kGoalTape)
    {
        return true;
    }
    else
    {
        return false;
    }
}

}  // namespace designlab
