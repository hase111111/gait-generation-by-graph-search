
//! @file      simulation_map_parameter_validator_test.h
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#ifndef DESIGNLAB_SIMULATION_MAP_PARAMETER_VALIDATOR_TEST_H_
#define DESIGNLAB_SIMULATION_MAP_PARAMETER_VALIDATOR_TEST_H_

#include <doctest.h>

#include "simulation_map_parameter_validator.h"


TEST_SUITE("SimulationMapParameterValidator::Validate")
{
    using designlab::SimulationMapParameter;
    using designlab::SimulationMapParameterValidator;

    SimulationMapParameterValidator validator;

    TEST_CASE("ホール率が0未満の時，falseが返るべき")
    {
        SUBCASE("ホール率が-1の時，falseが返るべき")
        {
            SimulationMapParameter parameter;
            parameter.hole_rate = -1;
            const auto [result, _] = validator.Validate(parameter);

            CHECK_FALSE(result);
        }

        SUBCASE("ホール率が0の時，trueが返るべき")
        {
            // 境界値テスト．
            SimulationMapParameter parameter;
            parameter.hole_rate = 0;
            const auto [result, _] = validator.Validate(parameter);

            CHECK(result);
        }
    }

    TEST_CASE("ホール率が100より大きい時，falseが返るべき")
    {
        SUBCASE("ホール率が101の時，falseが返るべき")
        {
            SimulationMapParameter parameter;
            parameter.hole_rate = 101;
            const auto [result, _] = validator.Validate(parameter);

            CHECK_FALSE(result);
        }

        SUBCASE("ホール率が100の時，trueが返るべき")
        {
            // 境界値テスト．
            SimulationMapParameter parameter;
            parameter.hole_rate = 100;
            const auto [result, _] = validator.Validate(parameter);

            CHECK(result);
        }
    }

    TEST_CASE("不整地の最小値が最大値を上回っている時，falseを返すべき")
    {
        SUBCASE("不整地の最小値が最大値より大きい時，falseを返すべき")
        {
            SimulationMapParameter parameter;
            parameter.rough_min_height = 10.f;
            parameter.rough_max_height = -10.f;
            const auto [result, _] = validator.Validate(parameter);

            CHECK_FALSE(result);
        }

        SUBCASE("不整地の最小値が最大値と等しい時，trueを返すべき")
        {
            // 境界値テスト．
            SimulationMapParameter parameter;
            parameter.rough_min_height = 10.f;
            parameter.rough_max_height = 10.f;
            const auto [result, _] = validator.Validate(parameter);

            CHECK(result);
        }
    }

    TEST_CASE("ストライプの間隔が0未満の時，falseを返すべき")
    {
        SUBCASE("ストライプの間隔が-1の時，falseを返すべき")
        {
            SimulationMapParameter parameter;
            parameter.stripe_interval = -1;
            const auto [result, _] = validator.Validate(parameter);

            CHECK_FALSE(result);
        }

        SUBCASE("ストライプの間隔が0の時，trueを返すべき")
        {
            // 境界値テスト．
            SimulationMapParameter parameter;
            parameter.stripe_interval = 0;
            const auto [result, _] = validator.Validate(parameter);

            CHECK(result);
        }
    }

    TEST_CASE("階段の奥行が0未満の時，falseを返すべき")
    {
        SUBCASE("階段の奥行が-1の時，falseを返すべき")
        {
            SimulationMapParameter parameter;
            parameter.step_length = -1.f;
            const auto [result, _] = validator.Validate(parameter);

            CHECK_FALSE(result);
        }

        SUBCASE("階段の奥行が0の時，trueを返すべき")
        {
            // 境界値テスト．
            SimulationMapParameter parameter;
            parameter.step_length = 0.f;
            const auto [result, _] = validator.Validate(parameter);

            CHECK(result);
        }
    }
}

#endif  // DESIGNLAB_SIMULATION_MAP_PARAMETER_VALIDATOR_TEST_H_
