#include "pch.h"

#include <thread>

#include "../DesignLab/stopwatch.h"
#include "../DesignLab/stopwatch.cpp"	//どこか1つのファイルで，cppファイルをインクルードする必要がある


// 時間計測クラスのテスト．精度がどの程度出るのかわからないので，ここでテストする意味があるのかは分からない...
// どうすればよいか知ってる人は教えてくれ．


namespace designlab::test::common 
{
	TEST(StopwatchTest, Constructor) 
	{
		//コンストラクタを作成した段階で，タイマーが開始する．
		Stopwatch sw;

		//0.1秒待つ
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

		sw.End();

		EXPECT_DOUBLE_EQ(sw.GetElapsedMilliSecond(), 100.0) << "精度がでない可能性が大です．このテストには失敗しても問題ないかもしれません．";
	}
}