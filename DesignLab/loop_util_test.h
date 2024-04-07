
/// @file      loop_util_test.h
/// @author    hasegawa
/// @copyright (C) 2023 Design Engineering Laboratory, Saitama University All right reserved.

#ifndef DESIGNLAB_LOOP_UTIL_TEST_H_
#define DESIGNLAB_LOOP_UTIL_TEST_H_

#include <doctest.h>

#include "loop_util.h"

TEST_SUITE("DoubleIntRange")
{
    using designlab::DoubleIntRange;

    TEST_CASE("When pram is [3,3]")
    {
        std::vector<std::tuple<int, int>> expected = {
            {0, 0}, {0, 1}, {0, 2}, {1, 0}, {1, 1}, {1, 2}, {2, 0}, {2, 1}, {2, 2},
        };

        std::vector<std::tuple<int, int>> actual;

        for (const auto [i, j] : DoubleIntRange{ 3, 3 })
        {
            actual.push_back(std::make_tuple(i, j));
        }

        CHECK_EQ(expected, actual);
    }

    TEST_CASE("When pram is [2,3]")
    {
        std::vector<std::tuple<int, int>> expected = {
            {0, 0}, {0, 1}, {0, 2}, {1, 0}, {1, 1}, {1, 2},
        };

        std::vector<std::tuple<int, int>> actual;

        for (const auto [i, j] : DoubleIntRange{ 2, 3 })
        {
            actual.push_back(std::make_tuple(i, j));
        }

        CHECK_EQ(expected, actual);
    }

    TEST_CASE("When pram is [3,2]")
    {
        std::vector<std::tuple<int, int>> expected = {
            {0, 0}, {0, 1}, {1, 0}, {1, 1}, {2, 0}, {2, 1},
        };

        std::vector<std::tuple<int, int>> actual;

        for (const auto [i, j] : DoubleIntRange{ 3, 2 })
        {
            actual.push_back(std::make_tuple(i, j));
        }

        CHECK_EQ(expected, actual);
    }
}

#endif  // DESIGNLAB_LOOP_UTIL_TEST_H_
