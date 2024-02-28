
//! @file      node_creator_leg_hierarchy_test.h
//! @author    hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2024. All right reserved.

#ifndef DESIGNLAB_NODE_CREATOR_LEG_HIERARCHY_TEST_H_
#define DESIGNLAB_NODE_CREATOR_LEG_HIERARCHY_TEST_H_

#include <array>
#include <vector>

#include "doctest.h"

#include "node_creator_leg_hierarchy.h"


TEST_SUITE("NodeCreatorLegHierarchyTest")
{
    using designlab::NodeCreatorLegHierarchy;
    using designlab::RobotStateNode;
    using designlab::enums::HexapodMove;
    using designlab::HexapodConst;
    using designlab::enums::DiscreteLegPos;
    using designlab::enums::DiscreteComPos;
    using designlab::leg_func::MakeLegStateBit;

    TEST_CASE("CreateTest_WhenOneLegIsSwinging")
    {
        using enum DiscreteLegPos;

        // Arrange
        const HexapodMove move = HexapodMove::kComMove;
        const std::vector<DiscreteLegPos> discrete_leg_pos = {
            kBack, kCenter, kFront, };

        NodeCreatorLegHierarchy node_creator_leg_hierarchy{
            move, discrete_leg_pos };

        // 1脚のみ遊脚させる．
        std::array<bool, HexapodConst::kLegNum> ground_leg = {
            false, true, true, true, true, true };

        RobotStateNode robot_state_node;

        robot_state_node.leg_state = MakeLegStateBit(
            DiscreteComPos::kCenterFront,
            ground_leg,
            { kCenter, kCenter, kCenter, kCenter, kCenter, kCenter });

        // Act
        std::vector<RobotStateNode> actual;

        node_creator_leg_hierarchy.Create(robot_state_node, 0, &actual);

        // Assert
        CHECK(actual.size() == 3);


    }
}

#endif  // DESIGNLAB_NODE_CREATOR_LEG_HIERARCHY_TEST_H_
