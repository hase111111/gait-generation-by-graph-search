#include "camera_gui.h"

#include <string>

#include "dxlib_util.h"
#include "mouse.h"


namespace dl = ::designlab;
namespace dldu = ::designlab::dxlib_util;


CameraGui::CameraGui(const std::shared_ptr<DxlibCamera> camera) : 	camera_(camera)
{
	const int kButtonDistance = 10;	//!< ボタン同士の間隔
	const int kButtonSize = 60;		//!< ボタンのサイズ

	const int kCloseButtonSizeX = 100;
	const int kCloseButtonSizeY = 30;

	button_.push_back(
		std::make_unique<SimpleButton>(
			"最大/小化",
			gui_left_pos_x_ + kWidth - kCloseButtonSizeX / 2 - 10,
			gui_top_pos_y_ + kCloseButtonSizeY - 10,
			kCloseButtonSizeX,
			kCloseButtonSizeY
		)
	);

	button_.back()->SetActivateFunction([this]()
		{
			for (size_t i = 1; i < button_.size(); i++)
			{
				button_[i]->SetVisible(false);
			}
		}
	);

	const int kButtonRange = kButtonSize + kButtonDistance;
	const int kLeftPosX = gui_left_pos_x_ + kButtonRange / 2 + 15;
	const int kTopPosY = gui_top_pos_y_ + kButtonRange / 2 + kCloseButtonSizeY + 10;

	button_.push_back(std::make_unique<SimpleButton>("Reset\nZoom", kLeftPosX, kTopPosY, kButtonSize, kButtonSize));
	button_.back()->SetActivateFunction([this]() { camera_->InitCaneraTargetLength(); });

	button_.push_back(std::make_unique<SimpleButton>("Front", kLeftPosX + kButtonRange, kTopPosY, kButtonSize, kButtonSize));
	button_.back()->SetActivateFunction([this]() { camera_->SetCameraViewMode(CameraViewMode::kFrontView); });

	button_.push_back(std::make_unique<SimpleButton>("Left", kLeftPosX, kTopPosY + kButtonRange, kButtonSize, kButtonSize));
	button_.back()->SetActivateFunction([this]() { camera_->SetCameraViewMode(CameraViewMode::kLeftSideView); });

	button_.push_back(std::make_unique<SimpleButton>("Top", kLeftPosX + kButtonRange, kTopPosY + kButtonRange, kButtonSize, kButtonSize));
	button_.back()->SetActivateFunction([this]() { camera_->SetCameraViewMode(CameraViewMode::kTopView); });

	button_.push_back(std::make_unique<SimpleButton>("Right", kLeftPosX + kButtonRange * 2, kTopPosY + kButtonRange, kButtonSize, kButtonSize));
	button_.back()->SetActivateFunction([this]() { camera_->SetCameraViewMode(CameraViewMode::kRightSideView); });

	button_.push_back(std::make_unique<SimpleButton>("Back", kLeftPosX + kButtonRange, kTopPosY + kButtonRange * 2, kButtonSize, kButtonSize));
	button_.back()->SetActivateFunction([this]() { camera_->SetCameraViewMode(CameraViewMode::kBackView); });

	button_.push_back(std::make_unique<SimpleButton>("Reset\nTarget", kLeftPosX + kButtonRange * 2, kTopPosY, kButtonSize, kButtonSize));
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

void CameraGui::SetHexapodPos(const designlab::Vector3& pos)
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

void CameraGui::DrawBackground() const
{
	const unsigned int kBackColor = GetColor(255, 255, 255);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
	DrawBox(gui_left_pos_x_, gui_top_pos_y_, gui_left_pos_x_ + kWidth, gui_top_pos_y_ + kHeight, kBackColor, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void CameraGui::DrawString() const
{
	const unsigned int kStrColor = GetColor(54, 54, 54);
	const unsigned int kStrRedColor = GetColor(255, 128, 128);

	const int kTextYInterval = 20;
	const int kTextYTop = gui_top_pos_y_ + 250;

	int text_line = 0;

	DrawFormatString(gui_left_pos_x_ + 10, kTextYTop + kTextYInterval * (text_line++),
		true /*Mouse::GetIns()->GetWheelRot() == 0 */ ? kStrColor : kStrRedColor, "マウスホイール回転");

	DrawFormatString(gui_left_pos_x_ + 10, kTextYTop + kTextYInterval * (text_line++),
		true /*Mouse::GetIns()->GetWheelRot() == 0 */ ? kStrColor : kStrRedColor, " ・ズーム");

	DrawFormatString(gui_left_pos_x_ + 10, kTextYTop + kTextYInterval * (text_line++),
		true /*Mouse::GetIns()->GetPressingCount(MOUSE_INPUT_MIDDLE) == 0 */ ? kStrColor : kStrRedColor, "ホイールクリック＆ドラッグ");

	DrawFormatString(gui_left_pos_x_ + 10, kTextYTop + kTextYInterval * (text_line++),
		true /*Mouse::GetIns()->GetPressingCount(MOUSE_INPUT_MIDDLE) == 0 */ ? kStrColor : kStrRedColor, " ・ビューを回転");

	DrawFormatString(gui_left_pos_x_ + 10, kTextYTop + kTextYInterval * (text_line++),
		true /*Mouse::GetIns()->GetPressingCount(MOUSE_INPUT_LEFT) == 0 */ ? kStrColor : kStrRedColor, "左クリック＆ドラッグ");

	DrawFormatString(gui_left_pos_x_ + 10, kTextYTop + kTextYInterval * (text_line++),
		true /*Mouse::GetIns()->GetPressingCount(MOUSE_INPUT_LEFT) == 0 */ ? kStrColor : kStrRedColor, " ・画面の中心から回転");

	DrawFormatString(gui_left_pos_x_ + 10, kTextYTop + kTextYInterval * (text_line++),
		true /*Mouse::GetIns()->GetPressingCount(MOUSE_INPUT_RIGHT) == 0 */ ? kStrColor : kStrRedColor, "右クリック＆ドラッグ");

	DrawFormatString(gui_left_pos_x_ + 10, kTextYTop + kTextYInterval * (text_line++),
		true /*Mouse::GetIns()->GetPressingCount(MOUSE_INPUT_RIGHT) == 0 */ ? kStrColor : kStrRedColor, " ・画面の平行移動");
}