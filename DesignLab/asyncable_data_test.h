#pragma once

#include <doctest.h>

#include "asyncable_data.h"


TEST_SUITE("AsyncableData")
{
	using namespace designlab;

	TEST_CASE_TEMPLATE("GetUpdateCount，更新していない時，update_countは変化するべきでない", T, int, float, double)
	{
		SUBCASE("初期化時，update_countは0となるべき")
		{
			AsyncableData<T> data;
			CHECK(data.GetUpdateCount() == 0);
		}

		SUBCASE("初期化時に値を渡した時，，update_countは0となるべき")
		{
			AsyncableData<T> data(1);
			CHECK(data.GetUpdateCount() == 0);
		}

		SUBCASE("GetDataを呼んだ時，update_countは変化するべきでない")
		{
			AsyncableData<T> data(1);
			CHECK(data.GetUpdateCount() == 0);
			CHECK(data.GetData() == 1);
			CHECK(data.GetUpdateCount() == 0);
		}
	}

	TEST_CASE_TEMPLATE("GetUpdateCount，値を更新した時，update_countは変化するべき", T, int, float, double)
	{
		SUBCASE("SetDataを1回呼んだ時，update_countは1となるべき")
		{
			AsyncableData<T> data;
			CHECK(data.GetUpdateCount() == 0);

			data.SetData(1);
			CHECK(data.GetUpdateCount() == 1);
		}

		SUBCASE("SetDataを2回呼んだ時，update_countは2となるべき")
		{
			AsyncableData<T> data;
			CHECK(data.GetUpdateCount() == 0);

			data.SetData(1);
			data.SetData(-1);
			CHECK(data.GetUpdateCount() == 2);
		}
	}
}