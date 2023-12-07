#include "dxlib_gui_node_displayer.h"

#include <Dxlib.h>
#include <magic_enum.hpp>

#include "designlab_math_util.h"
#include "designlab_rot_converter.h"
#include "designlab_string_util.h"
#include "font_loader.h"
#include "leg_state.h"
#include "phantomx_mk2_const.h"


namespace dl = ::designlab;
namespace dllf = ::designlab::leg_func;
namespace dlm = ::designlab::math_util;
namespace dlsu = ::designlab::string_util;


DxlibGuiNodeDisplayer::DxlibGuiNodeDisplayer(
	const int pos_x,
	const int pos_y,
	const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
	const std::shared_ptr<const IHexapodJointCalculator>& calculator_ptr,
	const std::shared_ptr<const IHexapodVaildChecker>& checker_ptr
) :
	window_x_(pos_x),
	window_y_(pos_y),
	converter_ptr_(converter_ptr),
	calculator_ptr_(calculator_ptr),
	checker_ptr_(checker_ptr),
	display_type_(DisplayMode::kDefualt),
	font_handle_(FontLoader::GetIns()->GetFontHandle(kFontPath))
{
	//ボタンを作成する
	const int button_size_x = 90;
	const int button_size_y = 30;

	buttons_.push_back(
		std::make_unique<SimpleButton>(
			"基本情報",
			10 + button_size_x / 2,
			gui_top_pos_y_ + kHeight - button_size_y / 2 - 10,
			button_size_x,
			button_size_y
		)
	);

	buttons_.back()->SetActivateFunction([this]() {display_type_ = DisplayMode::kDefualt; });

	buttons_.push_back(
		std::make_unique<SimpleButton>(
			"joint",
			(10 + button_size_x / 2) + (10 + button_size_x),
			gui_top_pos_y_ + kHeight - button_size_y / 2 - 10,
			button_size_x,
			button_size_y
		)
	);

	buttons_.back()->SetActivateFunction([this]() {display_type_ = DisplayMode::kJointState; });

	buttons_.push_back(
		std::make_unique<SimpleButton>(
			"脚先座標",
			(10 + button_size_x / 2) + (10 + button_size_x) * 2,
			gui_top_pos_y_ + kHeight - button_size_y / 2 - 10,
			button_size_x,
			button_size_y
		)
	);

	buttons_.back()->SetActivateFunction([this]() {display_type_ = DisplayMode::kGlobalPos; });

	const int close_button_size = 28;
	const int close_button_x = gui_left_pos_x_ + kWidth - close_button_size / 2 - 2;
	const int close_button_y = gui_top_pos_y_ + close_button_size / 2 + 2;

	buttons_.push_back(std::make_unique<SimpleButton>("×", close_button_x, close_button_y, close_button_size, close_button_size));
	buttons_.back()->SetActivateFunction([this]() { SetVisible(false); });
}

void DxlibGuiNodeDisplayer::SetPos(const int pos_x, const int pos_y, const unsigned int option, const bool this_is_first_time)
{
	const int past_x = gui_left_pos_x_;
	const int past_y = gui_top_pos_y_;

	if (option & dl::kDxlibGuiAnchorLeft) { gui_left_pos_x_ = pos_x; }
	else if (option & dl::kDxlibGuiAnchorMidleX) { gui_left_pos_x_ = pos_x - kWidth / 2; }
	else if (option & dl::kDxlibGuiAnchorRight) { gui_left_pos_x_ = pos_x - kWidth; }

	if (option & dl::kDxlibGuiAnchorTop) { gui_top_pos_y_ = pos_y; }
	else if (option & dl::kDxlibGuiAnchorMidleY) { gui_top_pos_y_ = pos_y - kHeight / 2; }
	else if (option & dl::kDxlibGuiAnchorBottom) { gui_top_pos_y_ = pos_y - kHeight; }

	const int diff_x = gui_left_pos_x_ - past_x;
	const int diff_y = gui_top_pos_y_ - past_y;

	for (auto& button : buttons_)
	{
		button->SetPos(button->GetPosMiddleX() + diff_x, button->GetPosMiddleY() + diff_y, dl::kDxlibGuiAnchorMidleXMidleY);
	}

	if (this_is_first_time)
	{
		set_pos_x_ = gui_left_pos_x_;
		set_pos_y_ = gui_top_pos_y_;
	}
}


void DxlibGuiNodeDisplayer::SetNode(const RobotStateNode& node)
{
	//ノードをセットする
	display_node_ = node;

	if (!calculator_ptr_) { return; }

	// 関節の角度をセットする
	joint_state_ = calculator_ptr_->CalculateAllJointState(display_node_);
}

void DxlibGuiNodeDisplayer::Update()
{
	//ボタンの更新を行う
	for (auto& button : buttons_)
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
	// 枠
	DrawBackground();

	// テキスト
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

	//ボタンを描画する
	for (auto& button : buttons_)
	{
		button->Draw();
	}
}

void DxlibGuiNodeDisplayer::SetVisible(const bool visible)
{
	visible_ = visible;

	for (auto& button : buttons_)
	{
		button->SetVisible(visible);
	}

	if (visible_)
	{
		SetPos(set_pos_x_, set_pos_y_, dl::kDxlibGuiAnchorLeftTop);
	}
}

void DxlibGuiNodeDisplayer::ClickedAction(const int cursor_x, const int cursor_y,
	const int left_pushing_count, [[maybe_unused]] const int middle_pushing_count, [[maybe_unused]] const int right_pushing_count)
{
	if (!is_dragging_ && left_pushing_count > 0)
	{
		is_dragging_ = true;
	}

	if (is_dragging_ && left_pushing_count == 0)
	{
		is_dragging_ = false;
	}

	//ボタンの処理を行う．
	for (auto& button : buttons_)
	{
		if (button->CursorOnGui(cursor_x, cursor_y))
		{
			button->ClickedAction(cursor_x, cursor_y, left_pushing_count, middle_pushing_count, right_pushing_count);
			break;				//1度に1つのボタンしか処理しない．
		}
	}
}

bool DxlibGuiNodeDisplayer::CursorOnGui(int cursor_x, int cursor_y) const noexcept
{
	if (!IsVisible()) { return false; }

	return (gui_left_pos_x_ < cursor_x && cursor_x < gui_left_pos_x_ + kWidth) &&
		(gui_top_pos_y_ < cursor_y && cursor_y < gui_top_pos_y_ + kHeight);
}

bool DxlibGuiNodeDisplayer::IsDraggable(const int cursor_x, const int cursor_y) const
{
	if (!IsVisible()) { return false; }

	return CursorOnGui(cursor_x, cursor_y);
}

void DxlibGuiNodeDisplayer::DraggedAction(const int cursor_dif_x, const int cursor_dif_y, [[maybe_unused]] unsigned int mouse_key_bit)
{
	SetPos(gui_left_pos_x_ + cursor_dif_x, gui_top_pos_y_ + cursor_dif_y, dl::kDxlibGuiAnchorLeftTop);
}

void DxlibGuiNodeDisplayer::DrawBackground() const
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
	const unsigned int text_color = GetColor(10, 10, 10);
	DrawFormatStringToHandle(text_pos_x, text_pos_y, text_color, font_handle_, "NodeDisplay");

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void DxlibGuiNodeDisplayer::DrawNodeInfo() const
{
	const unsigned int text_color = GetColor(10, 10, 10);
	const unsigned int text_color_dark = GetColor(80, 80, 80);

	const int text_pos_x = gui_left_pos_x_ + 10;
	const int text_pos_y_min = gui_top_pos_y_ + kTitleBarHeight + 10;
	const int text_interval_y = kFontSize + 4;

	const std::array<std::string, HexapodConst::kLegNum> leg_name = { "右前","右中","右後","左後","左中","左前" };

	int text_line = 0;

	DrawFormatStringToHandle(text_pos_x, text_pos_y_min + text_interval_y * (text_line++), text_color, font_handle_, "階層");
	DrawFormatStringToHandle(text_pos_x, text_pos_y_min + text_interval_y * (text_line++), text_color, font_handle_,
		" bit : %s", display_node_.leg_state.to_string().c_str()
	);

	DrawFormatStringToHandle(text_pos_x, text_pos_y_min + text_interval_y * (text_line++), text_color, font_handle_,
		" 重心 : %s(%d)",
		dlsu::EnumToStringRemoveTopK(dllf::GetDiscreteComPos(display_node_.leg_state)).c_str(),
		dllf::GetDiscreteComPos(display_node_.leg_state)
	);

	std::string str_leg_pos_right = "";
	std::string str_leg_pos_left = "";
	std::string str_ground = "";
	for (int i = 0; i < HexapodConst::kLegNum; i++)
	{
		if (dllf::IsGrounded(display_node_.leg_state, i)) { str_ground += "接地,"; }
		else { str_ground += "遊脚,"; }

		DiscreteLegPos pos = dllf::GetDiscreteLegPos(display_node_.leg_state, i);

		if (i < HexapodConst::kLegNum / 2)
		{
			str_leg_pos_right += leg_name[i] + "-" + dlsu::EnumToStringRemoveTopK(pos) + "(" + std::to_string(static_cast<int>(pos)) + "), ";
		}
		else
		{
			str_leg_pos_left += leg_name[i] + "-" + dlsu::EnumToStringRemoveTopK(pos) + "(" + std::to_string(static_cast<int>(pos)) + "), ";
		}
	}
	DrawFormatStringToHandle(text_pos_x, text_pos_y_min + text_interval_y * (text_line++), text_color, font_handle_, " 脚位置 : ", str_leg_pos_right.c_str());
	DrawFormatStringToHandle(text_pos_x, text_pos_y_min + text_interval_y * (text_line++), text_color, font_handle_, "  %s", str_leg_pos_right.c_str());
	DrawFormatStringToHandle(text_pos_x, text_pos_y_min + text_interval_y * (text_line++), text_color, font_handle_, "  %s", str_leg_pos_left.c_str());
	DrawFormatStringToHandle(text_pos_x, text_pos_y_min + text_interval_y * (text_line++), text_color, font_handle_, " 脚の状態：%s", str_ground.c_str());

	// 重心を表示する
	++text_line;
	DrawFormatStringToHandle(text_pos_x, text_pos_y_min + text_interval_y * (text_line++), text_color, font_handle_, "重心位置");

	DrawFormatStringToHandle(text_pos_x, text_pos_y_min + text_interval_y * (text_line++), text_color, font_handle_,
		" %s", display_node_.global_center_of_mass.ToString().c_str());

	// 回転を表示する
	DrawFormatStringToHandle(text_pos_x, text_pos_y_min + text_interval_y * (text_line++), text_color, font_handle_,
		"回転 (w:%5.3f,x:%5.3f,y:%5.3f,z:%5.3f)", display_node_.quat.w, display_node_.quat.v.x, display_node_.quat.v.y, display_node_.quat.v.z);

	// オイラー角にして表示する．
	const dl::EulerXYZ euler = dl::ToEulerXYZ(display_node_.quat);
	DrawFormatStringToHandle(text_pos_x, text_pos_y_min + text_interval_y * (text_line++), text_color, font_handle_,
		"　オイラー角(x:%5.3f[deg],y:%5.3f[deg],z:%5.3f[deg])",
		dlm::ConvertRadToDeg(euler.x_angle),
		dlm::ConvertRadToDeg(euler.y_angle),
		dlm::ConvertRadToDeg(euler.z_angle)
	);

	// 脚の位置を表示する
	++text_line;
	DrawFormatStringToHandle(text_pos_x, text_pos_y_min + text_interval_y * (text_line++), text_color, font_handle_, "脚位置");

	for (int i = 0; i < HexapodConst::kLegNum; i++)
	{
		DrawFormatStringToHandle(text_pos_x, text_pos_y_min + text_interval_y * (text_line++), text_color, font_handle_,
			"%s %s", leg_name[i].c_str(), display_node_.leg_pos[i].ToString().c_str());
	}

	// 脚の基準座標を表示する
	for (int i = 0; i < HexapodConst::kLegNum; i++)
	{
		if (display_node_.leg_pos[i] == display_node_.leg_reference_pos[i])
		{
			DrawFormatStringToHandle(text_pos_x, text_pos_y_min + text_interval_y * (text_line++), text_color_dark, font_handle_,
				" %s脚の基準座標は現在の脚位置と同じです．", leg_name[i].c_str());
		}
		else
		{
			DrawFormatStringToHandle(text_pos_x, text_pos_y_min + text_interval_y * (text_line++), text_color_dark, font_handle_,
				" %s脚の基準座標(x:%5.3f,y:%5.3f,z:%5.3f)",
				leg_name[i].c_str(),
				display_node_.leg_reference_pos[i].x,
				display_node_.leg_reference_pos[i].y,
				display_node_.leg_reference_pos[i].z
			);
		}
	}

	// 深さと次の動作を表示する
	++text_line;
	DrawFormatStringToHandle(text_pos_x, text_pos_y_min + text_interval_y * (text_line++), text_color, font_handle_,
		"深さ：%d, 次の動作 : %s", display_node_.depth, dlsu::EnumToStringRemoveTopK(display_node_.next_move).c_str());

	DrawFormatStringToHandle(text_pos_x, text_pos_y_min + text_interval_y * (text_line++), text_color, font_handle_, "指定がなければ単位は長さが[mm]，角度が[rad]");
}

void DxlibGuiNodeDisplayer::DrawJointInfo() const
{
	const unsigned int text_color = GetColor(10, 10, 10);
	const unsigned int error_text_color = GetColor(128, 10, 10);
	const int text_pos_x = gui_left_pos_x_ + 10;
	const int text_pos_y_min = gui_top_pos_y_ + kTitleBarHeight + 10;
	const int text_interval_y = 20;

	if (!calculator_ptr_)
	{
		DrawFormatStringToHandle(text_pos_x, text_pos_y_min + text_interval_y * 0, text_color, font_handle_, "計算クラスがnullptrです");
		return;
	}
	if (!checker_ptr_)
	{
		DrawFormatStringToHandle(text_pos_x, text_pos_y_min + text_interval_y * 0, text_color, font_handle_, "チェッカークラスがnullptrです");
		return;
	}

	for (int i = 0; i < HexapodConst::kLegNum; i++)
	{
		if (joint_state_[i].joint_angle.size() != 3)
		{
			DrawFormatStringToHandle(text_pos_x, text_pos_y_min + text_interval_y * 0, text_color, font_handle_, "間接の計算ができていない，またはされていません．");
			return;
		}
		if (joint_state_[i].joint_pos_leg_coordinate.size() != 4)
		{
			DrawFormatStringToHandle(text_pos_x, text_pos_y_min + text_interval_y * 0, text_color, font_handle_, "間接の計算ができていない，またはされていません．");
			return;
		}
	}

	int text_line = 0;


	for (int i = 0; i < HexapodConst::kLegNum; i++)
	{
		DrawFormatStringToHandle(text_pos_x, text_pos_y_min + text_interval_y * (text_line++), text_color, font_handle_, "[%d] c %s[deg],f %s[deg],t %s[deg]",
			i,
			dlm::ConvertFloatToString(dlm::ConvertRadToDeg(joint_state_[i].joint_angle[0])).c_str(),
			dlm::ConvertFloatToString(dlm::ConvertRadToDeg(joint_state_[i].joint_angle[1])).c_str(),
			dlm::ConvertFloatToString(dlm::ConvertRadToDeg(joint_state_[i].joint_angle[2])).c_str()
		);

		DrawFormatStringToHandle(text_pos_x, text_pos_y_min + text_interval_y * (text_line++), text_color, font_handle_, "    c %3.3f[mm],f %3.3f[mm],t %3.3f[mm]",
			(joint_state_[i].joint_pos_leg_coordinate[0] - joint_state_[i].joint_pos_leg_coordinate[1]).GetLength(),
			(joint_state_[i].joint_pos_leg_coordinate[1] - joint_state_[i].joint_pos_leg_coordinate[2]).GetLength(),
			(joint_state_[i].joint_pos_leg_coordinate[2] - joint_state_[i].joint_pos_leg_coordinate[3]).GetLength()
		);


		if (checker_ptr_->IsLegInRange(i, joint_state_[i].joint_pos_leg_coordinate[3]))
		{
			DrawFormatStringToHandle(text_pos_x, text_pos_y_min + text_interval_y * (text_line++), text_color,
				font_handle_, "    近似値された可動域内にあります．");
		}
		else
		{
			DrawFormatStringToHandle(text_pos_x, text_pos_y_min + text_interval_y * (text_line++), error_text_color,
				font_handle_, "    近似値された可動域外です．");
		}


		std::string str = "";
		if (joint_state_[i].joint_angle[0] < PhantomXMkIIConst::kCoxaDefaultAngle[i] + PhantomXMkIIConst::kCoxaAngleMin) { str += "coxa_min "; }
		if (joint_state_[i].joint_angle[0] > PhantomXMkIIConst::kCoxaDefaultAngle[i] + PhantomXMkIIConst::kCoxaAngleMax) { str += "coxa_max "; }
		if (joint_state_[i].joint_angle[1] < PhantomXMkIIConst::kFemurAngleMin) { str += "femur_min "; }
		if (joint_state_[i].joint_angle[1] > PhantomXMkIIConst::kFemurAngleMax) { str += "femur_max "; }
		if (joint_state_[i].joint_angle[2] < PhantomXMkIIConst::kTibiaAngleMin) { str += "tibia_min "; }
		if (joint_state_[i].joint_angle[2] > PhantomXMkIIConst::kTibiaAngleMax) { str += "tibia_max "; }
		if (!joint_state_[i].is_in_range) { str += "脚先が届いていません "; }

		if (!str.empty())
		{
			const size_t max_str_size = 30;
			if (str.size() > max_str_size) { str = str.substr(0, max_str_size); }

			DrawFormatStringToHandle(text_pos_x, text_pos_y_min + text_interval_y * (text_line++), error_text_color,
				font_handle_, "    実際の可動域の外です． %s", str.c_str());
		}
		else
		{
			DrawFormatStringToHandle(text_pos_x, text_pos_y_min + text_interval_y * (text_line++), text_color,
				font_handle_, "    実際の可動域の内です．");
		}
	}
}

void DxlibGuiNodeDisplayer::DrawGlobalPosInfo() const
{
	const unsigned int text_color = GetColor(10, 10, 10);
	const int text_pos_x = gui_left_pos_x_ + 10;
	const int text_pos_y_min = gui_top_pos_y_ + kTitleBarHeight + 10;
	const int text_interval_y = kFontSize + 4;
	const std::array<std::string, HexapodConst::kLegNum> leg_name = { "右前","右中","右後","左後","左中","左前" };

	int text_line = 0;

	DrawFormatStringToHandle(text_pos_x, text_pos_y_min + text_interval_y * (text_line++), text_color, font_handle_, "脚先座標(ローカル座標)");

	for (int i = 0; i < HexapodConst::kLegNum; i++)
	{
		DrawFormatStringToHandle(
			text_pos_x,
			text_pos_y_min + text_interval_y * (text_line++),
			text_color,
			font_handle_,
			"%s %s",
			leg_name[i].c_str(),
			display_node_.leg_pos[i].ToString().c_str()
		);
	}


	++text_line;
	DrawFormatStringToHandle(text_pos_x, text_pos_y_min + text_interval_y * (text_line++), text_color, font_handle_, "脚先座標(ロボット座標)");

	for (int i = 0; i < HexapodConst::kLegNum; i++)
	{
		DrawFormatStringToHandle(
			text_pos_x,
			text_pos_y_min + text_interval_y * (text_line++),
			text_color,
			font_handle_,
			"%s %s",
			leg_name[i].c_str(),
			converter_ptr_->ConvertLegToRobotCoordinate(display_node_.leg_pos[i], i).ToString().c_str()
		);
	}

	++text_line;
	DrawFormatStringToHandle(text_pos_x, text_pos_y_min + text_interval_y * (text_line++), text_color, font_handle_, "脚先座標(グローバル座標)");

	DrawFormatStringToHandle(
		text_pos_x,
		text_pos_y_min + text_interval_y * (text_line++),
		text_color,
		font_handle_,
		"重心 %s",
		display_node_.global_center_of_mass.ToString().c_str()
	);

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
				display_node_.leg_pos[i], i, display_node_.global_center_of_mass, display_node_.quat, true
			).ToString().c_str()
		);
	}
}

bool DxlibGuiNodeDisplayer::IsInWindow() const
{
	return gui_left_pos_x_ < window_x_ && gui_top_pos_y_ < window_y_ &&
		0 < gui_left_pos_x_ + kWidth && 0 < gui_top_pos_y_ + kHeight;
}