
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory,
//! Saitama University All right reserved.

#include "dxlib_gui_robot_control.h"

#include <string>

#include "cassert_define.h"
#include "dxlib_util.h"
#include "font_loader.h"
#include "mouse.h"


namespace designlab
{

DxlibGuiRobotControl::DxlibGuiRobotControl(
    const int window_x, const int window_y,
    const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
    const std::shared_ptr<const IHexapodJointCalculator>& calculator_ptr,
    const std::shared_ptr<const IHexapodPostureValidator>& checker_ptr) :
    AbstractDxlibGui(kWidth, kHeight),
    window_x_(window_x),
    window_y_(window_y),
    converter_ptr_(converter_ptr),
    calculator_ptr_(calculator_ptr),
    checker_ptr_(checker_ptr)
{
    assert(converter_ptr_ != nullptr);
    assert(calculator_ptr_ != nullptr);
    assert(checker_ptr_ != nullptr);

    const int button_distance = 10;  //!< ボタン同士の間隔．
    const int button_size = 60;  //!< ボタンのサイズ．

    const int button_range = button_size + button_distance;
    const int left_pos_x = gui_left_pos_x_ + button_range / 2 + 15;
    const int top_pos_y = gui_top_pos_y_ + button_range / 2 + 40;

    button_.push_back(std::make_unique<SimpleButton>(
        "Toggle\nMode", left_pos_x, top_pos_y, button_size, button_size));
    button_.back()->SetActivateFunction(
        [this]() {auto_update_flag_ = !auto_update_flag_; });

    button_.push_back(std::make_unique<SimpleButton>(
        "Update", left_pos_x, top_pos_y + button_range, button_size, button_size));
    button_.back()->SetActivateFunction(
        [this]() { serial_communication_.SetWriteData(GetSerialData());  });

    const int close_button_size = 28;
    const int close_button_x = gui_left_pos_x_ + kWidth - close_button_size / 2 - 2;
    const int close_button_y = gui_top_pos_y_ + close_button_size / 2 + 2;

    button_.push_back(std::make_unique<SimpleButton>(
        "×", close_button_x, close_button_y, close_button_size, close_button_size));
    button_.back()->SetActivateFunction([this]() { SetVisible(false); });

    // シリアル通信を開始する．
    serial_communication_thread_ptr_ =
        std::make_unique<boost::thread>(
        &SerialCommunicationThread::Loop, &serial_communication_);
}

DxlibGuiRobotControl::~DxlibGuiRobotControl()
{
    if (serial_communication_thread_ptr_)
    {
        serial_communication_.EndThread();

        serial_communication_thread_ptr_->join();
    }
}

void DxlibGuiRobotControl::Update()
{
    // 各ボタンの処理．
    for (auto& button : button_)
    {
        button->Update();
    }

    ++counter_;

    if (auto_update_flag_ && counter_ % 120 == 0)
    {
        serial_communication_.SetWriteData(GetSerialData());
    }

    if (!IsInWindow())
    {
        SetVisible(false);
    }
}

void DxlibGuiRobotControl::Draw() const
{
    DrawBackground("RobotControl");

    // 全てのボタンの描画．
    for (auto& button : button_)
    {
        button->Draw();
    }

    DrawString();
}

void DxlibGuiRobotControl::DrawString() const
{
    const unsigned int str_color = GetColor(54, 54, 54);

    const int text_interval_y = 20;
    const int text_top_y = gui_top_pos_y_ + 190;

    int text_line = 0;

    // スレッドの状態を出力する．
    if (serial_communication_.IsEnd())
    {
        DrawFormatStringToHandle(gui_left_pos_x_ + 10, text_top_y + text_interval_y * (text_line++), str_color, font_handle_, "スレッドは終了しています．");
    }
    else
    {
        DrawFormatStringToHandle(gui_left_pos_x_ + 10, text_top_y + text_interval_y * (text_line++), str_color, font_handle_, "スレッドは実行中です．");
    }

    // 通信されたデータの数を出力する．
    const std::vector<std::string> read_data = serial_communication_.GetAllReadData();
    DrawFormatStringToHandle(gui_left_pos_x_ + 10, text_top_y + text_interval_y * (text_line++), str_color, font_handle_, "受信したデータの数: %d", read_data.size());

    // 通信されたデータから，最新のデータを出力する．
    int display_num = 20;

    for (auto i = read_data.rbegin(); i != read_data.rend(); ++i)
    {
        std::string str = "[" + std::to_string(static_cast<int>(read_data.size()) - (10 - display_num)) + "] ";
        str += *i;

        DrawFormatStringToHandle(gui_left_pos_x_ + 10, text_top_y + text_interval_y * (text_line++), str_color, font_handle_, "%s", str.c_str());

        if (--display_num <= 0)
        {
            break;
        }
    }
}

bool DxlibGuiRobotControl::IsInWindow() const
{
    return gui_left_pos_x_ < window_x_ && gui_top_pos_y_ < window_y_ &&
        0 < gui_left_pos_x_ + kWidth && 0 < gui_top_pos_y_ + kHeight;
}

std::string DxlibGuiRobotControl::GetSerialData() const
{
    // ノードの値から，脚先座標をシリアル通信で送信する．
    // float を int に変換したのち，
    // 64 byte に収まるように，int_8型 * 2 に変換する．
    std::uint8_t send_data[64] = {};

    for (size_t i = 0; i < HexapodConst::kLegNum; i++)
    {
        const std::uint32_t leg_x = abs(static_cast<int>(node_.leg_pos[i].x));

        send_data[i * 8 + 0] = static_cast<std::uint8_t>(leg_x & 0x000000ff);
        send_data[i * 8 + 1] = static_cast<std::uint8_t>((leg_x & 0x0000ff00) >> 8);

        const std::uint32_t leg_y = abs(static_cast<int>(node_.leg_pos[i].y));

        send_data[i * 8 + 2] = static_cast<std::uint8_t>(leg_y & 0x000000ff);
        send_data[i * 8 + 3] = static_cast<std::uint8_t>((leg_y & 0x0000ff00) >> 8);

        // zに補正をかける．
        float leg_z_pos = node_.leg_pos[i].z;
        if (-30 >= leg_z_pos && leg_z_pos >= -45) { leg_z_pos = -45; }
        const std::uint32_t leg_z = abs(static_cast<int>(leg_z_pos));

        send_data[i * 8 + 4] = static_cast<std::uint8_t>(leg_z & 0x000000ff);
        send_data[i * 8 + 5] = static_cast<std::uint8_t>((leg_z & 0x0000ff00) >> 8);
    }

    // 正負のデータを送信する．
    for (size_t i = 0; i < HexapodConst::kLegNum; i++)
    {
        const int already_sent_data_num = HexapodConst::kLegNum * 8;

        send_data[already_sent_data_num + i] = 0;

        if (node_.leg_pos[i].x < 0)
        {
            send_data[already_sent_data_num + i] |= 0x01;
        }

        if (node_.leg_pos[i].y < 0)
        {
            send_data[already_sent_data_num + i] |= 0x02;
        }

        if (node_.leg_pos[i].z < 0)
        {
            send_data[already_sent_data_num + i] |= 0x04;
        }
    }

    // 送信データを文字列に変換する．
    std::string str;

    for (size_t i = 0; i < 64; i++)
    {
        str += static_cast<char>(send_data[i]);
    }

    return str;
}

}  // namespace designlab
