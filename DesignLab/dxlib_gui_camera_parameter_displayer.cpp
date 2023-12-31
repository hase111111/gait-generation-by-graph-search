
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#include "dxlib_gui_camera_parameter_displayer.h"

#include <magic_enum.hpp>

#include "font_loader.h"
#include "math_rot_converter.h"
#include "string_util.h"


namespace designlab
{

DxlibGuiCameraParameterDisplayer::DxlibGuiCameraParameterDisplayer(
  const int window_x,
  const int window_y,
  const std::shared_ptr<DxlibCamera> camera_ptr) :
    window_x_(window_x),
    window_y_(window_y),
    camera_ptr_(camera_ptr)
{
    const int close_button_size = 28;
    const int close_button_x = gui_left_pos_x_ + kWidth - close_button_size / 2 - 2;
    const int close_button_y = gui_top_pos_y_ + close_button_size / 2 + 2;

    button_.push_back(std::make_unique<SimpleButton>("×", close_button_x, close_button_y, close_button_size, close_button_size));
    button_.back()->SetActivateFunction([this]() { SetVisible(false); });
}

void DxlibGuiCameraParameterDisplayer::SetPos(const int pos_x, const int pos_y, unsigned int option, const bool this_is_first_time)
{
    const int past_x = gui_left_pos_x_;
    const int past_y = gui_top_pos_y_;

    if (option & kDxlibGuiAnchorLeft)
    {
        gui_left_pos_x_ = pos_x;
    }
    else if (option & kDxlibGuiAnchorMiddleX)
    {
        gui_left_pos_x_ = pos_x - kWidth / 2;
    }
    else if (option & kDxlibGuiAnchorRight)
    {
        gui_left_pos_x_ = pos_x - kWidth;
    }

    if (option & kDxlibGuiAnchorTop)
    {
        gui_top_pos_y_ = pos_y;
    }
    else if (option & kDxlibGuiAnchorMiddleY)
    {
        gui_top_pos_y_ = pos_y - kHeight / 2;
    }
    else if (option & kDxlibGuiAnchorBottom)
    {
        gui_top_pos_y_ = pos_y - kHeight;
    }

    const int diff_x = gui_left_pos_x_ - past_x;
    const int diff_y = gui_top_pos_y_ - past_y;

    for (auto& button : button_)
    {
        button->SetPos(button->GetPosMiddleX() + diff_x, button->GetPosMiddleY() + diff_y, kDxlibGuiAnchorMiddleXMiddleY);
    }

    if (this_is_first_time)
    {
        set_pos_x_ = gui_left_pos_x_;
        set_pos_y_ = gui_top_pos_y_;
    }
}

void DxlibGuiCameraParameterDisplayer::Update()
{
    // ボタンを更新する．
    for (auto& i : button_)
    {
        i->Update();
    }

    if (!IsInWindow())
    {
        SetVisible(false);
    }
}

void DxlibGuiCameraParameterDisplayer::Draw() const
{
    // 枠．
    DrawBackground();

    // ボタンを描画する．
    for (const auto& i : button_)
    {
        i->Draw();
    }

    DrawCameraParameter();
}

void DxlibGuiCameraParameterDisplayer::SetVisible(const bool visible)
{
    visible_ = visible;

    for (auto& i : button_)
    {
        i->SetVisible(visible);
    }

    if (visible_)
    {
        SetPos(set_pos_x_, set_pos_y_, kDxlibGuiAnchorLeftTop);
    }
}

void DxlibGuiCameraParameterDisplayer::ClickedAction(const int cursor_x, const int cursor_y,
                           const int left_pushing_count, [[maybe_unused]] const int middle_pushing_count, [[maybe_unused]] const int right_pushing_count)
{
    // ボタンを更新する．
    for (auto& i : button_)
    {
        if (i->CursorOnGui(cursor_x, cursor_y))
        {
            i->ClickedAction(cursor_x, cursor_y, left_pushing_count, middle_pushing_count, right_pushing_count);
            break;
        }
    }
}

bool DxlibGuiCameraParameterDisplayer::CursorOnGui(const int cursor_x, const int cursor_y) const noexcept
{
    if (!IsVisible())
    {
        return false;
    }

    return (gui_left_pos_x_ < cursor_x && cursor_x < gui_left_pos_x_ + kWidth) &&
        (gui_top_pos_y_ < cursor_y && cursor_y < gui_top_pos_y_ + kHeight);
}

bool DxlibGuiCameraParameterDisplayer::IsDraggable(const int cursor_x, const int cursor_y) const
{
    if (!IsVisible())
    {
        return false;
    }

    return (gui_left_pos_x_ < cursor_x && cursor_x < gui_left_pos_x_ + kWidth) &&
        (gui_top_pos_y_ < cursor_y && cursor_y < gui_top_pos_y_ + kHeight);
}

void DxlibGuiCameraParameterDisplayer::DraggedAction(const int cursor_dif_x, const int cursor_dif_y, [[maybe_unused]] const unsigned int mouse_key_bit)
{
    SetPos(gui_left_pos_x_ + cursor_dif_x, gui_top_pos_y_ + cursor_dif_y, kDxlibGuiAnchorLeftTop);
}

void DxlibGuiCameraParameterDisplayer::DrawBackground() const
{
    const unsigned int base_color = GetColor(255, 255, 255);
    const unsigned int frame_color = GetColor(30, 30, 30);
    const unsigned int alpha = 200;

    const int frame_width = 1;

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

    DrawBox(gui_left_pos_x_ - frame_width, gui_top_pos_y_ - frame_width,
        gui_left_pos_x_ + kWidth + frame_width, gui_top_pos_y_ + kHeight + frame_width, frame_color, TRUE);
    DrawBox(gui_left_pos_x_, gui_top_pos_y_, gui_left_pos_x_ + kWidth, gui_top_pos_y_ + kHeight, base_color, TRUE);

    DrawBox(gui_left_pos_x_, gui_top_pos_y_, gui_left_pos_x_ + kWidth, gui_top_pos_y_ + kTitleBarHeight, base_color, TRUE);
    DrawBox(gui_left_pos_x_ - frame_width, gui_top_pos_y_ - frame_width,
        gui_left_pos_x_ + kWidth + frame_width, gui_top_pos_y_ + kTitleBarHeight + frame_width, frame_color, FALSE);


    const int text_pos_x = gui_left_pos_x_ + 10;
    const int text_pos_y = gui_top_pos_y_ + 10;
    const int font_handle = FontLoader::GetIns()->GetFontHandle("font/Yu_Gothic_UI.dft");
    const unsigned int text_color = GetColor(10, 10, 10);
    DrawFormatStringToHandle(text_pos_x, text_pos_y, text_color, font_handle, "CameraParameterDisplayer");

    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void DxlibGuiCameraParameterDisplayer::DrawCameraParameter() const
{
    const unsigned int text_color = GetColor(10, 10, 10);

    const int font_handle = FontLoader::GetIns()->GetFontHandle("font/Yu_Gothic_UI.dft");
    const int text_pos_x = gui_left_pos_x_ + 10;
    const int text_pos_y_min = gui_top_pos_y_ + kTitleBarHeight + 10;
    const int text_interval_y = kFontSize + 4;

    int text_line = 0;

    DrawFormatStringToHandle(text_pos_x, text_pos_y_min + text_interval_y * (text_line++), text_color, font_handle, "カメラの向き(正規化クォータニオン)");
    DrawFormatStringToHandle(text_pos_x, text_pos_y_min + text_interval_y * (text_line++), text_color, font_handle, "　(w:%5.3f,x:%5.3f,y:%5.3f,z:%5.3f)",
                 camera_ptr_->GetNowCameraQuat().w, camera_ptr_->GetNowCameraQuat().v.x, camera_ptr_->GetNowCameraQuat().v.y, camera_ptr_->GetNowCameraQuat().v.z);

    EulerXYZ euler_xyz = ToEulerXYZ(camera_ptr_->GetNowCameraQuat());
    DrawFormatStringToHandle(text_pos_x, text_pos_y_min + text_interval_y * (text_line++), text_color, font_handle, "カメラの向き(オイラー角)");
    DrawFormatStringToHandle(text_pos_x, text_pos_y_min + text_interval_y * (text_line++), text_color, font_handle, "　(roll:%5.3f[deg],pitch:%5.3f[deg],yaw:%5.3f[deg])",
                 math_util::ConvertRadToDeg(euler_xyz.x_angle), math_util::ConvertRadToDeg(euler_xyz.y_angle), math_util::ConvertRadToDeg(euler_xyz.z_angle));

    text_line++;
    DrawFormatStringToHandle(text_pos_x, text_pos_y_min + text_interval_y * (text_line++), text_color, font_handle, "カメラの表示モード");
    DrawFormatStringToHandle(text_pos_x, text_pos_y_min + text_interval_y * (text_line++), text_color, font_handle, "　%s",
                 static_cast<std::string>(magic_enum::enum_name(camera_ptr_->GetCameraViewMode())).c_str());

    DrawFormatStringToHandle(text_pos_x, text_pos_y_min + text_interval_y * (text_line++), text_color, font_handle, "カメラの注視点の座標");
    DrawFormatStringToHandle(text_pos_x, text_pos_y_min + text_interval_y * (text_line++), text_color, font_handle, "　(x:%5.3f [mm],y:%5.3f [mm],z:%5.3f [mm])",
                 camera_ptr_->GetNowTargetPos().x, camera_ptr_->GetNowTargetPos().y, camera_ptr_->GetNowTargetPos().z);

    DrawFormatStringToHandle(text_pos_x, text_pos_y_min + text_interval_y * (text_line++), text_color, font_handle, "カメラと対象との距離");
    DrawFormatStringToHandle(text_pos_x, text_pos_y_min + text_interval_y * (text_line++), text_color, font_handle, "　%5.3f [mm]", camera_ptr_->GetNowCameraToTargetLength());

    text_line++;
    DrawFormatStringToHandle(text_pos_x, text_pos_y_min + text_interval_y * (text_line++), text_color, font_handle, "カメラの位置");
    DrawFormatStringToHandle(text_pos_x, text_pos_y_min + text_interval_y * (text_line++), text_color, font_handle, "　(x:%5.3f [mm],y:%5.3f [mm],z:%5.3f [mm])",
                 camera_ptr_->GetNowCameraPos().x, camera_ptr_->GetNowCameraPos().y, camera_ptr_->GetNowCameraPos().z);
}

bool DxlibGuiCameraParameterDisplayer::IsInWindow() const
{
    return gui_left_pos_x_ < window_x_ && gui_top_pos_y_ < window_y_ &&
        0 < gui_left_pos_x_ + kWidth && 0 < gui_top_pos_y_ + kHeight;
}

}  // namespace designlab
