#pragma once
#include <chrono>

//時間計測用のクラス
class MyTimer final
{
public:

	MyTimer() = default;

	//計測開始
	void start()
	{
		m_start_time = std::chrono::system_clock::now();
	}

	//計測終了
	void end()
	{
		m_end_time = std::chrono::system_clock::now();
	}

	//計測結果をミリ秒で取得
	double getMilliSecond() const
	{
		auto dur = m_end_time - m_start_time;        // 要した時間を計算
		auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();

		return (double)msec;
	}

	//計測結果を秒で取得
	double getSecond() const
	{
		auto dur = m_end_time - m_start_time;        // 要した時間を計算
		auto sec = std::chrono::duration_cast<std::chrono::seconds>(dur).count();

		return (double)sec;
	}

	//計測結果をマイクロ秒で取得	
	double getMicroSecond() const
	{
		auto dur = m_end_time - m_start_time;        // 要した時間を計算
		auto usec = std::chrono::duration_cast<std::chrono::microseconds>(dur).count();

		return (double)usec;
	}

private:
	//時間計測用の変数
	std::chrono::system_clock::time_point m_start_time;
	std::chrono::system_clock::time_point m_end_time;
};