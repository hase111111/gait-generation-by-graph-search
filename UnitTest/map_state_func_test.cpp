#include "pch.h"

#include "../DesignLab/map_state.h"


namespace dl = ::designlab;


namespace designlab::test::map
{
	TEST(MapStateTest, SetMapPointTest)
	{
		std::vector<dl::Vector3> map_point_list{
			dl::Vector3(1, 2, 3),
			dl::Vector3(4, 5, 6),
			dl::Vector3(7, 8, 9),
		};

		MapState map_state(map_point_list);

		EXPECT_EQ(map_state.GetMapPointSize(), 3);

		EXPECT_EQ(map_state.GetMapPoint(0), map_point_list[0]);
		EXPECT_EQ(map_state.GetMapPoint(1), map_point_list[1]);
		EXPECT_EQ(map_state.GetMapPoint(2), map_point_list[2]);

		map_state.SetMapPoint(0, dl::Vector3(10, 11, 12));
		map_state.SetMapPoint(1, dl::Vector3(13, 14, 15));
		map_state.SetMapPoint(2, dl::Vector3(16, 17, 18));

		EXPECT_EQ(map_state.GetMapPointSize(), 3);

		EXPECT_EQ(map_state.GetMapPoint(0), dl::Vector3(10, 11, 12));
		EXPECT_EQ(map_state.GetMapPoint(1), dl::Vector3(13, 14, 15));
		EXPECT_EQ(map_state.GetMapPoint(2), dl::Vector3(16, 17, 18));
	}

	TEST(MapStateTest, SetMapPointVecTest)
	{
		std::vector<dl::Vector3> map_point_list{
			dl::Vector3(1, 2, 3),
				dl::Vector3(4, 5, 6),
				dl::Vector3(7, 8, 9),
		};

		MapState map_state(map_point_list);

		EXPECT_EQ(map_state.GetMapPointSize(), 3);

		EXPECT_EQ(map_state.GetMapPoint(0), map_point_list[0]);
		EXPECT_EQ(map_state.GetMapPoint(1), map_point_list[1]);
		EXPECT_EQ(map_state.GetMapPoint(2), map_point_list[2]);

		std::vector<dl::Vector3> new_map_point_list{
			dl::Vector3(10, 11, 12),
			dl::Vector3(13, 14, 15),
			dl::Vector3(16, 17, 18),
		};

		map_state.SetMapPointVec(new_map_point_list);

		EXPECT_EQ(map_state.GetMapPointSize(), 3);

		EXPECT_EQ(map_state.GetMapPoint(0), new_map_point_list[0]);
		EXPECT_EQ(map_state.GetMapPoint(1), new_map_point_list[1]);
		EXPECT_EQ(map_state.GetMapPoint(2), new_map_point_list[2]);
	}

	TEST(MapStateTest, AddMapPointTest)
	{
		std::vector<dl::Vector3> map_point_list{
			dl::Vector3(1, 2, 3),
			dl::Vector3(4, 5, 6),
			dl::Vector3(7, 8, 9)
		};

		MapState map_state;

		EXPECT_EQ(map_state.GetMapPointSize(), 0);

		map_state.AddMapPoint(map_point_list[0]);
		map_state.AddMapPoint(map_point_list[1]);
		map_state.AddMapPoint(map_point_list[2]);


		EXPECT_EQ(map_state.GetMapPointSize(), 3);

		EXPECT_EQ(map_state.GetMapPoint(0), map_point_list[0]);
		EXPECT_EQ(map_state.GetMapPoint(1), map_point_list[1]);
		EXPECT_EQ(map_state.GetMapPoint(2), map_point_list[2]);
	}
}