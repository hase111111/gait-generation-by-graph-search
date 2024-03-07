
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory,
//! Saitama University All right reserved.

#include "dxlib_gui_node_displayer.h"

#include <format>

#include <magic_enum.hpp>

#include <Dxlib.h>

#include "math_util.h"
#include "math_rot_converter.h"
#include "string_util.h"
#include "font_loader.h"
#include "leg_state.h"
#include "phantomx_mk2_const.h"


namespace designlab
{

DxlibGuiNodeDisplayer::DxlibGuiNodeDisplayer(
    const int pos_x,
    const int pos_y,
    const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
    const std::shared_ptr<const IHexapodJointCalculator>& calculator_ptr,
    const std::shared_ptr<const IHexapodPostureValidator>& checker_ptr) :
    AbstractDxlibGui{ 470, 680 },
    converter_ptr_(converter_ptr),
    calculator_ptr_(calculator_ptr),
    checker_ptr_(checker_ptr),
    display_type_(DisplayMode::kDefualt),
    window_x_(pos_x),
    window_y_(pos_y)
{
    // ボタンを作成する．
    const int button_size_x = 90;
    const int button_size_y = 30;

    button_.push_back(
        std::make_unique<SimpleButton>(
        "基本情報",
        10 + button_size_x / 2,
        gui_top_pos_y_ + height_ - button_size_y / 2 - 10,
        button_size_x,
        button_size_y));

    button_.back()->SetActivateFunction(
        [this]() {display_type_ = DisplayMode::kDefualt; });

    button_.push_back(
        std::make_unique<SimpleButton>(
        "joint",
        (10 + button_size_x / 2) + (10 + button_size_x),
        gui_top_pos_y_ + height_ - button_size_y / 2 - 10,
        button_size_x,
        button_size_y));

    button_.back()->SetActivateFunction(
        [this]() {display_type_ = DisplayMode::kJointState; });

    button_.push_back(
        std::make_unique<SimpleButton>(
        "脚先座標",
        (10 + button_size_x / 2) + (10 + button_size_x) * 2,
        gui_top_pos_y_ + height_ - button_size_y / 2 - 10,
        button_size_x,
        button_size_y));

    button_.back()->SetActivateFunction(
        [this]() {display_type_ = DisplayMode::kGlobalPos; });

    const int close_button_size = 28;
    const int close_button_x = gui_left_pos_x_ + width_ - close_button_size / 2 - 2;
    const int close_button_y = gui_top_pos_y_ + close_button_size / 2 + 2;

    button_.push_back(std::make_unique<SimpleButton>(
        "×", close_button_x, close_button_y,
        close_button_size, close_button_size));

    button_.back()->SetActivateFunction([this]() { SetVisible(false); });
}

void DxlibGuiNodeDisplayer::SetNode(const RobotStateNode& node)
{
    // ノードをセットする．
    display_node_ = node;

    if (!calculator_ptr_)
    {
        return;
    }

    // 関節の角度をセットする．
    joint_state_ = calculator_ptr_->CalculateAllJointState(display_node_);
}

void DxlibGuiNodeDisplayer::Update()
{
    // ボタンの更新を行う．
    for (auto& button : button_)
    {
        button->Update();
    }

    if (!IsInWindow())
    {
        SetVisible(false);
    }
}

void DxlibGuiNodeDisplayer::Draw() const
{
    // 枠．
    DrawBackground("NodeDisplay");

    // テキスト．
    if (display_type_ == DisplayMode::kDefualt)
    {
        DrawNodeInfo();
    }
    else if (display_type_ == DisplayMode::kJointState)
    {
        DrawJointInfo();
    }
    else if (display_type_ == DisplayMode::kGlobalPos)
    {
        DrawGlobalPosInfo();
    }

    // ボタンを描画する．
    for (auto& button : button_)
    {
        button->Draw();
    }
}

void DxlibGuiNodeDisplayer::DrawNodeInfo() const
{
    using string_util::EnumToStringRemoveTopK;

    const unsigned int text_color = GetColor(10, 10, 10);
    const unsigned int text_color_dark = GetColor(80, 80, 80);

    const int text_pos_x = gui_left_pos_x_ + 10;
    const int text_pos_y_min = gui_top_pos_y_ + kTitleBarHeight + 10;
    const int text_interval_y = 20;

    const std::array<std::string, HexapodConst::kLegNum> leg_name = {
        "右前", "右中", "右後", "左後", "左中", "左前" };

    int text_line = 0;

    DrawFormatStringToHandle(
        text_pos_x, text_pos_y_min + text_interval_y * (text_line++),
        text_color, font_handle_, "階層");
    DrawFormatStringToHandle(
        text_pos_x, text_pos_y_min + text_interval_y * (text_line++),
        text_color, font_handle_,
        " bit : %s", display_node_.leg_state.to_string().c_str());

    const auto com_pos_name =
        EnumToStringRemoveTopK(
            leg_func::GetDiscreteComPos(display_node_.leg_state));
    const auto com_pos_num = leg_func::GetDiscreteComPos(display_node_.leg_state);

    DrawFormatStringToHandle(
        text_pos_x, text_pos_y_min + text_interval_y * (text_line++),
        text_color, font_handle_,
        " 重心 : %s(%d)", com_pos_name.c_str(), com_pos_num);

    std::string str_leg_pos_right = "";
    std::string str_leg_pos_left = "";
    std::string str_ground = "";

    for (int i = 0; i < HexapodConst::kLegNum; i++)
    {
        if (leg_func::IsGrounded(display_node_.leg_state, i))
        {
            str_ground += "接地,";
        }
        else
        {
            str_ground += "遊脚,";
        }

        const enums::DiscreteLegPos pos =
            leg_func::GetDiscreteLegPos(display_node_.leg_state, i);

        if (i < HexapodConst::kLegNum / 2)
        {
            str_leg_pos_right += std::format(
                "{}-{}({}), ",
                leg_name[i],
                EnumToStringRemoveTopK(pos),
                static_cast<int>(pos));
        }
        else
        {
            str_leg_pos_left += std::format(
                "{}-{}({}), ",
                leg_name[i],
                EnumToStringRemoveTopK(pos),
                static_cast<int>(pos));
        }
    }
    DrawFormatStringToHandle(
        text_pos_x,
        text_pos_y_min + text_interval_y * (text_line++),
        text_color, font_handle_,
        " 脚位置 : ", str_leg_pos_right.c_str());

    DrawFormatStringToHandle(
        text_pos_x,
        text_pos_y_min + text_interval_y * (text_line++),
        text_color, font_handle_,
        "  %s", str_leg_pos_right.c_str());

    DrawFormatStringToHandle(
        text_pos_x, text_pos_y_min + text_interval_y * (text_line++),
        text_color, font_handle_,
        "  %s", str_leg_pos_left.c_str());

    DrawFormatStringToHandle(
        text_pos_x, text_pos_y_min + text_interval_y * (text_line++),
        text_color, font_handle_,
        " 脚の状態：%s", str_ground.c_str());

    // 重心を表示する
    ++text_line;
    DrawFormatStringToHandle(
        text_pos_x,
        text_pos_y_min + text_interval_y * (text_line++),
        text_color, font_handle_,
        "重心位置");

    DrawFormatStringToHandle(
        text_pos_x,
        text_pos_y_min + text_interval_y * (text_line++),
        text_color, font_handle_,
        " %s", display_node_.center_of_mass_global_coord.ToString().c_str());

    // 回転を表示する
    DrawFormatStringToHandle(
        text_pos_x,
        text_pos_y_min + text_interval_y * (text_line++),
        text_color, font_handle_,
        "回転 (w:%5.3f,x:%5.3f,y:%5.3f,z:%5.3f)",
        display_node_.posture.w,
        display_node_.posture.v.x,
        display_node_.posture.v.y,
        display_node_.posture.v.z);

    // オイラー角にして表示する．
    const EulerXYZ euler = ToEulerXYZ(display_node_.posture);

    DrawFormatStringToHandle(
        text_pos_x,
        text_pos_y_min + text_interval_y * (text_line++),
        text_color, font_handle_,
        "　オイラー角(x:%5.3f[deg],y:%5.3f[deg],z:%5.3f[deg])",
        math_util::ConvertRadToDeg(euler.x_angle),
        math_util::ConvertRadToDeg(euler.y_angle),
        math_util::ConvertRadToDeg(euler.z_angle));

    // 脚の位置を表示する．
    ++text_line;
    DrawFormatStringToHandle(
        text_pos_x,
        text_pos_y_min + text_interval_y * (text_line++),
        text_color, font_handle_,
        "脚位置");

    for (int i = 0; i < HexapodConst::kLegNum; i++)
    {
        DrawFormatStringToHandle(
            text_pos_x,
            text_pos_y_min + text_interval_y * (text_line++),
            text_color, font_handle_,
            "%s %s",
            leg_name[i].c_str(),
            display_node_.leg_pos[i].ToString().c_str());
    }

    // 脚の基準座標を表示する．
    for (int i = 0; i < HexapodConst::kLegNum; i++)
    {
        if (display_node_.leg_pos[i] == display_node_.leg_reference_pos[i])
        {
            DrawFormatStringToHandle(
                text_pos_x,
                text_pos_y_min + text_interval_y * (text_line++),
                text_color_dark, font_handle_,
                " %s脚の基準座標は現在の脚位置と同じです．", leg_name[i].c_str());
        }
        else
        {
            DrawFormatStringToHandle(
                text_pos_x,
                text_pos_y_min + text_interval_y * (text_line++),
                text_color_dark, font_handle_,
                " %s脚の基準座標(x:%5.3f,y:%5.3f,z:%5.3f)",
                leg_name[i].c_str(),
                display_node_.leg_reference_pos[i].x,
                display_node_.leg_reference_pos[i].y,
                display_node_.leg_reference_pos[i].z);
        }
    }

    // 深さと次の動作を表示する．
    ++text_line;

    DrawFormatStringToHandle(
        text_pos_x,
        text_pos_y_min + text_interval_y * (text_line++),
        text_color, font_handle_,
        "深さ：%d, 次の動作 : %s",
        display_node_.depth,
        EnumToStringRemoveTopK(display_node_.next_move).c_str());

    DrawFormatStringToHandle(
        text_pos_x,
        text_pos_y_min + text_interval_y * (text_line++),
        text_color, font_handle_,
        "指定がなければ単位は長さが[mm]，角度が[rad]");
}

void DxlibGuiNodeDisplayer::DrawJointInfo() const
{
    using math_util::FloatingPointNumToString;
    using math_util::ConvertRadToDeg;

    const unsigned int text_color = GetColor(10, 10, 10);
    const unsigned int error_text_color = GetColor(128, 10, 10);
    const int text_pos_x = gui_left_pos_x_ + 10;
    const int text_pos_y_min = gui_top_pos_y_ + kTitleBarHeight + 10;
    const int text_interval_y = 20;

    if (!calculator_ptr_)
    {
        DrawFormatStringToHandle(
            text_pos_x,
            text_pos_y_min + text_interval_y * 0,
            text_color,
            font_handle_,
            "計算クラスが nullptr です");
        return;
    }
    if (!checker_ptr_)
    {
        DrawFormatStringToHandle(
            text_pos_x,
            text_pos_y_min + text_interval_y * 0,
            text_color,
            font_handle_,
            "チェッカークラスが nullptr です");
        return;
    }

    for (int i = 0; i < HexapodConst::kLegNum; i++)
    {
        if (joint_state_[i].joint_angle.size() != 3)
        {
            DrawFormatStringToHandle(
                text_pos_x,
                text_pos_y_min + text_interval_y * 0,
                text_color,
                font_handle_,
                "間接の計算ができていない，またはされていません．");

            return;
        }
        if (joint_state_[i].joint_pos_leg_coordinate.size() != 4)
        {
            DrawFormatStringToHandle(
                text_pos_x,
                text_pos_y_min + text_interval_y * 0,
                text_color,
                font_handle_,
                "間接の計算ができていない，またはされていません．");

            return;
        }
    }

    int text_line = 0;


    for (int i = 0; i < HexapodConst::kLegNum; i++)
    {
        const float coxa_angle_deg =
            ConvertRadToDeg(joint_state_[i].joint_angle[0]);
        const float femur_angle_deg =
            ConvertRadToDeg(joint_state_[i].joint_angle[1]);
        const float tibia_angle_deg =
            ConvertRadToDeg(joint_state_[i].joint_angle[2]);

        DrawFormatStringToHandle(
            text_pos_x,
            text_pos_y_min + text_interval_y * (text_line++),
            text_color,
            font_handle_,
            "[%d] c %s[deg],f %s[deg],t %s[deg]",
            i,
            FloatingPointNumToString(coxa_angle_deg).c_str(),
            FloatingPointNumToString(femur_angle_deg).c_str(),
            FloatingPointNumToString(tibia_angle_deg).c_str());

        const auto coxa_to_femur = joint_state_[i].joint_pos_leg_coordinate[0] -
            joint_state_[i].joint_pos_leg_coordinate[1];
        const auto femur_to_tibia = joint_state_[i].joint_pos_leg_coordinate[1] -
            joint_state_[i].joint_pos_leg_coordinate[2];
        const auto tibia_to_end = joint_state_[i].joint_pos_leg_coordinate[2] -
            joint_state_[i].joint_pos_leg_coordinate[3];

        DrawFormatStringToHandle(
            text_pos_x,
            text_pos_y_min + text_interval_y * (text_line++),
            text_color,
            font_handle_,
            "    c %3.3f[mm],f %3.3f[mm],t %3.3f[mm]",
            coxa_to_femur.GetLength(),
            femur_to_tibia.GetLength(),
            tibia_to_end.GetLength());


        if (checker_ptr_->IsLegInRange(
            i, joint_state_[i].joint_pos_leg_coordinate[3]))
        {
            DrawFormatStringToHandle(
                text_pos_x,
                text_pos_y_min + text_interval_y * (text_line++),
                text_color,
                font_handle_,
                "    近似値された可動域内にあります．");
        }
        else
        {
            DrawFormatStringToHandle(
                text_pos_x,
                text_pos_y_min + text_interval_y * (text_line++),
                error_text_color,
                font_handle_,
                "    近似値された可動域外です．");
        }


        std::string str = "";

        const float coxa_min_angle =
            PhantomXMkIIConst::kCoxaDefaultAngle[i] +
            PhantomXMkIIConst::kCoxaAngleMin;
        const float coxa_max_angle =
            PhantomXMkIIConst::kCoxaDefaultAngle[i] +
            PhantomXMkIIConst::kCoxaAngleMax;

        if (joint_state_[i].joint_angle[0] < coxa_min_angle)
        {
            str += "coxa_min ";
        }

        if (joint_state_[i].joint_angle[0] > coxa_max_angle)
        {
            str += "coxa_max ";
        }

        if (joint_state_[i].joint_angle[1] < PhantomXMkIIConst::kFemurAngleMin)
        {
            str += "femur_min ";
        }

        if (joint_state_[i].joint_angle[1] > PhantomXMkIIConst::kFemurAngleMax)
        {
            str += "femur_max ";
        }

        if (joint_state_[i].joint_angle[2] < PhantomXMkIIConst::kTibiaAngleMin)
        {
            str += "tibia_min ";
        }

        if (joint_state_[i].joint_angle[2] > PhantomXMkIIConst::kTibiaAngleMax)
        {
            str += "tibia_max ";
        }

        if (!joint_state_[i].is_in_range)
        {
            str += "脚先が届いていません ";
        }

        if (!str.empty())
        {
            const size_t max_str_size = 30;
            if (str.size() > max_str_size) { str = str.substr(0, max_str_size); }

            DrawFormatStringToHandle(
                text_pos_x,
                text_pos_y_min + text_interval_y * (text_line++),
                error_text_color,
                font_handle_,
                "    実際の可動域の外です． %s", str.c_str());
        }
        else
        {
            DrawFormatStringToHandle(
                text_pos_x,
                text_pos_y_min + text_interval_y * (text_line++),
                text_color,
                font_handle_,
                "    実際の可動域の内です．");
        }
    }
}

void DxlibGuiNodeDisplayer::DrawGlobalPosInfo() const
{
    const unsigned int text_color = GetColor(10, 10, 10);
    const int text_pos_x = gui_left_pos_x_ + 10;
    const int text_pos_y_min = gui_top_pos_y_ + kTitleBarHeight + 10;
    const int text_interval_y = 20;
    const std::array<std::string, HexapodConst::kLegNum> leg_name = {
        "右前", "右中", "右後", "左後", "左中", "左前" };

    int text_line = 0;

    DrawFormatStringToHandle(text_pos_x,
                             text_pos_y_min + text_interval_y * (text_line++),
                             text_color,
                             font_handle_,
                             "脚先座標(ローカル座標)");

    for (int i = 0; i < HexapodConst::kLegNum; i++)
    {
        DrawFormatStringToHandle(
            text_pos_x,
            text_pos_y_min + text_interval_y * (text_line++),
            text_color,
            font_handle_,
            "%s %s",
            leg_name[i].c_str(),
            display_node_.leg_pos[i].ToString().c_str());
    }


    ++text_line;
    DrawFormatStringToHandle(text_pos_x,
                             text_pos_y_min + text_interval_y * (text_line++),
                             text_color,
                             font_handle_,
                             "脚先座標(ロボット座標)");

    for (int i = 0; i < HexapodConst::kLegNum; i++)
    {
        const std::string str =
            converter_ptr_->ConvertLegToRobotCoordinate(
                display_node_.leg_pos[i], i).ToString();

        DrawFormatStringToHandle(
            text_pos_x,
            text_pos_y_min + text_interval_y * (text_line++),
            text_color,
            font_handle_,
            "%s %s", leg_name[i].c_str(), str.c_str());
    }

    ++text_line;
    DrawFormatStringToHandle(text_pos_x,
                             text_pos_y_min + text_interval_y * (text_line++),
                             text_color,
                             font_handle_,
                             "脚先座標(グローバル座標)");

    DrawFormatStringToHandle(
        text_pos_x,
        text_pos_y_min + text_interval_y * (text_line++),
        text_color,
        font_handle_,
        "重心 %s",
        display_node_.center_of_mass_global_coord.ToString().c_str());

    ++text_line;

    for (int i = 0; i < HexapodConst::kLegNum; i++)
    {
        DrawFormatStringToHandle(
            text_pos_x,
            text_pos_y_min + text_interval_y * (text_line++),
            text_color,
            font_handle_,
            "%s %s",
            leg_name[i].c_str(),
            converter_ptr_->ConvertLegToGlobalCoordinate(
            display_node_.leg_pos[i],
            i,
            display_node_.center_of_mass_global_coord,
            display_node_.posture,
            true).ToString().c_str());
    }

    ++text_line;

    // 脚先の付け根からの距離を表示する．
    for (int i = 0; i < HexapodConst::kLegNum; i++)
    {
        DrawFormatStringToHandle(
                    text_pos_x + width_ / 2 * (i % 2),
                    text_pos_y_min + text_interval_y * text_line,
                    text_color,
                    font_handle_,
                    "%s %f",
                    leg_name[i].c_str(),
                    display_node_.leg_pos[i].ProjectedXY().GetLength());

        if (i % 2 == 1) { ++text_line; }
    }
}

bool DxlibGuiNodeDisplayer::IsInWindow() const
{
    return gui_left_pos_x_ < window_x_ && gui_top_pos_y_ < window_y_ &&
        0 < gui_left_pos_x_ + width_ && 0 < gui_top_pos_y_ + height_;
}

}  // namespace designlab
