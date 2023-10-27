#include "pch.h"

#include "../DesignLab/asyncable_data.h"


// 非同期処理のテストは行わず，単一のスレッドでのテストのみ行う．


namespace designlab::test::common 
{
	TEST(AsyncableDataTestIntType, DefaultConstructorTest)
	{
		AsyncableData<int> data;	//初期化

		EXPECT_EQ(data.GetUpdateCount(), 0) << "コンストラクタで初期化した場合，更新回数は0です．";
	}

	TEST(AsyncableDataTestIntType, ConstructorTest)
	{
		AsyncableData<int> data(-4);	//初期化

		EXPECT_EQ(data.GetData(), -4) << "コンストラクタで値を初期化できます．";
		EXPECT_EQ(data.GetUpdateCount(), 0) << "コンストラクタで初期化した場合，更新回数は0です．";
	}

	TEST(AsyncableDataTestIntType, GetUpdateCountTest)
	{
		AsyncableData<int> data(0);	//初期化

		EXPECT_EQ(data.GetUpdateCount(), 0) << "初期化時点では，更新回数は0です．";

		data.SetData(10);
		EXPECT_EQ(data.GetUpdateCount(), 1) << "データを更新すると，更新回数が1ずつ増えます．";

		data.SetData(20);
		EXPECT_EQ(data.GetUpdateCount(), 2) << "データを更新すると，更新回数が1ずつ増えます．";
	}

	TEST(AsyncableDataTestIntType, TestCanGetAndSetData)
	{
		//初期化
		AsyncableData<int> data(0);

		data.SetData(10);
		EXPECT_EQ(data.GetData(), 10);

		data.SetData(20);
		EXPECT_EQ(data.GetData(), 20);
	}
}