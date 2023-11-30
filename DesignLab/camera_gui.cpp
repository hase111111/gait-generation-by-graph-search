#include "camera_gui.h"

#include <string>

#include "dxlib_util.h"
#include "font_loader.h"
#include "mouse.h"


namespace dl = ::designlab;
namespace dldu = ::designlab::dxlib_util;


CameraGui::CameraGui(const std::shared_ptr<DxlibCamera> camera) :
	camera_(camera),
	font_handle_(FontLoader::GetIns()->GetFontHandle(kFontPath))
{
	const int button_distance = 10;	//!< ボタン同士の間隔
	const int button_size = 60;		//!< ボタンのサイズ

	const int button_range = button_size + button_distance;
	const int left_pos_x = gui_left_pos_x_ + button_range / 2 + 15;
	const int top_pos_y = gui_top_pos_y_ + button_range / 2 + 40;

	button_.push_back(std::make_unique<SimpleButton>("Reset\nZoom", left_pos_x, top_pos_y, button_size, button_size));
	button_.back()->SetActivateFunction([this]() { camera_->InitCaneraTargetLength(); });

	button_.push_back(std::make_unique<SimpleButton>("Front", left_pos_x + button_range, top_pos_y, button_size, button_size));
	button_.back()->SetActivateFunction([this]() { camera_->SetCameraViewMode(CameraViewMode::kFrontView); });

	button_.push_back(std::make_unique<SimpleButton>("Left", left_pos_x, top_pos_y + button_range, button_size, button_size));
	button_.back()->SetActivateFunction([this]() { camera_->SetCameraViewMode(CameraViewMode::kLeftSideView); });

	button_.push_back(std::make_unique<SimpleButton>("Top", left_pos_x + button_range, top_pos_y + button_range, button_size, button_size));
	button_.back()->SetActivateFunction([this]() { camera_->SetCameraViewMode(CameraViewMode::kTopView); });

	button_.push_back(std::make_unique<SimpleButton>("Right", left_pos_x + button_range * 2, top_pos_y + button_range, button_size, button_size));
	button_.back()->SetActivateFunction([this]() { camera_->SetCameraViewMode(CameraViewMode::kRightSideView); });

	button_.push_back(std::make_unique<SimpleButton>("Back", left_pos_x + button_range, top_pos_y + button_range * 2, button_size, button_size));
	button_.back()->SetActivateFunction([this]() { camera_->SetCameraViewMode(CameraViewMode::kBackView); });

	button_.push_back(std::make_unique<SimpleButton>("Reset\nTarget", left_pos_x + button_range * 2, top_pos_y, button_size, button_size));
	button_.back()->SetActivateFunction([this]() { camera_->SetCameraViewMode(CameraViewMode::kFreeControlled); });
}

void CameraGui::SetPos(const int pos_x, const int pos_y, const unsigned int option)
{
	const int past_x = gui_left_pos_x_;
	const int past_y = gui_top_pos_y_;

	if (option & dl::kOptionLeft) { gui_left_pos_x_ = pos_x; }
	else if (option & dl::kOptionMidleX) { gui_left_pos_x_ = pos_x - kWidth / 2; }
	else if (option & dl::kOptionRight) { gui_left_pos_x_ = pos_x - kWidth; }

	if (option & dl::kOptionTop) { gui_top_pos_y_ = pos_y; }
	else if (option & dl::kOptionMidleY) { gui_top_pos_y_ = pos_y - kHeight / 2; }
	else if (option & dl::kOptionBottom) { gui_top_pos_y_ = pos_y - kHeight; }

	const int diff_x = gui_left_pos_x_ - past_x;
	const int diff_y = gui_top_pos_y_ - past_y;

	for (auto& button : button_)
	{
		button->SetPos(button->GetPosMiddleX() + diff_x, button->GetPosMiddleY() + diff_y, dl::kOptionMidleXMidleY);
	}
}

void CameraGui::SetHexapodPos(const dl::Vector3& pos)
{
	camera_->SetTargetPos(pos);
}

void CameraGui::Update()
{
	//各ボタンの処理
	for (auto& button : button_)
	{
		button->Update();
	}

	//カメラの更新
	camera_->Update();
}

void CameraGui::Draw() const
{
	DrawBackground();

	//全てのボタンの描画
	for (auto& button : button_)
	{
		button->Draw();
	}

	DrawString();
}

void CameraGui::SetVisible(const bool visible)
{
	visible_ = visible;

	for (auto& button : button_)
	{
		button->SetVisible(visible);
	}
}

void CameraGui::ClickedAction(const int cursor_x, const int cursor_y,
	const int left_pushing_count, [[maybe_unused]] const int middle_pushing_count, [[maybe_unused]] const int right_pushing_count)
{
	//各ボタンの処理
	for (auto& button : button_)
	{
		if (button->CursorOnGui(cursor_x, cursor_y) && left_pushing_count == 1)
		{
			button->ClickedAction(cursor_x, cursor_y, left_pushing_count, middle_pushing_count, right_pushing_count);
		}
	}
}

bool CameraGui::CursorOnGui(const int cursor_x, const int cursor_y) const noexcept
{
	return gui_left_pos_x_ < cursor_x && cursor_x < gui_left_pos_x_ + kWidth &&
		gui_top_pos_y_ < cursor_y && cursor_y < gui_top_pos_y_ + kHeight;
}

bool CameraGui::IsDraggable(int cursor_x, int cursor_y) const
{
	return CursorOnGui(cursor_x, cursor_y);
}

void CameraGui::DraggedAction(int cursor_dif_x, int cursor_dif_y, [[maybe_unused]] unsigned int mouse_key_bit)
{
	SetPos(gui_left_pos_x_ + cursor_dif_x, gui_top_pos_y_ + cursor_dif_y, dl::kOptionLeftTop);
}

void CameraGui::DrawBackground() const
{
	if (!visible_) { return; }

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
	const unsigned int text_color = GetColor(10, 10, 10);
	DrawFormatStringToHandle(text_pos_x, text_pos_y, text_color, font_handle_, "CameraGui");

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void CameraGui::DrawString() const
{
	const unsigned int str_color = GetColor(54, 54, 54);

	const int text_interval_y = 20;
	const int text_top_y = gui_top_pos_y_ + 250;

	int text_line = 0;

	DrawFormatStringToHandle(gui_left_pos_x_ + 10, text_top_y + text_interval_y * (text_line++), str_color, font_handle_, "ズーム");
	DrawFormatStringToHandle(gui_left_pos_x_ + 10, text_top_y + text_interval_y * (text_line++), str_color, font_handle_, " ・マウスホイール回転");

	DrawFormatStringToHandle(gui_left_pos_x_ + 10, text_top_y + text_interval_y * (text_line++), str_color, font_handle_, "ビューを回転");
	DrawFormatStringToHandle(gui_left_pos_x_ + 10, text_top_y + text_interval_y * (text_line++), str_color, font_handle_, " ・ホイールクリック＆ドラッグ");

	DrawFormatStringToHandle(gui_left_pos_x_ + 10, text_top_y + text_interval_y * (text_line++), str_color, font_handle_, "画面の中心から回転");
	DrawFormatStringToHandle(gui_left_pos_x_ + 10, text_top_y + text_interval_y * (text_line++), str_color, font_handle_, " ・左クリック＆ドラッグ");

	DrawFormatStringToHandle(gui_left_pos_x_ + 10, text_top_y + text_interval_y * (text_line++), str_color, font_handle_, "画面の平行移動");
	DrawFormatStringToHandle(gui_left_pos_x_ + 10, text_top_y + text_interval_y * (text_line++), str_color, font_handle_, " ・右クリック＆ドラッグ");
}