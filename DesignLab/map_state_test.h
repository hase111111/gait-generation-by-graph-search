
//! @file      map_state_test.h
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#ifndef DESIGNLAB_MAP_STATE_TEST_H_
#define DESIGNLAB_MAP_STATE_TEST_H_

#include <vector>

#include "doctest.h"

#include "map_state.h"


TEST_SUITE("MapState::Constructor")
{
    using designlab::MapState;
    using designlab::Vector3;

    TEST_CASE("デフォルトコンストラクタの呼び出し時，マップのサイズは0になるべき")
    {
        MapState map_state;

        CHECK_EQ(map_state.GetMapPointSize(), 0);
    }

    TEST_CASE("脚接地点を引数に渡した時，マップの要素は引数のと同じになるべき")
    {
        std::vector<Vector3> map_point = { {1, 2, 3}, {4, 5, 6} };

        MapState map_state(map_point);

        SUBCASE("マップのサイズは引数と同じサイズになるべき")
        {
            CHECK(map_state.GetMapPointSize() == map_point.size());
        }

        SUBCASE("追加した順番で脚接地可能点を取得できるべき")
        {
            CHECK(map_state.GetMapPoint(0) == map_point[0]);
            CHECK(map_state.GetMapPoint(1) == map_point[1]);
        }
    }

    TEST_CASE("コピーコンストラクタの呼び出し時，コピー元と同じになるべき")
    {
        SUBCASE("初期化時，コピー元と同じデータサイズになるべき")
        {
            std::vector<Vector3> map_point = { {1, 2, 3}, {4, 5, 6} };

            MapState map_state(map_point);
            MapState map_state_copy(map_state);

            CHECK(map_state_copy.GetMapPointSize() == map_state.GetMapPointSize());
        }

        SUBCASE("初期化時，コピー元と同じデータが取得できるべき")
        {
            std::vector<Vector3> map_point = { {1, 2, 3}, {4, 5, 6} };

            MapState map_state(map_point);
            MapState map_state_copy(map_state);

            CHECK(map_state_copy.GetMapPoint(0) == map_state.GetMapPoint(0));
            CHECK(map_state_copy.GetMapPoint(1) == map_state.GetMapPoint(1));
        }
    }
}

TEST_SUITE("MapState::AssignmentOperator")
{
    using designlab::MapState;
    using designlab::Vector3;

    TEST_CASE("代入した時，代入元と同じサイズになるべき")
    {
        std::vector<Vector3> map_point = { {1, 2, 3}, {4, 5, 6} };

        MapState map_state(map_point);
        MapState map_state_copy = map_state;

        CHECK(map_state_copy.GetMapPointSize() == map_state.GetMapPointSize());
    }

    TEST_CASE("代入した時，代入元と同じデータが取得できるべき")
    {
        std::vector<Vector3> map_point = { {1, 2, 3}, {4, 5, 6} };

        MapState map_state(map_point);
        MapState map_state_copy = map_state;

        CHECK(map_state_copy.GetMapPoint(0) == map_state.GetMapPoint(0));
        CHECK(map_state_copy.GetMapPoint(1) == map_state.GetMapPoint(1));
    }
}

TEST_SUITE("MapState::SetMapPoint")
{
    using designlab::MapState;
    using designlab::Vector3;

    TEST_CASE("0番目にデータをセットした時，マップのサイズは変わるべきでない")
    {
        std::vector<Vector3> map_point = { {1, 2, 3}, {4, 5, 6} };
        Vector3 map_point_set = { 7, 8, 9 };

        MapState map_state(map_point);

        map_state.SetMapPoint(0, map_point_set);

        CHECK(map_state.GetMapPointSize() == map_point.size());
    }

    TEST_CASE("0番目にデータをセットした時，0番の脚接地点が書き換わるべき")
    {
        std::vector<Vector3> map_point = { {1, 2, 3}, {4, 5, 6} };
        Vector3 map_point_set = { 7, 8, 9 };

        MapState map_state(map_point);

        map_state.SetMapPoint(0, map_point_set);

        CHECK(map_state.GetMapPoint(0) == map_point_set);
        CHECK(map_state.GetMapPoint(1) == map_point[1]);
    }
}

TEST_SUITE("MapState::SetMapPointVec")
{
    using designlab::MapState;
    using designlab::Vector3;

    TEST_CASE("3つの脚接地点をセットした時，マップのサイズは3になるべき")
    {
        SUBCASE("マップが空の時")
        {
            std::vector<Vector3> map_point_set = { {7, 8, 9}, {10, 11, 12}, {13, 14, 15} };

            MapState map_state;

            map_state.SetMapPointVec(map_point_set);

            CHECK(map_state.GetMapPointSize() == map_point_set.size());
        }

        SUBCASE("マップにデータがある時")
        {
            std::vector<Vector3> map_point = { {1, 2, 3}, {4, 5, 6} };
            std::vector<Vector3> map_point_set = { {7, 8, 9}, {10, 11, 12}, {13, 14, 15} };

            MapState map_state(map_point);

            map_state.SetMapPointVec(map_point_set);

            CHECK(map_state.GetMapPointSize() == map_point_set.size());
        }
    }

    TEST_CASE("3つの脚接地点をセットした時，0番から順に脚接地点が書き換わるべき")
    {
        SUBCASE("マップが空の時")
        {
            std::vector<Vector3> map_point_set = { {7, 8, 9}, {10, 11, 12}, {13, 14, 15} };

            MapState map_state;

            map_state.SetMapPointVec(map_point_set);

            CHECK(map_state.GetMapPoint(0) == map_point_set[0]);
            CHECK(map_state.GetMapPoint(1) == map_point_set[1]);
            CHECK(map_state.GetMapPoint(2) == map_point_set[2]);
        }

        SUBCASE("マップにデータがある時")
        {
            std::vector<Vector3> map_point = { {1, 2, 3}, {4, 5, 6} };
            std::vector<Vector3> map_point_set = { {7, 8, 9}, {10, 11, 12}, {13, 14, 15} };

            MapState map_state(map_point);

            map_state.SetMapPointVec(map_point_set);

            CHECK(map_state.GetMapPoint(0) == map_point_set[0]);
            CHECK(map_state.GetMapPoint(1) == map_point_set[1]);
            CHECK(map_state.GetMapPoint(2) == map_point_set[2]);
        }
    }
}

TEST_SUITE("MapState::AddMapPoint")
{
    using designlab::MapState;
    using designlab::Vector3;

    TEST_CASE("2回呼び出した時，マップのサイズは2増えるべき")
    {
        std::vector<Vector3> map_point = { {1, 2, 3}, {4, 5, 6} };

        CHECK_EQ(map_point.size(), 2);

        MapState map_state(map_point);

        map_state.AddMapPoint({ 7, 8, 9 });
        map_state.AddMapPoint({ 10, 11, 12 });

        CHECK_EQ(map_state.GetMapPointSize(), 4);
    }

    TEST_CASE("2回呼び出した時，追加した順番で脚接地可能点を取得できるべき")
    {
        std::vector<Vector3> map_point = { {1, 2, 3}, {4, 5, 6} };

        MapState map_state(map_point);

        map_state.AddMapPoint({ 7, 8, 9 });
        map_state.AddMapPoint({ 10, 11, 12 });

        CHECK_EQ(map_state.GetMapPoint(0), map_point[0]);
        CHECK_EQ(map_state.GetMapPoint(1), map_point[1]);
        CHECK_EQ(map_state.GetMapPoint(2), Vector3(7, 8, 9));
        CHECK_EQ(map_state.GetMapPoint(3), Vector3(10, 11, 12));
    }
}

TEST_SUITE("MapState::ClearMapPoint")
{
    using designlab::MapState;
    using designlab::Vector3;

    TEST_CASE("ClearMapPoint")
    {
        SUBCASE("2つの脚接地点を消去した時，マップのサイズは0になるべき")
        {
            std::vector<Vector3> map_point = { {1, 2, 3}, {4, 5, 6} };

            MapState map_state(map_point);

            map_state.ClearMapPoint();

            CHECK_EQ(map_state.GetMapPointSize(), 0);
        }
    }
}

#endif  // DESIGNLAB_MAP_STATE_TEST_H_
