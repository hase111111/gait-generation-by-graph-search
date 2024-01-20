
/// @author    hasegawa
/// @copyright © 埼玉大学 設計工学研究室 2024. All right reserved.

#include "system_main_robot_control.h"

#include <format>

#include <boost/thread.hpp>

#include "cmdio_util.h"
#include "serial_communication_thread.h"


namespace designlab
{

SystemMainRobotControl::SystemMainRobotControl()
{
}

void SystemMainRobotControl::Main()
{
    SerialCommunicationThread serial_communication_thread;

    // シリアル通信のスレッドを立ち上げる．
    boost::thread serial_communication_thread_(boost::bind(&SerialCommunicationThread::Loop,
                                               &serial_communication_thread));

    while (true)
    {
        // シリアル通信のスレッドが終了していたら，プログラムを終了する．
        if (!serial_communication_thread_.joinable())
        {
            CmdIOUtil::Output("シリアル通信のスレッドが終了しました．", enums::OutputDetail::kError);
            break;
        }

        if (serial_communication_thread.IsEnd())
        {
            CmdIOUtil::Output("シリアル通信のスレッドは終了しています．", enums::OutputDetail::kError);
            break;
        }

        // シリアル通信のスレッドが終了していない場合は，キー入力を受け付ける．
        const auto val = CmdIOUtil::InputInt(0, 1, 1, "終了する場合は0，データの送信をする場合は1を入力してください．");

        // キー入力に応じて，シリアル通信のスレッドにメッセージを送信する．
        if (val == 0)
        {
            serial_communication_thread.EndThread();
            break;
        }
        else if (val == 1)
        {
            const auto str = serial_communication_thread.GetAllReadData();
            CmdIOUtil::Output(std::format("受信したデータ : {}個", str.size()), enums::OutputDetail::kInfo);
        }
    }

    // 通信のスレッドを待つ．
    serial_communication_thread_.join();

    CmdIOUtil::Output("SystemMainRobotControl::Main() is finished．", enums::OutputDetail::kInfo);
};

} // namespace designlab
