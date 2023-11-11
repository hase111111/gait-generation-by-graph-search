#include "pch.h"

#include "../DesignLab/robot_state_node.h"
#include "../DesignLab/robot_state_node.cpp"


namespace dl = ::designlab;
namespace dllf = ::designlab::leg_func;


namespace designlab::test::node 
{
	TEST(RobotStateNodeTest, DefaultConstructorTest)
	{
		RobotStateNode node;
		const auto zero_vec = dl::Vector3{0.f, 0.f, 0.f};
		const auto zero_leg_pos = dl::MakeArray<dl::Vector3>(zero_vec, zero_vec, zero_vec, zero_vec, zero_vec, zero_vec);
		const auto zero_rot = dl::EulerXYZ{0.f, 0.f, 0.f};

		EXPECT_EQ(node.leg_state.to_ullong(), 0);
		EXPECT_EQ(node.leg_pos, zero_leg_pos);
		EXPECT_EQ(node.leg_reference_pos, zero_leg_pos);
		EXPECT_EQ(node.global_center_of_mass, zero_vec);
		EXPECT_EQ(node.rot, zero_rot);
		EXPECT_EQ(node.next_move, HexapodMove::kComUpDown);
		EXPECT_EQ(node.parent_num, -1);
		EXPECT_EQ(node.depth, 0);
	}

	TEST(RobotStateNodeTest, ConstructorTest)
	{

		const auto leg_state = dllf::MakeLegStateBit(
			DiscreteComPos::kCenterBack, 
			{true, false, true, false, true, false},
			{DiscreteLegPos::kCenter, DiscreteLegPos::kBack, DiscreteLegPos::kLowerFront, 
			DiscreteLegPos::kUpperBack, DiscreteLegPos::kUpperFront, DiscreteLegPos::kLowerBack}
		);
		const auto leg_pos = dl::MakeArray<dl::Vector3>(
			dl::Vector3{ 1.f, 2.f, 3.f },
			dl::Vector3{ 4.f, 5.f, 6.f },
			dl::Vector3{ 7.f, 8.f, 9.f },
			dl::Vector3{ 10.f, 11.f, 12.f },
			dl::Vector3{ 13.f, 14.f, 15.f },
			dl::Vector3{ 16.f, 17.f, 18.f }
		);
		const auto leg_reference_pos = dl::MakeArray<dl::Vector3>(
			dl::Vector3{ 19.f, 20.f, 21.f },
			dl::Vector3{ 22.f, 23.f, 24.f },
			dl::Vector3{ 25.f, 26.f, 27.f },
			dl::Vector3{ 28.f, 29.f, 30.f },
			dl::Vector3{ 31.f, 32.f, 33.f },
			dl::Vector3{ 34.f, 35.f, 36.f }
		);
		const auto global_center_of_mass = dl::Vector3{ 37.f, 38.f, 39.f };
		const auto rot = dl::EulerXYZ{ 40.f, 41.f, 42.f };
		const HexapodMove next_move = HexapodMove::kComUpDown;
		const int parent_num = 43;
		const int depth = 44;

		RobotStateNode node{
			leg_state,
			leg_pos,
			leg_reference_pos,
			global_center_of_mass,
			rot,
			next_move,
			parent_num,
			depth
		};

		EXPECT_EQ(node.leg_state, leg_state);
		EXPECT_EQ(node.leg_pos, leg_pos);
		EXPECT_EQ(node.leg_reference_pos, leg_reference_pos);
		EXPECT_EQ(node.global_center_of_mass, global_center_of_mass);
		EXPECT_EQ(node.rot, rot);
		EXPECT_EQ(node.next_move, next_move);
		EXPECT_EQ(node.parent_num, parent_num);
		EXPECT_EQ(node.depth, depth);
	}
}