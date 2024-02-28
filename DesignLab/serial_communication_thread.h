
//! @file      serial_communication_thread.h
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2024. All right reserved.

#ifndef DESIGNLAB_SERIAL_COMMUNICATION_THREAD_H_
#define DESIGNLAB_SERIAL_COMMUNICATION_THREAD_H_

#include <string>
#include <vector>

#include <boost/thread.hpp>

#include <Windows.h>  // シリアル通信を行うために必要．


namespace designlab
{

class SerialCommunicationThread final
{
public:
    SerialCommunicationThread() = default;
    ~SerialCommunicationThread() = default;

    void Loop();

    //! @brief 指定した文字列をシリアル通信で送信する．
    //! この時，排他制御を行う．
    //! @param[in] str 送信する文字列．
    void SetWriteData(const std::string& str);

    //! @brief シリアル通信で受信した最新の文字列を取得する．
    //! この時，排他制御を行う．
    //! @return 受信した文字列．
    std::string GetTopReadData() const;

    //! @brief シリアル通信で受信した文字列を全て取得する．
    //! この時，排他制御を行う．
    //! @return 受信した文字列群．
    std::vector<std::string> GetAllReadData() const;

    //! @brief シリアル通信で受信した文字列の中から指定した数だけ取得する．
    //! この時，排他制御を行う．
    //! @param[in] num 取得する文字列の数．
    //! @return 受信した文字列群．
    std::vector<std::string> GetReadData(const int num) const;

    //! @brief シリアル通信のスレッドを終了する．
    void EndThread()
    {
        boost::mutex::scoped_lock lock(mutex_);
        end_flag_ = true;
    }

    bool IsEnd() const
    {
        boost::mutex::scoped_lock lock(mutex_);
        return end_flag_;
    }

private:
    static constexpr float kThreadPeriod = 0.01f;  //!< シリアル通信スレッドの周期 [s]
    static constexpr int kBufferSize = 1024;  //!< シリアル通信のバッファサイズ [byte]
    static constexpr int kComPortNumber = 3;  //!< シリアル通信のポート番号．

    //! @brief シリアル通信を開始する．
    bool Initialize();

    //! @brief シリアル通信のデータを受信する．
    //! @retval true  受信成功．
    //! @retval false 受信失敗．
    bool Read();

    //! @brief シリアル通信のデータを送信する．
    //! @retval true  送信成功．
    //! @retval false 送信失敗．
    bool Write();

    HANDLE serial_handle_{ INVALID_HANDLE_VALUE };  //!< シリアル通信のハンドル．

    std::vector<std::string> read_data_;  //!< シリアル通信で受信した文字列群．
    std::string write_data_;  //!< シリアル通信で送信する文字列．

    bool end_flag_{ false };  //!< スレッド終了フラグ．

    mutable boost::mutex mutex_;  //!< 排他制御用のミューテックス．
};

}  // namespace designlab


#endif  // DESIGNLAB_SERIAL_COMMUNICATION_THREAD_H_
