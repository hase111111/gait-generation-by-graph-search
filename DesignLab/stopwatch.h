
//! @file      stopwatch.h
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory, Saitama University All right reserved.

#ifndef DESIGNLAB_STOPWATCH_H_
#define DESIGNLAB_STOPWATCH_H_

#include <chrono>
#include <string>


namespace designlab
{

//! @class Stopwatch
//! @brief 時間計測用のクラス．
//! @details
//! Start() を呼び出してから End() を呼び出すまでの経過時間を計測する．
//! コンストラクタにて現在時刻で初期化するので，Start() を呼ばずとも計測開始できる．
//! @code
//! designlab::Stopwatch stopwatch;
//! stopwatch.Start();
//! // 何か処理
//! stopwatch.End();
//! std::cout << stopwatch.GetElapsedSeconds() << std::endl;
//! @endcode
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
    //! @n start()と end()を呼び出してからこの関数を呼ぶ．
    //! @return 計測結果(秒)．
    double GetElapsedSeconds() const;

    //! @brief 計測結果をミリ秒で取得．
    //! @n start()と end()を呼び出してからこの関数を呼ぶ．
    //! @return 計測結果(ミリ秒)．
    double GetElapsedMilliSecond() const;

    //! @brief 計測結果をマイクロ秒で取得．
    //! @n start()と end()を呼び出してからこの関数を呼ぶ．
    //! @note このレベルのオーダーで精度を出せない可能性があるため注意．
    //! @return 計測結果(マイクロ秒)．
    double GetElapsedMicroSecond() const;


    //! @brief 経過時間を秒で表した文字列を取得する．
    //! @return 経過時間(秒)．
    std::string GetElapsedSecondsString() const;

    //! @brief 経過時間をミリ秒で表した文字列を取得する．
    //! @return 経過時間(ミリ秒)．
    std::string GetElapsedMilliSecondString() const;

    //! @brief 経過時間をマイクロ秒で表した文字列を取得する．
    //! @return 経過時間(マイクロ秒)．
    std::string GetElapsedMicroSecondString() const;


    //! @brief 現在の日時をYYYY/MM/DD HH:MM形式の文字列で取得する．
    //! @return YYYYMMDD_HH_MM 形式の文字列．
    std::string GetNowTimeString() const;

private:
    std::chrono::system_clock::time_point start_time_;  //!< 測定開始時間．

    std::chrono::system_clock::time_point end_time_;    //!< 測定終了時間．
};

}  // namespace designlab


#endif  // DESIGNLAB_STOPWATCH_H_
