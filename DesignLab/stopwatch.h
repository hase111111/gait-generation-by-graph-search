//! @file stopwatch.h
//! @brief 自作の時間計測クラス．

#ifndef DESIGNLAB_STOPWATCH_H_
#define DESIGNLAB_STOPWATCH_H_

#include <chrono>
#include <string>


//! @class Stopwatch
//! @brief 時間計測用のクラス．
//! @details
//! Start関数を呼び出してからEnd関数を呼び出すまでの経過時間を計測する．
//! @n 実はコンストラクタにて現在時刻で初期化するので，Start関数を呼ばずとも計測開始できる．
class Stopwatch final
{
public:

	//! @brief コンストラクタ
	//! @n 現在時刻で初期化する．
	Stopwatch();

	//! @brief 計測開始にこの関数を呼ぶ．
	//! @n リセット処理などを呼ぶ必要なしで，再度計測できる．
	void Start();

	//! @brief 計測終了時にこの関数を呼ぶ．
	void End();

	//! @brief 経過時間を秒で取得．
	//! @n start()とend()を呼び出してからこの関数を呼ぶ．
	//! @return double 計測結果(秒)．
	double GetElapsedSeconds() const;

	//! @brief 計測結果をミリ秒で取得．
	//! @n start()とend()を呼び出してからこの関数を呼ぶ．
	//! @return double 計測結果(ミリ秒)．
	double GetElapsedMilliSecond() const;

	//! @brief 計測結果をマイクロ秒で取得．
	//! @n start()とend()を呼び出してからこの関数を呼ぶ．
	//! @n (このレベルのオーダーで精度出せるのか...?)．
	//! @return double  計測結果(マイクロ秒)．
	double GetElapsedMicroSecond() const;


	//! @brief 経過時間を秒で表した文字列を取得する．
	//! @return std::string 経過時間．
	std::string GetElapsedSecondsString() const;

	//! @brief 経過時間をミリ秒で表した文字列を取得する．
	//! @return std::string 経過時間．
	std::string GetElapsedMilliSecondString() const;

	//! @brief 経過時間をマイクロ秒で表した文字列を取得する．
	//! @return std::string 経過時間．
	std::string GetElapsedMicroSecondString() const;


	//! @brief 現在の日時をYYYY/MM/DD HH:MM形式の文字列で取得する．
	//! @return std::string 現在の日時．
	std::string GetNowTimeString() const;

private:

	std::chrono::system_clock::time_point start_time_;		//!< 測定開始時間

	std::chrono::system_clock::time_point end_time_;		//!< 測定終了時間	
};


#endif	// DESIGNLAB_STOPWATCH_H_