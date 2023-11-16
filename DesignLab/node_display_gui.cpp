#include "node_display_gui.h"

#include <Dxlib.h>
#include <magic_enum.hpp>

#include "designlab_math_util.h"
#include "designlab_rot_converter.h"
#include "designlab_string_util.h"
#include "font_loader.h"
#include "leg_state.h"
#include "mouse.h"
#include "phantomx_mk2_const.h"


namespace dl = ::designlab;
namespace dllf = ::designlab::leg_func;
namespace dlm = ::designlab::math_util;
namespace dlsu = ::designlab::string_util;


const int NodeDisplayGui::kWidth = 470;
const int NodeDisplayGui::kHeight = 600;
const int NodeDisplayGui::kClosedHeight = 50;


NodeDisplayGui::NodeDisplayGui(
	const int x_pos,
	const int y_pos,
	const std::shared_ptr<const IHexapodJointCalculator>& calculator_ptr,
	const std::shared_ptr<const IHexapodVaildChecker>& checker_ptr
) :
	kGuiLeftPosX(x_pos),
	kGuiTopPosY(y_pos),
	calculator_ptr_(calculator_ptr),
	checker_ptr_(checker_ptr),
	is_closed_(false),
	display_type_(DisplayMode::kDefualt),
	font_handle_(FontLoader::GetIns()->GetFontHandle(kFontPath))
{
	//ボタンを作成する
	const int kButtonSizeX = 100;
	const int kButtonSizeY = 30;

	buttons_.push_back(
		std::make_unique<SimpleButton>(
			"最大/小化",
			kGuiLeftPosX + kWidth - kButtonSizeX / 2 - 10,
			kGuiTopPosY + 10 + kButtonSizeY / 2,
			kButtonSizeX,
			kButtonSizeY)
	);
	buttons_.back()->SetActivateFunction([this]() { is_closed_ = !is_closed_; });

	buttons_.push_back(std::make_unique<SimpleButton>(
		"切り替え",
		kGuiLeftPosX + kWidth - kButtonSizeX / 2 - 10,
		kGuiTopPosY + kHeight - kButtonSizeY / 2 - 10,
		kButtonSizeX,
		kButtonSizeY)
	);
	buttons_.back()->SetActivateFunction([this]()
		{
			if (display_type_ == DisplayMode::kDefualt)
			{
				display_type_ = DisplayMode::kJointState;
			}
			else
			{
				display_type_ = DisplayMode::kDefualt;
			}
		}
	);
}

void NodeDisplayGui::SetDisplayNode(const RobotStateNode& node)
{
	//ノードをセットする
	display_node_ = node;

	if (!calculator_ptr_) { return; }

	// 関節の角度をセットする
	joint_state_ = calculator_ptr_->CalculateAllJointState(display_node_);
}


void NodeDisplayGui::Update()
{
	//ボタンの更新を行う
	for (auto& button : buttons_)
	{
		button->Update();

		if (button->IsCursorInGui() && Mouse::GetIns()->GetPressingCount(MOUSE_INPUT_LEFT) == 1)
		{
			button->Activate();
		}
	}
}


void NodeDisplayGui::Draw() const
{
	// 枠
	DrawBackground();

	// テキスト
	if (!is_closed_)
	{
		if (display_type_ == DisplayMode::kDefualt)
		{
			DrawNodeInfo();
		}
		else
		{
			DrawJointInfo();
		}
	}


	//ボタンを描画する
	for (auto& button : buttons_)
	{
		button->Draw();
	}
}


void NodeDisplayGui::DrawBackground() const
{
	const unsigned int kBoxColor = GetColor(255, 255, 255);
	const unsigned int kBoxAlpha = 200;

	if (is_closed_)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, kBoxAlpha);
		DrawBox(kGuiLeftPosX, kGuiTopPosY, kGuiLeftPosX + kWidth, kGuiTopPosY + kClosedHeight, kBoxColor, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	else
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, kBoxAlpha);
		DrawBox(kGuiLeftPosX, kGuiTopPosY, kGuiLeftPosX + kWidth, kGuiTopPosY + kHeight, kBoxColor, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

}

void NodeDisplayGui::DrawNodeInfo() const
{
	const unsigned int kTextColor = GetColor(10, 10, 10);
	const unsigned int kBaseTextColor = GetColor(80, 80, 80);
	const int kTextXPos = kGuiLeftPosX + 10;
	const int kTextYMinPos = kGuiTopPosY + 10;
	const int kTextYInterval = kFontSize + 4;
	const std::array<std::string, HexapodConst::kLegNum> leg_name = { "右前","右中","右後","左後","左中","左前" };

	int text_line = 0;

	DrawFormatStringToHandle(kTextXPos, kTextYMinPos + kTextYInterval * (text_line++), kTextColor, font_handle_, "階層");
	DrawFormatStringToHandle(kTextXPos, kTextYMinPos + kTextYInterval * (text_line++), kTextColor, font_handle_,
		" bit : %s", display_node_.leg_state.to_string().c_str()
	);

	DrawFormatStringToHandle(kTextXPos, kTextYMinPos + kTextYInterval * (text_line++), kTextColor, font_handle_,
		" 重心 : %s(%d)", 
		dlsu::MyEnumToString(dllf::GetDiscreteComPos(display_node_.leg_state)).c_str(),
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
			str_leg_pos_right += leg_name[i] + "-" + dlsu::MyEnumToString(pos) + "(" + std::to_string(static_cast<int>(pos)) + "), ";
		}
		else
		{
			str_leg_pos_left += leg_name[i] + "-" + dlsu::MyEnumToString(pos) + "(" + std::to_string(static_cast<int>(pos)) + "), ";
		}
	}
	DrawFormatStringToHandle(kTextXPos, kTextYMinPos + kTextYInterval * (text_line++), kTextColor, font_handle_, " 脚位置 : ", str_leg_pos_right.c_str());
	DrawFormatStringToHandle(kTextXPos, kTextYMinPos + kTextYInterval * (text_line++), kTextColor, font_handle_, "  %s", str_leg_pos_right.c_str());
	DrawFormatStringToHandle(kTextXPos, kTextYMinPos + kTextYInterval * (text_line++), kTextColor, font_handle_, "  %s", str_leg_pos_left.c_str());
	DrawFormatStringToHandle(kTextXPos, kTextYMinPos + kTextYInterval * (text_line++), kTextColor, font_handle_, " 脚の状態：%s", str_ground.c_str());

	// 重心を表示する
	++text_line;
	DrawFormatStringToHandle(kTextXPos, kTextYMinPos + kTextYInterval * (text_line++), kTextColor, font_handle_, "重心位置");

	DrawFormatStringToHandle(kTextXPos, kTextYMinPos + kTextYInterval * (text_line++), kTextColor, font_handle_,
		" %s", display_node_.global_center_of_mass.ToString().c_str());

	// 回転を表示する
	DrawFormatStringToHandle(kTextXPos, kTextYMinPos + kTextYInterval * (text_line++), kTextColor, font_handle_,
		"回転 (w:%5.3f,x:%5.3f,y:%5.3f,z:%5.3f)", display_node_.quat.w, display_node_.quat.v.x, display_node_.quat.v.y, display_node_.quat.v.z);

	// オイラー角にして表示する．
	const dl::EulerXYZ euler = dl::ToEulerXYZ(display_node_.quat);
	DrawFormatStringToHandle(kTextXPos, kTextYMinPos + kTextYInterval * (text_line++), kTextColor, font_handle_,
		"　オイラー角(x:%5.3f[deg],y:%5.3f[deg],z:%5.3f[deg])", 
		dlm::ConvertRadToDeg(euler.x_angle),
		dlm::ConvertRadToDeg(euler.y_angle), 
		dlm::ConvertRadToDeg(euler.z_angle)
	);

	// 脚の位置を表示する
	++text_line;
	DrawFormatStringToHandle(kTextXPos, kTextYMinPos + kTextYInterval * (text_line++), kTextColor, font_handle_, "脚位置");

	for (int i = 0; i < HexapodConst::kLegNum; i++)
	{
		DrawFormatStringToHandle(kTextXPos, kTextYMinPos + kTextYInterval * (text_line++), kTextColor, font_handle_,
			"%s %s", leg_name[i].c_str(), display_node_.leg_pos[i].ToString().c_str());
	}

	// 脚の基準座標を表示する
	for (int i = 0; i < HexapodConst::kLegNum; i++)
	{
		if (display_node_.leg_pos[i] == display_node_.leg_reference_pos[i])
		{
			DrawFormatStringToHandle(kTextXPos, kTextYMinPos + kTextYInterval * (text_line++), kBaseTextColor, font_handle_,
				" %s脚の基準座標は現在の脚位置と同じです．", leg_name[i].c_str());
		}
		else
		{
			DrawFormatStringToHandle(kTextXPos, kTextYMinPos + kTextYInterval * (text_line++), kBaseTextColor, font_handle_,
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
	DrawFormatStringToHandle(kTextXPos, kTextYMinPos + kTextYInterval * (text_line++), kTextColor, font_handle_,
		"深さ：%d, 次の動作 : %s", display_node_.depth, dlsu::MyEnumToString(display_node_.next_move).c_str());

	DrawFormatStringToHandle(kTextXPos, kTextYMinPos + kTextYInterval * (text_line++), kTextColor, font_handle_, "指定がなければ単位は長さが[mm]，角度が[rad]");
}

void NodeDisplayGui::DrawJointInfo() const
{
	const unsigned int kTextColor = GetColor(10, 10, 10);
	const unsigned int kErrorTextColor = GetColor(128, 10, 10);
	const int kTextXPos = kGuiLeftPosX + 10;
	const int kTextYMinPos = kGuiTopPosY + 50;
	const int kTextYInterval = 20;

	if (!calculator_ptr_) 
	{
		DrawFormatStringToHandle(kTextXPos, kTextYMinPos + kTextYInterval * 0, kTextColor, font_handle_, "計算クラスがnullptrです");
		return;
	}
	if (!checker_ptr_) 
	{ 
		DrawFormatStringToHandle(kTextXPos, kTextYMinPos + kTextYInterval * 0, kTextColor, font_handle_, "チェッカークラスがnullptrです");
		return; 
	}

	for (int i = 0; i < HexapodConst::kLegNum; i++)
	{
		if (joint_state_[i].joint_angle.size() != 3) 
		{
			DrawFormatStringToHandle(kTextXPos, kTextYMinPos + kTextYInterval * 0, kTextColor, font_handle_, "間接の計算ができていない，またはされていません．");
			return;
		}
		if (joint_state_[i].joint_pos_leg_coordinate.size() != 4) 
		{
			DrawFormatStringToHandle(kTextXPos, kTextYMinPos + kTextYInterval * 0, kTextColor, font_handle_, "間接の計算ができていない，またはされていません．");
			return; 
		}
	}

	int text_line = 0;
	

	for (int i = 0; i < HexapodConst::kLegNum; i++)
	{
		DrawFormatStringToHandle(kTextXPos, kTextYMinPos + kTextYInterval * (text_line++), kTextColor, font_handle_, "[%d] c %s[deg],f %s[deg],t %s[deg]",
			i,
			dlm::ConvertFloatToString(dlm::ConvertRadToDeg(joint_state_[i].joint_angle[0])).c_str(),
			dlm::ConvertFloatToString(dlm::ConvertRadToDeg(joint_state_[i].joint_angle[1])).c_str(),
			dlm::ConvertFloatToString(dlm::ConvertRadToDeg(joint_state_[i].joint_angle[2])).c_str()
		);

		DrawFormatStringToHandle(kTextXPos, kTextYMinPos + kTextYInterval * (text_line++), kTextColor, font_handle_, "    c %3.3f[mm],f %3.3f[mm],t %3.3f[mm]",
			(joint_state_[i].joint_pos_leg_coordinate[0] - joint_state_[i].joint_pos_leg_coordinate[1]).GetLength(),
			(joint_state_[i].joint_pos_leg_coordinate[1] - joint_state_[i].joint_pos_leg_coordinate[2]).GetLength(),
			(joint_state_[i].joint_pos_leg_coordinate[2] - joint_state_[i].joint_pos_leg_coordinate[3]).GetLength()
		);


		if (checker_ptr_->IsLegInRange(i, joint_state_[i].joint_pos_leg_coordinate[3]))
		{
			DrawFormatStringToHandle(kTextXPos, kTextYMinPos + kTextYInterval * (text_line++), kTextColor, 
				font_handle_, "    近似値された可動域内にあります．");
		}
		else
		{
			DrawFormatStringToHandle(kTextXPos, kTextYMinPos + kTextYInterval * (text_line++), kErrorTextColor, 
				font_handle_, "    近似値された可動域外です．");
		}


		std::string str = "";
		if (joint_state_[i].joint_angle[0] < PhantomXMkIIConst::kCoxaDefaultAngle[i] + PhantomXMkIIConst::kCoxaAngleMin) { str += "coxa_min "; }
		if (joint_state_[i].joint_angle[0] > PhantomXMkIIConst::kCoxaDefaultAngle[i] + PhantomXMkIIConst::kCoxaAngleMax) { str += "coxa_max "; }
		if (joint_state_[i].joint_angle[1] < PhantomXMkIIConst::kFemurAngleMin) { str += "femur_min "; }
		if (joint_state_[i].joint_angle[1] > PhantomXMkIIConst::kFemurAngleMax) { str += "femur_max "; }
		if (joint_state_[i].joint_angle[2] < PhantomXMkIIConst::kTibiaAngleMin) { str += "tibia_min "; }
		if (joint_state_[i].joint_angle[2] > PhantomXMkIIConst::kTibiaAngleMax) { str += "tibia_max "; }
		if (! joint_state_[i].is_in_range) { str += "脚先が届いていません "; }

		if (!str.empty())
		{
			const size_t max_str_size = 30;
			if (str.size() > max_str_size) { str = str.substr(0, max_str_size); }

			DrawFormatStringToHandle(kTextXPos, kTextYMinPos + kTextYInterval * (text_line++), kErrorTextColor, 
				font_handle_, "    実際の可動域の外です． %s", str.c_str());
		}
		else 
		{
			DrawFormatStringToHandle(kTextXPos, kTextYMinPos + kTextYInterval * (text_line++), kTextColor, 
								font_handle_, "    実際の可動域の内です．");
		}
	}
}

void NodeDisplayGui::DrawGlobalPosInfo() const
{
	const unsigned int kTextColor = GetColor(10, 10, 10);
	const int kTextXPos = kGuiLeftPosX + 10;
	const int kTextYMinPos = kGuiTopPosY + 10;
	const int kTextYInterval = kFontSize + 4;
	const std::array<std::string, HexapodConst::kLegNum> leg_name = { "右前","右中","右後","左後","左中","左前" };

	int text_line = 0;

	DrawFormatStringToHandle(kTextXPos, kTextYMinPos + kTextYInterval * (text_line++), kTextColor, font_handle_, "脚先座標(グローバル座標)");

	for (int i = 0; i < HexapodConst::kLegNum; i++)
	{
		dl::Vector3 global_pos = display_node_.leg_pos[i];

		DrawFormatStringToHandle(kTextXPos, kTextYMinPos + kTextYInterval * (text_line++), kTextColor, font_handle_,
						"%s %s", leg_name[i].c_str(), global_pos.ToString().c_str());
	}

}
