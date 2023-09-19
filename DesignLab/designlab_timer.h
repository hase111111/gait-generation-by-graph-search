#pragma once

#include <chrono>
#include <string>


//! @class DesignlabTimer
//! @date 2023/08/06
//! @author 長谷川
//! @brief 時間計測用のクラス．
class DesignlabTimer final
{
public:

	DesignlabTimer() = default;

	//! @brief 計測開始にこの関数を呼ぶ．リセット処理などを呼ぶ必要なしで，再度計測できる．
	void start();

	//! @brief 計測終了にこの関数を呼ぶ．
	void end();

	//! @brief 計測結果を秒で取得．start()とend()を呼び出してからこの関数を呼ぶ．
	//! @return double 計測結果(秒)
	double getSecond() const;

	//! @brief 計測結果をミリ秒で取得．start()とend()を呼び出してからこの関数を呼ぶ．
	//! @return double 計測結果(ミリ秒)
	double getMilliSecond() const;

	//! @biref 計測結果をマイクロ秒で取得．start()とend()を呼び出してからこの関数を呼ぶ．
	//! @return double  計測結果(マイクロ秒)
	double getMicroSecond() const;

	//! @brief 現在の日時をYYYY/MM/DD HH:MM形式の文字列で取得する
	//! @return std::string 現在の日時
	std::string getNowTime() const;

private:


	std::chrono::system_clock::time_point m_start_time;		//測定開始時間

	std::chrono::system_clock::time_point m_end_time;		//測定終了時間	
};


//! @file my_timer.h
//! @date 2023/08/06
//! @author 長谷川
//! @brief 自作の時間計測クラス．
//! @n 行数 : @lineinfo
