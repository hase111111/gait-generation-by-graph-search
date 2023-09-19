#include "stopwatch.h"


Stopwatch::Stopwatch() : 
	start_time_(std::chrono::system_clock::now()),
	end_time_(std::chrono::system_clock::now())
{
}

void Stopwatch::Start()
{
	start_time_ = std::chrono::system_clock::now();
}


void Stopwatch::End()
{
	end_time_ = std::chrono::system_clock::now();
}


double Stopwatch::GetElapsedSeconds() const
{
	auto dur = end_time_ - start_time_;        // 要した時間を計算

	auto sec = std::chrono::duration_cast<std::chrono::seconds>(dur).count();	// 秒に変換

	return static_cast<double>(sec);
}


double Stopwatch::GetElapsedMilliSecond() const
{
	auto dur = end_time_ - start_time_;        // 要した時間を計算

	auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();		// ミリ秒に変換

	return static_cast<double>(msec);
}


double Stopwatch::GetElapsedMicroSecond() const
{
	auto dur = end_time_ - start_time_;        // 要した時間を計算

	auto usec = std::chrono::duration_cast<std::chrono::microseconds>(dur).count();		// マイクロ秒に変換	

	return static_cast<double>(usec);
}


std::string Stopwatch::GetNowTimeString() const
{
	// 現在の日時をYYYY/MM/DD HH:MM形式の文字列で取得する
	auto now = std::chrono::system_clock::now();
	auto now_time = std::chrono::system_clock::to_time_t(now);
	std::tm now_tm;
	localtime_s(&now_tm, &now_time);
	char now_time_str[64];
	strftime(now_time_str, sizeof(now_time_str), "%Y%m%d-%H%M", &now_tm);
	return std::string(now_time_str);
}
