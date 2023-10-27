#include "pch.h"

#include "../DesignLab/asyncable_data.h"


// 非同期処理のテストは行わず，単一のスレッドでのテストのみ行う．


namespace designlab::test::common
{
	TEST(AsyncableDataTestVectorIntType, DefaultConstructorTest)
	{
		AsyncableData<std::vector<int>> data;	//初期化

		EXPECT_EQ(data.GetUpdateCount(), 0) << "コンストラクタで初期化した場合，更新回数は0です．";
		EXPECT_EQ(data.GetSize(), 0) << "コンストラクタで初期化した場合，サイズは0です．";
	}

	TEST(AsyncableDataTestVectorIntType, ConstructorTest)
	{
		const std::vector<int> sample = { 1, 2, 3 };

		AsyncableData<std::vector<int>> data(sample);	//初期化

		EXPECT_EQ(data.GetData(), sample) << "コンストラクタで値を初期化できます．";
		EXPECT_EQ(data.GetUpdateCount(), 0) << "コンストラクタで初期化した場合，更新回数は0です．";
		EXPECT_EQ(data.GetSize(), 3) << "コンストラクタで初期化した場合，サイズは3です．";
	}

	TEST(AsyncableDataTestVectorIntType, TestCanSetAndGetData)
	{
		AsyncableData<std::vector<int>> data;	//初期化

		std::vector<int> sample = { 1, 2, 3 };

		data.SetData(sample);
		EXPECT_EQ(data.GetData(), sample);

		sample = { 4, 5, 6 };
		data.SetData(sample);
		EXPECT_EQ(data.GetData(), sample);
	}

	TEST(AsyncableDataTestVectorIntType, TestCanPushBackAndGetData)
	{
		AsyncableData<std::vector<int>> data;	//初期化

		std::vector<int> sample = { 1, 2, 3 };

		data.SetData(sample);
		EXPECT_EQ(data.GetData(), sample);

		data.PushBack(4);
		sample.push_back(4);
		EXPECT_EQ(data.GetData(), sample);

		data.PushBack(5);
		sample.push_back(5);
		EXPECT_EQ(data.GetData(), sample);
	}

	TEST(AsyncableDataTestVectorIntType, TestCanClearAndGetData)
	{
		AsyncableData<std::vector<int>> data;	//初期化

		std::vector<int> sample = { 1, 2, 3 };

		data.SetData(sample);
		EXPECT_EQ(data.GetData(), sample);

		data.Clean();
		sample.clear();
		EXPECT_EQ(data.GetData(), sample);
	}

	TEST(AsyncableDataTestVectorIntType, GetUpdateCountTestCaseOfSetData)
	{
		AsyncableData<std::vector<int>> data;	//初期化
		EXPECT_EQ(data.GetUpdateCount(), 0) << "初期化時には更新回数は0です．";

		std::vector<int> sample = { 1, 2, 3 };

		data.SetData(sample);
		EXPECT_EQ(data.GetUpdateCount(), 1) << "値を更新した場合1ずつ増えます．";

		sample = { 4, 5, 6 };
		data.SetData(sample);
		EXPECT_EQ(data.GetUpdateCount(), 2) << "値を更新した場合1ずつ増えます．";
	}

	TEST(AsyncableDataTestVectorIntType, GetUpdateCountTestCaseOfGetData)
	{
		AsyncableData<std::vector<int>> data;	//初期化
		EXPECT_EQ(data.GetUpdateCount(), 0) << "初期化時には更新回数は0です．";

		std::vector<int> sample = { 1, 2, 3 };

		data.SetData(sample);
		EXPECT_EQ(data.GetUpdateCount(), 1) << "値を更新した場合1ずつ増えます．";

		data.GetData();
		EXPECT_EQ(data.GetUpdateCount(), 1) << "値を取得しただけでは更新回数は増えません．";

		data.GetData();
		EXPECT_EQ(data.GetUpdateCount(), 1) << "値を取得しただけでは更新回数は増えません．";
	}

	TEST(AsyncableDataTestVectorIntType, GetUpdateCountTestCaseOfPushData)
	{
		AsyncableData<std::vector<int>> data;	//初期化
		EXPECT_EQ(data.GetUpdateCount(), 0) << "初期化時には更新回数は0です．";

		std::vector<int> sample = { 1, 2, 3 };

		data.SetData(sample);
		EXPECT_EQ(data.GetUpdateCount(), 1) << "値を更新した場合1ずつ増えます．";

		data.PushBack(4);
		EXPECT_EQ(data.GetUpdateCount(), 2) << "値を更新した場合1ずつ増えます．";
	}

	TEST(AsyncableDataTestVectorIntType, GetUpdateCountTestCaseOfClear)
	{
		AsyncableData<std::vector<int>> data;	//初期化
		EXPECT_EQ(data.GetUpdateCount(), 0) << "初期化時には更新回数は0です．";

		std::vector<int> sample = { 1, 2, 3 };

		data.SetData(sample);
		EXPECT_EQ(data.GetUpdateCount(), 1) << "値を更新した場合1ずつ増えます．";

		data.Clean();
		EXPECT_EQ(data.GetUpdateCount(), 2) << "値を更新した場合1ずつ増えます．";
	}

	TEST(AsyncableDataTestVectorIntType, GetUpdateCountTestCaseOfGetSize)
	{
		AsyncableData<std::vector<int>> data;	//初期化
		EXPECT_EQ(data.GetUpdateCount(), 0) << "初期化時には更新回数は0です．";

		std::vector<int> sample = { 1, 2, 3 };

		data.SetData(sample);
		EXPECT_EQ(data.GetUpdateCount(), 1) << "値を更新した場合1ずつ増えます．";

		data.GetSize();
		EXPECT_EQ(data.GetUpdateCount(), 1) << "値を更新した場合1ずつ増えます．";
	}
}