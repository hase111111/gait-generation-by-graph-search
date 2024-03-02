
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory,
//! Saitama University All right reserved.

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
    AbstractDxlibGui{ 470 ,340 },
    window_x_(window_x),
    window_y_(window_y),
    camera_ptr_(camera_ptr)
{
    const int close_button_size = 28;
    const int close_button_x = gui_left_pos_x_ + width_ - close_button_size / 2 - 2;
    const int close_button_y = gui_top_pos_y_ + close_button_size / 2 + 2;

    button_.push_back(std::make_unique<SimpleButton>("×", close_button_x, close_button_y, close_button_size, close_button_size));
    button_.back()->SetActivateFunction([this]() { SetVisible(false); });
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
    DrawBackground("CameraParameterDisplayer");

    // ボタンを描画する．
    for (const auto& i : button_)
    {
        i->Draw();
    }

    DrawCameraParameter();
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
        0 < gui_left_pos_x_ + width_ && 0 < gui_top_pos_y_ + height_;
}

}  // namespace designlab
