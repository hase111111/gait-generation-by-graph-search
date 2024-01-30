
/// @author    Hasegawa
/// @copyright © 埼玉大学 設計工学研究室 2024. All right reserved.

#include "serial_communication_thread.h"

#include "cmdio_util.h"
#include "string_util.h"


namespace
{
// 無名名前空間．このファイル内でのみ有効．

static constexpr int kBaudRate = 9600;  // ボーレート．Arduino側と合わせる．

// COMポートの名前．
static constexpr std::string kComName[] = { "", "COM1", "COM2", "COM3", "COM4",
                                            "COM5", "COM6", "\\\\.\\COM7" , "COM8", "COM9",
                                            "\\\\.\\COM10", "\\\\.\\COM11", "\\\\.\\COM12", "\\\\.\\COM13" };
}

namespace designlab
{

void SerialCommunicationThread::Loop()
{
    if (!Initialize())
    {
        end_flag_ = true;

        // 通信の初期化に失敗した場合は，即座に終了する．
        return;
    }

    while (true)
    {
        Sleep(static_cast<int>(1000 * kThreadPeriod));  // 指定した時間だけスリープする．(ミリ秒単位)

        // 読み込みを行う．
        if (!Read())
        {
            EndThread();

            // 読み込みに失敗した場合は，即座に終了する．
            return;
        }

        // 書き込みを行う．
        if (!Write())
        {
            EndThread();

            // 書き込みに失敗した場合は，即座に終了する．
            return;
        }

        // スレッドの終了フラグが立っている場合は，即座に終了する．
        if (IsEnd())
        {
            return;
        }
    }
}

void SerialCommunicationThread::SetWriteData(const std::string& str)
{
    // 通信が開始されていない場合は，即座に終了する．
    if (serial_handle_ == INVALID_HANDLE_VALUE)
    {
        return;
    }

    // 書き込みデータを送信する．
    boost::mutex::scoped_lock lock(mutex_);

    write_data_ = str;
}

std::string SerialCommunicationThread::GetTopReadData() const
{
    // スレッドの排他制御を行う．
    boost::mutex::scoped_lock lock(mutex_);

    // 読み込みデータが空の場合は，空文字を返す．
    if (read_data_.empty())
    {
        return "";
    }

    return read_data_.back();
}

std::vector<std::string> SerialCommunicationThread::GetAllReadData() const
{
    // スレッドの排他制御を行う．
    boost::mutex::scoped_lock lock(mutex_);

    return read_data_;
}

std::vector<std::string> SerialCommunicationThread::GetReadData(const int num) const
{
    // スレッドの排他制御を行う．
    boost::mutex::scoped_lock lock(mutex_);

    std::vector<std::string> ret;

    // 読み込みデータが空の場合は，空の配列を返す．
    if (read_data_.empty())
    {
        return ret;
    }

    // 読み込みデータが指定した数よりも少ない場合は，全てのデータを返す．
    if (read_data_.size() <= num)
    {
        return read_data_;
    }

    // 読み込みデータが指定した数よりも多い場合は，最新のデータを返す．
    for (int i = 0; i < num; ++i)
    {
        ret.push_back(read_data_[read_data_.size() - 1 - i]);
    }

    return ret;
}

bool SerialCommunicationThread::Initialize()
{
    // 通信を開始する．Windowsでは CreateFile で COMポートを開く．
    serial_handle_ = CreateFile(kComName[7].c_str(), GENERIC_READ | GENERIC_WRITE,
                                0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (serial_handle_ == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    // 通信の設定を行う．
    SetupComm(serial_handle_, kBufferSize, kBufferSize);  // バッファーを作る．
    PurgeComm(serial_handle_, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);  // バッファーのクリア．

    // シリアルポートの設定を行う．
    DCB dcb;
    GetCommState(serial_handle_, &dcb);  // 現在の設定を取得．

    dcb.ByteSize = 8;
    dcb.XonChar = 0;
    dcb.XoffChar = 0;
    dcb.EofChar = 4;
    dcb.EvtChar = 10;
    dcb.BaudRate = kBaudRate;

    SetCommState(serial_handle_, &dcb);  // 設定を反映．

    // タイムアウトの設定を行う．
    COMMTIMEOUTS timeouts;
    GetCommTimeouts(serial_handle_, &timeouts);  // 現在の設定を取得．

    timeouts.ReadIntervalTimeout = 100;
    timeouts.ReadTotalTimeoutMultiplier = 100;
    timeouts.ReadTotalTimeoutConstant = 100;
    timeouts.WriteTotalTimeoutMultiplier = 0;
    timeouts.WriteTotalTimeoutConstant = 0;

    SetCommTimeouts(serial_handle_, &timeouts);  // 設定を反映．

    return true;
}

bool SerialCommunicationThread::Read()
{
    // 通信が開始されていない場合は，即座に終了する．
    if (serial_handle_ == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    DWORD read_size = 0;
    char read_buffer[kBufferSize] = { 0 };

    if (!ReadFile(serial_handle_, read_buffer, kBufferSize, &read_size, NULL))
    {
        // 読み込みに失敗した場合は終了する．
        return false;
    }

    if (read_size == 0)
    {
        // 読み込みデータがない場合は終了する．
        // 読み込み自体は成功しているため，trueを返す．
        return true;
    }

    // 読み込んだデータを格納する．
    std::string read_data(read_buffer, read_size);

    // 読み込んだデータを改行コードで分割する．
    std::vector<std::string> split_data = string_util::Split(read_data, "\n");

    // 分割したデータを読み込みデータに追加する．
    {
        // スレッドの排他制御を行う．
        boost::mutex::scoped_lock lock(mutex_);

        for (const auto& data : split_data)
        {
            // 空文字の場合は追加しない．
            if (data.empty())
            {
                continue;
            }

            // 読み込みデータに追加する．
            read_data_.push_back(data);
        }
    }

    return true;
}

bool SerialCommunicationThread::Write()
{
    // 通信が開始されていない場合は，即座に終了する．
    if (serial_handle_ == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    // 書き込みデータがない場合は，即座に終了する．
    if (write_data_ == "")
    {
        return true;
    }

    DWORD write_size = 0;

    if (!WriteFile(serial_handle_, write_data_.c_str(), static_cast<DWORD>(write_data_.size()), &write_size, NULL))
    {
        // 書き込みに失敗した場合は終了する．
        return false;
    }

    if (write_size == 0)
    {
        // 書き込みデータがない場合は終了する．
        // 書き込み自体は成功しているため，trueを返す．
        return true;
    }

    // 書き込みデータをクリアする．
    write_data_ = "";

    return true;
}

}  // namespace designlab
