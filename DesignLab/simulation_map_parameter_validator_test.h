#pragma once

#include <doctest.h>

#include "simulation_map_parameter_validator.h"


TEST_SUITE("SimulationMapParameterValidator")
{
	SimulationMapParameterValidator validator;

	TEST_CASE("Validate_ホール率が0未満の時_falseが返るべき")
	{
		SUBCASE("ホール率が-1の時_falseが返るべき")
		{
			SimulationMapParameter parameter;
			parameter.hole_rate = -1;
			const auto [result, message] = validator.Validate(parameter);

			CHECK_FALSE(result);
		}

		SUBCASE("ホール率が0の時_trueが返るべき")
		{
			SimulationMapParameter parameter;
			parameter.hole_rate = 0;
			const auto [result, message] = validator.Validate(parameter);

			CHECK(result);
		}
	}

	TEST_CASE("Validate_ホール率が100より大きい時_falseが返るべき")
	{
		SUBCASE("ホール率が101の時_falseが返るべき")
		{
			SimulationMapParameter parameter;
			parameter.hole_rate = 101;
			const auto [result, message] = validator.Validate(parameter);

			CHECK_FALSE(result);
		}

		SUBCASE("ホール率が100の時_trueが返るべき")
		{
			SimulationMapParameter parameter;
			parameter.hole_rate = 100;
			const auto [result, message] = validator.Validate(parameter);

			CHECK(result);
		}
	}
}
