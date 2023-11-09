#include "pch.h"

#include "../DesignLab/map_state.h"
#include "../DesignLab/devide_map_state.h"
#include "../DesignLab/devide_map_state.cpp"


namespace dl = ::designlab;


namespace designlab::test::map
{
	TEST(MapStateTest, DefaultConstructorTest)
	{
		MapState map_state;

		EXPECT_EQ(map_state.GetMapPointSize(), 0) << "値は入っていない．";
	}

	TEST(MapStateTest, ConstructorTest)
	{
		std::vector<dl::Vector3> map_point_list{
			dl::Vector3(1, 2, 3),
				dl::Vector3(4, 5, 6),
				dl::Vector3(7, 8, 9),
		};

		MapState map_state(map_point_list);

		EXPECT_EQ(map_state.GetMapPointSize(), 3) << "値は入っている．";

		EXPECT_EQ(map_state.GetMapPoint(0), map_point_list[0]);
		EXPECT_EQ(map_state.GetMapPoint(1), map_point_list[1]);
		EXPECT_EQ(map_state.GetMapPoint(2), map_point_list[2]);
	}

	TEST(MapStateTest, CopyConstructorTest)
	{
		std::vector<dl::Vector3> map_point_list{
			dl::Vector3(1, 2, 3),
			dl::Vector3(4, 5, 6),
			dl::Vector3(7, 8, 9),
		};

		const MapState map_state(map_point_list);

		MapState map_state_copy(map_state);

		EXPECT_EQ(map_state_copy.GetMapPointSize(), 3) << "値は入っている．";

		EXPECT_EQ(map_state_copy.GetMapPoint(0), map_point_list[0]);
		EXPECT_EQ(map_state_copy.GetMapPoint(1), map_point_list[1]);
		EXPECT_EQ(map_state_copy.GetMapPoint(2), map_point_list[2]);
	}

	TEST(MapStateTest, MoveConstructorTest)
	{
		std::vector<dl::Vector3> map_point_list{
			dl::Vector3(1, 2, 3),
			dl::Vector3(4, 5, 6),
			dl::Vector3(7, 8, 9),
		};

		MapState map_state(map_point_list);

		MapState map_state_move(std::move(map_state));

		EXPECT_EQ(map_state_move.GetMapPointSize(), 3) << "値は入っている．";

		EXPECT_EQ(map_state_move.GetMapPoint(0), map_point_list[0]);
		EXPECT_EQ(map_state_move.GetMapPoint(1), map_point_list[1]);
		EXPECT_EQ(map_state_move.GetMapPoint(2), map_point_list[2]);
	}

	TEST(MapStateTest, CopyAssignmentTest)
	{
		std::vector<dl::Vector3> map_point_list{
			dl::Vector3(1, 2, 3),
			dl::Vector3(4, 5, 6),
			dl::Vector3(7, 8, 9),
		};

		const MapState map_state(map_point_list);

		MapState map_state_copy;
		map_state_copy = map_state;

		EXPECT_EQ(map_state_copy.GetMapPointSize(), 3) << "値は入っている．";

		EXPECT_EQ(map_state_copy.GetMapPoint(0), map_point_list[0]);
		EXPECT_EQ(map_state_copy.GetMapPoint(1), map_point_list[1]);
		EXPECT_EQ(map_state_copy.GetMapPoint(2), map_point_list[2]);
	}

} // namespace designlab::test::map_state