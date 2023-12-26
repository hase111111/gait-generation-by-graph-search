
#include "stopwatch.h"

#include <cstdint>

#include "math_util.h"


namespace sc = ::std::chrono;

namespace designlab
{

Stopwatch::Stopwatch() :
    start_time_(sc::system_clock::now()),
    end_time_(sc::system_clock::now())
{
}

void Stopwatch::Start()
{
    start_time_ = sc::system_clock::now();
}


void Stopwatch::End()
{
    end_time_ = sc::system_clock::now();
}


double Stopwatch::GetElapsedSeconds() const
{
    // 要した時間を計算．
    const sc::system_clock::duration dur = end_time_ - start_time_;

    // 秒に変換．
    const std::int64_t sec = sc::duration_cast<sc::seconds>(dur).count();

    // ミリ秒に変換．
    const std::int64_t milli_sec = sc::duration_cast<sc::milliseconds>(dur).count();

    return static_cast<double>(sec) + static_cast<double>(milli_sec) / 1000.0;
}


double Stopwatch::GetElapsedMilliSecond() const
{
    // 要した時間を計算．
    const sc::system_clock::duration dur = end_time_ - start_time_;

    // ミリ秒に変換．
    const std::int64_t milli_sec = sc::duration_cast<sc::milliseconds>(dur).count();

    // マイクロ秒に変換．
    const std::int64_t usec = sc::duration_cast<sc::microseconds>(dur).count();

    return static_cast<double>(milli_sec) + static_cast<double>(usec) / 1000.0;
}


double Stopwatch::GetElapsedMicroSecond() const
{
    // 要した時間を計算．
    const sc::system_clock::duration dur = end_time_ - start_time_;

    // マイクロ秒に変換．
    const std::int64_t usec = sc::duration_cast<sc::microseconds>(dur).count();

    return static_cast<double>(usec);
}


std::string Stopwatch::GetElapsedSecondsString() const
{
    return math_util::FloatingPointNumToString(GetElapsedSeconds()) + " [s]";
}

std::string Stopwatch::GetElapsedMilliSecondString() const
{
    return math_util::FloatingPointNumToString(GetElapsedMilliSecond()) + " [ms]";
}

std::string Stopwatch::GetElapsedMicroSecondString() const
{
    return math_util::FloatingPointNumToString(GetElapsedMicroSecond()) + "[μs]";
}


std::string Stopwatch::GetNowTimeString() const
{
    // 現在の日時をYYYY/MM/DD HH:MM形式の文字列で取得する．
    sc::system_clock::time_point now = std::chrono::system_clock::now();
    time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm now_tm;
    localtime_s(&now_tm, &now_time);
    char now_time_str[64];
    strftime(now_time_str, sizeof(now_time_str), "%Y%m%d_%H%M_%S", &now_tm);
    return std::string(now_time_str);
}

}  // namespace designlab
