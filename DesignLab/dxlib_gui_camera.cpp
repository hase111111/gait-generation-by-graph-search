
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory,
//!  Saitama University All right reserved.

#include "dxlib_gui_camera.h"

#include <string>

#include "dxlib_util.h"
#include "font_loader.h"
#include "mouse.h"


namespace designlab
{

DxlibGuiCamera::DxlibGuiCamera(const int window_x, const int window_y,
                               const std::shared_ptr<DxlibCamera> camera) :
    AbstractDxlibGui(kWidth, kHeight),
    window_x_(window_x),
    window_y_(window_y),
    camera_(camera)
{
    const int button_distance = 10;  //!< ボタン同士の間隔．
    const int button_size = 60;  //!< ボタンのサイズ．

    const int button_range = button_size + button_distance;
    const int left_pos_x = gui_left_pos_x_ + button_range / 2 + 15;
    const int top_pos_y = gui_top_pos_y_ + button_range / 2 + 40;

    button_.push_back(std::make_unique<SimpleButton>(
        "Reset\nZoom", left_pos_x, top_pos_y, button_size, button_size));
    button_.back()->SetActivateFunction(
        [this]() { camera_->InitCameraTargetLength(); });

    button_.push_back(std::make_unique<SimpleButton>(
        "Front",
        left_pos_x + button_range, top_pos_y, button_size, button_size));
    button_.back()->SetActivateFunction([this]() {
        camera_->SetCameraViewMode(enums::CameraViewMode::kFrontView); });

    button_.push_back(std::make_unique<SimpleButton>(
        "Left", left_pos_x, top_pos_y + button_range, button_size, button_size));
    button_.back()->SetActivateFunction([this]() {
        camera_->SetCameraViewMode(enums::CameraViewMode::kLeftSideView); });

    button_.push_back(std::make_unique<SimpleButton>(
        "Top", left_pos_x + button_range, top_pos_y + button_range,
        button_size, button_size));
    button_.back()->SetActivateFunction([this]() {
        camera_->SetCameraViewMode(enums::CameraViewMode::kTopView); });

    button_.push_back(std::make_unique<SimpleButton>(
        "Right", left_pos_x + button_range * 2, top_pos_y + button_range,
        button_size, button_size));
    button_.back()->SetActivateFunction([this]() {
        camera_->SetCameraViewMode(enums::CameraViewMode::kRightSideView); });

    button_.push_back(std::make_unique<SimpleButton>(
        "Back", left_pos_x + button_range, top_pos_y + button_range * 2,
        button_size, button_size));
    button_.back()->SetActivateFunction([this]() {
        camera_->SetCameraViewMode(enums::CameraViewMode::kBackView); });

    button_.push_back(std::make_unique<SimpleButton>(
        "Reset\nTarget", left_pos_x + button_range * 2, top_pos_y,
        button_size, button_size));
    button_.back()->SetActivateFunction([this]() {
        camera_->SetCameraViewMode(enums::CameraViewMode::kFreeControlled); });

    const int close_button_size = 28;
    const int close_button_x = gui_left_pos_x_ + kWidth - close_button_size / 2 - 2;
    const int close_button_y = gui_top_pos_y_ + close_button_size / 2 + 2;

    button_.push_back(std::make_unique<SimpleButton>(
        "×", close_button_x, close_button_y,
        close_button_size, close_button_size));
    button_.back()->SetActivateFunction([this]() { SetVisible(false); });
}

void DxlibGuiCamera::SetNode(const RobotStateNode& node)
{
    camera_->SetTargetPos(node.center_of_mass_global_coord);
}

void DxlibGuiCamera::Update()
{
    // 各ボタンの処理．
    for (auto& button : button_)
    {
        button->Update();
    }

    // カメラの更新．
    camera_->Update();

    if (!IsInWindow())
    {
        SetVisible(false);
    }
}

void DxlibGuiCamera::Draw() const
{
    DrawBackground("CameraGui");

    // 全てのボタンの描画．
    for (auto& button : button_)
    {
        button->Draw();
    }

    DrawString();
}

void DxlibGuiCamera::DrawString() const
{
    const unsigned int str_color = GetColor(54, 54, 54);

    const int text_interval_y = 20;
    const int text_top_y = gui_top_pos_y_ + 250;

    int text_line = 0;

    DrawFormatStringToHandle(
        gui_left_pos_x_ + 10, text_top_y + text_interval_y * (text_line++),
        str_color, font_handle_, "ズーム");
    DrawFormatStringToHandle(
        gui_left_pos_x_ + 10, text_top_y + text_interval_y * (text_line++),
        str_color, font_handle_, " ・マウスホイール回転");

    DrawFormatStringToHandle(
        gui_left_pos_x_ + 10, text_top_y + text_interval_y * (text_line++),
        str_color, font_handle_, "ビューを回転");
    DrawFormatStringToHandle(
        gui_left_pos_x_ + 10, text_top_y + text_interval_y * (text_line++),
        str_color, font_handle_, " ・ホイールクリック＆ドラッグ");

    DrawFormatStringToHandle(
        gui_left_pos_x_ + 10, text_top_y + text_interval_y * (text_line++),
        str_color, font_handle_, "画面の中心から回転");
    DrawFormatStringToHandle(
        gui_left_pos_x_ + 10, text_top_y + text_interval_y * (text_line++),
        str_color, font_handle_, " ・左クリック＆ドラッグ");

    DrawFormatStringToHandle(
        gui_left_pos_x_ + 10, text_top_y + text_interval_y * (text_line++),
        str_color, font_handle_, "画面の平行移動");
    DrawFormatStringToHandle(
        gui_left_pos_x_ + 10, text_top_y + text_interval_y * (text_line++),
        str_color, font_handle_, " ・右クリック＆ドラッグ");
}

bool DxlibGuiCamera::IsInWindow() const
{
    return gui_left_pos_x_ < window_x_ && gui_top_pos_y_ < window_y_ &&
        0 < gui_left_pos_x_ + kWidth && 0 < gui_top_pos_y_ + kHeight;
}

}  // namespace designlab
