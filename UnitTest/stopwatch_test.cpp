#include "pch.h"

#include <thread>

#include "../DesignLab/stopwatch.h"
#include "../DesignLab/stopwatch.cpp"	//どこか1つのファイルで，cppファイルをインクルードする必要がある


// 時間計測クラスのテスト．精度がどの程度出るのかわからないので，ここでテストする意味があるのかは分からない...
// どうすればよいか知ってる人は教えてくれ．
// 正確に時間が計測できるかではなく，関数の機能が正しいかのみテストする．


namespace designlab::test::common 
{
	TEST(StopwatchTest, ConstructorTestCanTimerStart) 
	{
		Stopwatch sw;

		std::this_thread::sleep_for(std::chrono::milliseconds(10));	//0.01秒待つ

		sw.End();

		EXPECT_NE(sw.GetElapsedMilliSecond(), 0.0) << "コンストラクタが呼ばれた時点でタイマーは開始します．";
	}

	TEST(StopwatchTest, EndTestCanTimerStop)
	{
		Stopwatch sw;

		sw.Start();

		std::this_thread::sleep_for(std::chrono::milliseconds(10));	//0.01秒待つ

		sw.End();

		const double first_elapsed_time = sw.GetElapsedMilliSecond();

		std::this_thread::sleep_for(std::chrono::milliseconds(10));	//0.01秒待つ

		const double second_elapsed_time = sw.GetElapsedMilliSecond();

		EXPECT_EQ(first_elapsed_time, second_elapsed_time) << "End()が呼ばれた時点でタイマーは停止します．";
	}

	TEST(StopwatchTest, StartTestCanTimerRestart)
	{
		Stopwatch sw;
		Stopwatch long_sw;

		sw.Start();
		long_sw.Start();

		std::this_thread::sleep_for(std::chrono::milliseconds(10));	//0.01秒待つ

		sw.End();

		const double first_elapsed_time = sw.GetElapsedMilliSecond();

		sw.Start();

		std::this_thread::sleep_for(std::chrono::milliseconds(10));	//0.01秒待つ

		sw.End();
		long_sw.End();

		const double second_elapsed_time = sw.GetElapsedMilliSecond();

		const double long_elapsed_time = long_sw.GetElapsedMilliSecond();

		EXPECT_LT(first_elapsed_time, long_elapsed_time) << "Start()が呼ばれた時点でタイマーはリスタートします．";	// less than <
		EXPECT_LT(second_elapsed_time, long_elapsed_time) << "Start()が呼ばれた時点でタイマーはリスタートします．";
		EXPECT_LT(0.f, first_elapsed_time) << "Start()が呼ばれた時点でタイマーはリスタートします．";
		EXPECT_LT(0.f, second_elapsed_time) << "Start()が呼ばれた時点でタイマーはリスタートします．";

		//計測精度の問題で以下のテストは失敗するか成功するかが不安定
		//EXPECT_EQ(first_elapsed_time, second_elapsed_time) << "Start()が呼ばれた時点でタイマーはリスタートします．";
	}

	TEST(StopwatchTest, TestTimeUnitCheck)
	{
		// 単位があっているかを確認するテスト

		Stopwatch sw;

		sw.Start();

		std::this_thread::sleep_for(std::chrono::milliseconds(10));	//0.01秒待つ

		sw.End();

		const double elapsed_seconds = sw.GetElapsedSeconds();
		const double elapsed_milliseconds = sw.GetElapsedMilliSecond();
		const double elapsed_microseconds = sw.GetElapsedMicroSecond();

		EXPECT_EQ(static_cast<int>(elapsed_seconds), static_cast<int>(elapsed_milliseconds / 1000.0)) << "1 [sec] = 1000 [msec]";
		EXPECT_EQ(static_cast<int>(elapsed_milliseconds), static_cast<int>(elapsed_microseconds / 1000.0)) << "1 [msec] = 1000 [usec]";
	}
}