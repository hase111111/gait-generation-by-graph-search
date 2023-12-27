#include "pch.h"

#include "../DesignLab/robot_state_node.h"


namespace dl = ::designlab;
namespace dllf = ::designlab::leg_func;


namespace designlab::test::node
{
	TEST(RobotStateNodeTest, ChangeParentNodeTest)
	{
		const RobotStateNode node(
			dllf::MakeLegStateBit(DiscreteComPos::kCenterFront, { true, true, false, true, false, true }, {}),
			dl::MakeArray<dl::Vector3>(dl::Vector3{ 10,10,10 }, dl::Vector3{ 10,10,10 }, dl::Vector3{ 10,10,10 }, dl::Vector3{ 10,10,10 }, dl::Vector3{ 10,10,10 }, dl::Vector3{ 10,10,10 }),
			dl::MakeArray<dl::Vector3>(dl::Vector3{ 10,10,10 }, dl::Vector3{ 10,10,10 }, dl::Vector3{ 10,10,10 }, dl::Vector3{ 10,10,10 }, dl::Vector3{ 10,10,10 }, dl::Vector3{ 10,10,10 }),
			dl::Vector3{ 300, 50, -10 },
			dl::Quaternion{ 1.0f, 0.0f, 0.0f, 0.0f },
			HexapodMove::kLegHierarchyChange,
			315,
			5
		);

		EXPECT_EQ(node.parent_index, 315);
		EXPECT_EQ(node.depth, 5);

		RobotStateNode changeed = node;

		changeed.ChangeLootNode();

		EXPECT_EQ(changeed.parent_index, -1) << "親がいないならば，indexは-1です．";
		EXPECT_EQ(changeed.depth, 0) << "親ノードの深さは0です．";

		const std::string error_mes = "その他の値は変更されません．";
		EXPECT_EQ(node.leg_state, changeed.leg_state) << error_mes;
		EXPECT_EQ(node.leg_pos, changeed.leg_pos) << error_mes;
		EXPECT_EQ(node.leg_reference_pos, changeed.leg_reference_pos) << error_mes;
		EXPECT_EQ(node.center_of_mass_global_coord, changeed.center_of_mass_global_coord) << error_mes;
		EXPECT_EQ(node.quat, changeed.quat) << error_mes;
		EXPECT_EQ(node.next_move, changeed.next_move) << error_mes;
	}

	TEST(RobotStateNodeTest, ChangeToNextNodeTest)
	{
		const RobotStateNode node(
			dllf::MakeLegStateBit(DiscreteComPos::kCenterFront, { true, true, false, true, false, true }, {}),
			dl::MakeArray<dl::Vector3>(dl::Vector3{ 10,10,10 }, dl::Vector3{ 10,10,10 }, dl::Vector3{ 10,10,10 }, dl::Vector3{ 10,10,10 }, dl::Vector3{ 10,10,10 }, dl::Vector3{ 10,10,10 }),
			dl::MakeArray<dl::Vector3>(dl::Vector3{ 10,10,10 }, dl::Vector3{ 10,10,10 }, dl::Vector3{ 10,10,10 }, dl::Vector3{ 10,10,10 }, dl::Vector3{ 10,10,10 }, dl::Vector3{ 10,10,10 }),
			dl::Vector3{ 300, 50, -10 },
			dl::Quaternion{ 1.0f, 0.0f, 0.0f, 0.0f },
			HexapodMove::kLegHierarchyChange,
			315,
			5
		);

		EXPECT_EQ(node.next_move, HexapodMove::kLegHierarchyChange);
		EXPECT_EQ(node.parent_index, 315);
		EXPECT_EQ(node.depth, 5);

		RobotStateNode changeed = node;
		const int parent_index = 1000;
		const HexapodMove next_move = HexapodMove::kComMove;

		changeed.ChangeToNextNode(parent_index, next_move);

		EXPECT_EQ(changeed.next_move, next_move) << "次の動作は変更されません．";
		EXPECT_EQ(changeed.parent_index, parent_index) << "親は変更されません．";
		EXPECT_EQ(changeed.depth, node.depth + 1) << "深さは変更されません．";

		const std::string error_mes = "その他の値は変更されます．";
		EXPECT_EQ(node.leg_state, changeed.leg_state) << error_mes;
		EXPECT_EQ(node.leg_pos, changeed.leg_pos) << error_mes;
		EXPECT_EQ(node.leg_reference_pos, changeed.leg_reference_pos) << error_mes;
		EXPECT_EQ(node.center_of_mass_global_coord, changeed.center_of_mass_global_coord) << error_mes;
		EXPECT_EQ(node.quat, changeed.quat) << error_mes;
	}
}