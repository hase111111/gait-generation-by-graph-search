#pragma once

#include <doctest.h>

#include "asyncable_data.h"


TEST_SUITE("AsyncableData")
{
	using namespace designlab;

	TEST_CASE_TEMPLATE("GetData，更新していない時，update_countは変化するべきでない", T, int, float, double)
	{
		SUBCASE("初期化時")
		{
			AsyncableData<T> data;
			CHECK(data.GetData() == T());
			CHECK(data.GetUpdateCount() == 0);
		}

		SUBCASE("初期化時に値を渡した場合")
		{
			AsyncableData<T> data(1);
			CHECK(data.GetData() == 1);
			CHECK(data.GetUpdateCount() == 0);
		}
	}

	TEST_CASE_TEMPLATE("GetData，更新した時，update_countは変化するべき", T, int, float, double)
	{
		SUBCASE("初期化時")
		{
			AsyncableData<T> data;
			data.Update(1);
			CHECK(data.GetData() == 1);
			CHECK(data.GetUpdateCount() == 1);
		}

		SUBCASE("初期化時に値を渡した場合")
		{
			AsyncableData<T> data(1);
			data.Update(2);
			CHECK(data.GetData() == 2);
			CHECK(data.GetUpdateCount() == 1);

			data.Update(3);
			CHECK(data.GetData() == 3);
			CHECK(data.GetUpdateCount() == 2);
		}
	}
}