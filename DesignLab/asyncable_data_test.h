#pragma once

#include <doctest.h>

#include "asyncable_data.h"


TEST_SUITE("AsyncableData::GetUpdateCount")
{
    using namespace designlab;

    TEST_CASE_TEMPLATE("値の更新をしていない時，更新回数は0となるべき", T, int, float, double)
    {
        SUBCASE("初期化時，更新回数は0となるべき")
        {
            AsyncableData<T> data;
            CHECK(data.GetUpdateCount() == 0);
        }

        SUBCASE("初期化時に値を渡した時，更新回数は0となるべき")
        {
            AsyncableData<T> data(1);
            CHECK(data.GetUpdateCount() == 0);
        }

        SUBCASE("データを読み込んだ時，更新回数は0のままであるべき")
        {
            AsyncableData<T> data(1);
            CHECK(data.GetUpdateCount() == 0);
            CHECK(data.GetData() == 1);
            CHECK(data.GetUpdateCount() == 0);
        }
    }

    TEST_CASE_TEMPLATE("値を更新した時，更新回数が増加するべき", T, int, float, double)
    {
        SUBCASE("値の更新を1回した時，更新回数は1となるべき")
        {
            AsyncableData<T> data;
            CHECK(data.GetUpdateCount() == 0);

            data.SetData(1);
            CHECK(data.GetUpdateCount() == 1);
        }

        SUBCASE("値の更新を2回した時，更新回数は2となるべき")
        {
            AsyncableData<T> data;
            CHECK(data.GetUpdateCount() == 0);

            data.SetData(1);
            data.SetData(-1);
            CHECK(data.GetUpdateCount() == 2);
        }
    }
}

TEST_SUITE("AsyncableData::GetUpdateCount")
{
    using namespace designlab;

    TEST_CASE_TEMPLATE("値をセットした時，セットしたデータを取得可能になるべき", T, int, float, double)
    {
        AsyncableData<T> data;

        T act1 = static_cast<T>(1);
        data.SetData(act1);
        CHECK(data.GetData() == act1);

        T act2 = static_cast<T>(-385);
        data.SetData(act2);
        CHECK(data.GetData() == act2);
    }
}