
/// @file      asyncable_data_test.h
/// @author    Hasegawa
/// @copyright © 埼玉大学 設計工学研究室 2024. All right reserved.

#ifndef DESIGNLAB_ASYNCABLE_DATA_TEST_H_
#define DESIGNLAB_ASYNCABLE_DATA_TEST_H_

#include <doctest.h>

#include "asyncable_data.h"


TEST_SUITE("AsyncableData")
{
    using designlab::AsyncableData;

    TEST_CASE_TEMPLATE("GetUpdateCountTest_WhenValueIsNotUpdated_ShouldBeZeroForUpdateCount",
                       T, int, float, double, unsigned int)
    {
        SUBCASE("WhenInitialized")
        {
            AsyncableData<T> data;
            CHECK_EQ(data.GetUpdateCount(), 0);
        }

        SUBCASE("WhenValueIsPassedUponInitialization")
        {
            AsyncableData<T> data(1);
            CHECK_EQ(data.GetUpdateCount(), 0);
        }

        SUBCASE("WhenOnlyReadingValues")
        {
            AsyncableData<T> data(1);

            T act = data.GetData();

            CHECK_EQ(data.GetUpdateCount(), 0);

            CHECK_EQ(act, 1);   // 警告の抑制．
        }
    }

    TEST_CASE_TEMPLATE("GetUpdateCountTest_WhenValueIsUpdated_ShouldIncrementUpdateCount",
                       T, int, float, double, unsigned int)
    {
        SUBCASE("WhenValueIsUpdatedOnce_ShouldUpdateCountBeOne")
        {
            AsyncableData<T> data;
            CHECK_EQ(data.GetUpdateCount(), 0);

            data.SetData(1);
            CHECK_EQ(data.GetUpdateCount(), 1);
        }

        SUBCASE("WhenValueIsUpdatedTwice_ShouldUpdateCountBeTwo")
        {
            AsyncableData<T> data;
            CHECK_EQ(data.GetUpdateCount(), 0);

            data.SetData(1);
            data.SetData(5);
            CHECK_EQ(data.GetUpdateCount(), 2);
        }
    }

    TEST_CASE_TEMPLATE("GetDataTest_WhenValueIsSet_ShouldBeAbleToRetrieveSetValue",
                       T, int, float, double, unsigned int)
    {
        AsyncableData<T> data;

        T act1 = static_cast<T>(1);
        data.SetData(act1);
        CHECK_EQ(data.GetData(), act1);

        T act2 = static_cast<T>(5);
        data.SetData(act2);
        CHECK_EQ(data.GetData(), act2);
    }
}

#endif  // DESIGNLAB_ASYNCABLE_DATA_TEST_H_
