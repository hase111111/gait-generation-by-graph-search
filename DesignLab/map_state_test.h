#pragma once

#include <doctest.h>

#include "map_state.h"

using namespace designlab;

TEST_SUITE("MapStateTest")
{
	TEST_CASE("DefaultConstructor_呼び出し時_マップのサイズは0になるべき")
	{
		MapState map_state;

		CHECK(map_state.GetMapPointSize() == 0);
	}

	TEST_CASE("Constructor_std::vector<Vector3>で初期化時_マップの要素は引数のvectorと同じになるべき")
	{
		std::vector<designlab::Vector3> map_point = { {1, 2, 3}, {4, 5, 6} };

		MapState map_state(map_point);

		SUBCASE("初期化時_マップのサイズは引数のvectorと同じサイズになるべき")
		{
			CHECK(map_state.GetMapPointSize() == map_point.size());
		}

		SUBCASE("初期化時_追加した順番で脚接地可能点を取得できるべき")
		{
			CHECK(map_state.GetMapPoint(0) == map_point[0]);
			CHECK(map_state.GetMapPoint(1) == map_point[1]);
		}
	}

	TEST_CASE("CopyConstructor")
	{
		SUBCASE("初期化時，コピー元と同じデータサイズになる")
		{
			std::vector<designlab::Vector3> map_point = { {1, 2, 3}, {4, 5, 6} };

			MapState map_state(map_point);
			MapState map_state_copy(map_state);

			CHECK(map_state_copy.GetMapPointSize() == map_state.GetMapPointSize());
		}

		SUBCASE("初期化時，コピー元と同じデータが取得できる")
		{
			std::vector<designlab::Vector3> map_point = { {1, 2, 3}, {4, 5, 6} };

			MapState map_state(map_point);
			MapState map_state_copy(map_state);

			CHECK(map_state_copy.GetMapPoint(0) == map_state.GetMapPoint(0));
			CHECK(map_state_copy.GetMapPoint(1) == map_state.GetMapPoint(1));
		}
	}

	TEST_CASE("AssignmentOperator")
	{
		SUBCASE("代入時，代入元と同じデータサイズになる")
		{
			std::vector<designlab::Vector3> map_point = { {1, 2, 3}, {4, 5, 6} };

			MapState map_state(map_point);
			MapState map_state_copy = map_state;

			CHECK(map_state_copy.GetMapPointSize() == map_state.GetMapPointSize());
		}

		SUBCASE("代入時，代入元と同じデータが取得できる")
		{
			std::vector<designlab::Vector3> map_point = { {1, 2, 3}, {4, 5, 6} };

			MapState map_state(map_point);
			MapState map_state_copy = map_state;

			CHECK(map_state_copy.GetMapPoint(0) == map_state.GetMapPoint(0));
			CHECK(map_state_copy.GetMapPoint(1) == map_state.GetMapPoint(1));
		}
	}

	TEST_CASE("SetMapPoint")
	{
		SUBCASE("2つの脚接地点を持っている時に，0番をSetすると，マップのサイズは変わらない")
		{
			std::vector<designlab::Vector3> map_point = { {1, 2, 3}, {4, 5, 6} };
			designlab::Vector3 map_point_set = { 7, 8, 9 };

			MapState map_state(map_point);

			map_state.SetMapPoint(0, map_point_set);

			CHECK(map_state.GetMapPointSize() == map_point.size());
		}

		SUBCASE("2つの脚接地点を持っている時に，0番をSetすると，0番の脚接地点が書き換わる")
		{
			std::vector<designlab::Vector3> map_point = { {1, 2, 3}, {4, 5, 6} };
			designlab::Vector3 map_point_set = { 7, 8, 9 };

			MapState map_state(map_point);

			map_state.SetMapPoint(0, map_point_set);

			CHECK(map_state.GetMapPoint(0) == map_point_set);
			CHECK(map_state.GetMapPoint(1) == map_point[1]);
		}

	}

	TEST_CASE("SetMapPointVec")
	{
		SUBCASE("2つの脚接地点を持っている時に，3つの脚接地点のvectorをSetすると，マップのサイズは3になる")
		{
			std::vector<designlab::Vector3> map_point = { {1, 2, 3}, {4, 5, 6} };
			std::vector<designlab::Vector3> map_point_set = { {7, 8, 9}, {10, 11, 12}, {13, 14, 15} };

			MapState map_state(map_point);

			map_state.SetMapPointVec(map_point_set);

			CHECK(map_state.GetMapPointSize() == map_point_set.size());
		}

		SUBCASE("2つの脚接地点を持っている時に，3つの脚接地点のvectorをSetすると，0番から順に脚接地点が書き換わる")
		{
			std::vector<designlab::Vector3> map_point = { {1, 2, 3}, {4, 5, 6} };
			std::vector<designlab::Vector3> map_point_set = { {7, 8, 9}, {10, 11, 12}, {13, 14, 15} };

			MapState map_state(map_point);

			map_state.SetMapPointVec(map_point_set);

			CHECK(map_state.GetMapPoint(0) == map_point_set[0]);
			CHECK(map_state.GetMapPoint(1) == map_point_set[1]);
			CHECK(map_state.GetMapPoint(2) == map_point_set[2]);
		}
	}

	TEST_CASE("AddMapPoint")
	{
		SUBCASE("2回呼び出すと，マップのサイズは2増える")
		{
			std::vector<designlab::Vector3> map_point = { {1, 2, 3}, {4, 5, 6} };

			MapState map_state(map_point);

			map_state.AddMapPoint({ 7, 8, 9 });
			map_state.AddMapPoint({ 10, 11, 12 });

			CHECK(map_state.GetMapPointSize() == 4);
		}

		SUBCASE("2回呼び出すと，追加した順番で脚接地可能点を取得できる")
		{
			std::vector<designlab::Vector3> map_point = { {1, 2, 3}, {4, 5, 6} };

			MapState map_state(map_point);

			map_state.AddMapPoint({ 7, 8, 9 });
			map_state.AddMapPoint({ 10, 11, 12 });

			CHECK(map_state.GetMapPoint(0) == map_point[0]);
			CHECK(map_state.GetMapPoint(1) == map_point[1]);
			CHECK(map_state.GetMapPoint(2) == designlab::Vector3(7, 8, 9));
			CHECK(map_state.GetMapPoint(3) == designlab::Vector3(10, 11, 12));
		}
	}

	TEST_CASE("ClearMapPoint")
	{
		SUBCASE("2つの脚接地点を持っている時に，呼び出すと，マップのサイズは0になる")
		{
			std::vector<designlab::Vector3> map_point = { {1, 2, 3}, {4, 5, 6} };

			MapState map_state(map_point);

			map_state.ClearMapPoint();

			CHECK(map_state.GetMapPointSize() == 0);
		}
	}
}