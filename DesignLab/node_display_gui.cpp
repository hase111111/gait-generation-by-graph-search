#include "node_display_gui.h"

#include "DxLib.h"

#include "leg_state.h"


const int NodeDisplayGUI::BOX_SIZE_X = 450;
const int NodeDisplayGUI::BOX_SIZE_Y = 550;
const int NodeDisplayGUI::BOX_SIZE_Y_CLOSED = 50;



NodeDisplayGUI::NodeDisplayGUI(const int x_pos, const int y_pos, std::shared_ptr<AbstractHexapodStateCalculator> calc) :
	kGUILeftPosX(x_pos), kGUITopPosY(y_pos), mp_calculator(calc)
{
	//ボタンを作成する
	const int kButtonSizeX = 100;
	const int kButtonSizeY = 30;

	m_buttons[EButtonType::OPEN_CLOSE] = std::make_unique<ButtomController>(kGUILeftPosX + BOX_SIZE_X - kButtonSizeX / 2 - 10, kGUITopPosY + 10 + kButtonSizeY / 2,
		kButtonSizeX, kButtonSizeY, "最大/小化");
	m_buttons[EButtonType::SWITCHING] = std::make_unique<ButtomController>(kGUILeftPosX + BOX_SIZE_X - kButtonSizeX / 2 - 10, kGUITopPosY + BOX_SIZE_Y - kButtonSizeY / 2 - 10,
		kButtonSizeX, kButtonSizeY, "切り替え");

	for (int i = 0; i < HexapodConst::LEG_NUM; i++)
	{
		m_joint_state[i].global_joint_position.resize(4);
		m_joint_state[i].local_joint_position.resize(4);
		m_joint_state[i].joint_angle.resize(3);
	}
}


void NodeDisplayGUI::setDisplayNode(const SNode& node)
{
	//ノードをセットする
	m_node = node;

	if (!mp_calculator) { return; }

	// 関節の角度をセットする
	mp_calculator->calculateAllJointState(m_node, m_joint_state);
}


void NodeDisplayGUI::update()
{
	//ボタンの更新を行う
	for (auto& button : m_buttons)
	{
		button.second->update();

		if (button.first == EButtonType::OPEN_CLOSE && button.second->isPushedNow())
		{
			m_is_closed = !m_is_closed;
		}
		else if (button.first == EButtonType::SWITCHING && !m_is_closed && button.second->isPushedNow())
		{
			if (m_display_type == EDisplayType::DEFUALT)
			{
				m_display_type = EDisplayType::LEG_STATE;
			}
			else
			{
				m_display_type = EDisplayType::DEFUALT;
			}
		}
	}
}


void NodeDisplayGUI::draw() const
{
	// 枠
	drawBackground();

	// テキスト
	if (!m_is_closed)
	{
		if (m_display_type == EDisplayType::DEFUALT)
		{
			drawNodeInfo();
		}
		else
		{
			drawJointInfo();
		}
	}


	//ボタンを描画する
	for (auto& button : m_buttons)
	{
		if (!(button.first == EButtonType::SWITCHING && m_is_closed))
		{
			button.second->draw();
		}
	}
}


void NodeDisplayGUI::drawBackground() const
{
	const unsigned int kBoxColor = GetColor(255, 255, 255);
	const unsigned int kBoxAlpha = 200;

	if (m_is_closed)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, kBoxAlpha);
		DrawBox(kGUILeftPosX, kGUITopPosY, kGUILeftPosX + BOX_SIZE_X, kGUITopPosY + BOX_SIZE_Y_CLOSED, kBoxColor, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	else
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, kBoxAlpha);
		DrawBox(kGUILeftPosX, kGUITopPosY, kGUILeftPosX + BOX_SIZE_X, kGUITopPosY + BOX_SIZE_Y, kBoxColor, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

}

void NodeDisplayGUI::drawNodeInfo() const
{
	const unsigned int kTextColor = GetColor(10, 10, 10);
	const unsigned int kBaseTextColor = GetColor(80, 80, 80);
	const int kTextXPos = kGUILeftPosX + 10;
	const int kTextYMinPos = kGUITopPosY + 10;
	const int kTextYInterval = 30;

	int text_line = 0;

	DrawFormatString(kTextXPos, kTextYMinPos + kTextYInterval * (text_line++), kTextColor, "重心：%d，脚位置：%d,%d,%d,%d,%d,%d", dl_leg::getComPatternState(m_node.leg_state),
		dl_leg::getLegState(m_node.leg_state, 0), dl_leg::getLegState(m_node.leg_state, 1), dl_leg::getLegState(m_node.leg_state, 2),
		dl_leg::getLegState(m_node.leg_state, 3), dl_leg::getLegState(m_node.leg_state, 4), dl_leg::getLegState(m_node.leg_state, 5));

	// 重心を表示する
	DrawFormatString(kTextXPos, kTextYMinPos + kTextYInterval * (text_line++), kTextColor,
		"重心位置(x:%5.3f,y:%5.3f,z:%5.3f)", m_node.global_center_of_mass.x, m_node.global_center_of_mass.y, m_node.global_center_of_mass.z);

	// 回転を表示する
	DrawFormatString(kTextXPos, kTextYMinPos + kTextYInterval * (text_line++), kTextColor,
		"回転(roll:%5.3f,pitch:%5.3f,yaw:%5.3f)", m_node.rot.roll, m_node.rot.pitch, m_node.rot.yaw);

	//遊脚か接地脚か
	std::string str = "";
	for (int i = 0; i < HexapodConst::LEG_NUM; i++)
	{
		if (dl_leg::isGrounded(m_node.leg_state, i)) { str += "接地,"; }
		else { str += "遊脚,"; }
	}
	DrawFormatString(kTextXPos, kTextYMinPos + kTextYInterval * (text_line++), kTextColor, "脚の状態：%s", str.c_str());

	// 脚の位置を表示する
	for (int i = 0; i < HexapodConst::LEG_NUM; i++)
	{
		DrawFormatString(kTextXPos, kTextYMinPos + kTextYInterval * (text_line++), kTextColor,
			"%d番脚の位置(x:%5.3f,y:%5.3f,z:%5.3f)", i, m_node.leg_pos[i].x, m_node.leg_pos[i].y, m_node.leg_pos[i].z);
	}

	// 脚の基準座標を表示する
	for (int i = 0; i < HexapodConst::LEG_NUM; i++)
	{
		DrawFormatString(kTextXPos, kTextYMinPos + kTextYInterval * (text_line++), kBaseTextColor,
			" %d番脚の基準座標(x:%5.3f,y:%5.3f,z:%5.3f)", i, m_node.leg_base_pos[i].x, m_node.leg_base_pos[i].y, m_node.leg_base_pos[i].z);
	}

	// 深さと次の動作を表示する
	DrawFormatString(kTextXPos, kTextYMinPos + kTextYInterval * (text_line++), kTextColor,
		"深さ：%d, 次の動作 : %s", m_node.depth, std::to_string(m_node.next_move).c_str());

	DrawFormatString(kTextXPos, kTextYMinPos + kTextYInterval * (text_line++), kTextColor, "単位は長さが[mm]，角度が[rad]");
}


void NodeDisplayGUI::drawJointInfo() const
{
	if (!mp_calculator) { return; }


	const unsigned int kTextColor = GetColor(10, 10, 10);
	const unsigned int kErrorTextColor = GetColor(128, 10, 10);
	const int kTextXPos = kGUILeftPosX + 10;
	const int kTextYMinPos = kGUITopPosY + 50;
	const int kTextYInterval = 30;


	int text_line = 0;


	for (int i = 0; i < HexapodConst::LEG_NUM; i++)
	{
		DrawFormatString(kTextXPos, kTextYMinPos + kTextYInterval * (text_line++), kTextColor, "[%d] c %3.3f[deg],f %3.3f[deg],t %3.3f[deg]", i,
			dl_math::convertRadToDeg(m_joint_state[i].joint_angle[0]), dl_math::convertRadToDeg(m_joint_state[i].joint_angle[1]), dl_math::convertRadToDeg(m_joint_state[i].joint_angle[2]));

		DrawFormatString(kTextXPos, kTextYMinPos + kTextYInterval * (text_line++), kTextColor, "    c %3.3f[mm],f %3.3f[mm],t %3.3f[mm]",
			(m_joint_state[i].local_joint_position[0] - m_joint_state[i].local_joint_position[1]).length(),
			(m_joint_state[i].local_joint_position[1] - m_joint_state[i].local_joint_position[2]).length(),
			(m_joint_state[i].local_joint_position[2] - m_joint_state[i].local_joint_position[3]).length()
		);


		if (mp_calculator->isLegInRange(i, m_joint_state[i].local_joint_position[3]))
		{
			DrawFormatString(kTextXPos, kTextYMinPos + kTextYInterval * (text_line++), kTextColor, "    近似値 true");
		}
		else
		{
			DrawFormatString(kTextXPos, kTextYMinPos + kTextYInterval * (text_line++), kErrorTextColor, "    近似値 false");
		}


		std::string str = "";
		if (m_joint_state[i].joint_angle[0] < HexapodConst::PHANTOMX_COXA_DEFAULT_ANGLE[i] + HexapodConst::PHANTOMX_COXA_ANGLE_MIN) { str += "coxa_min "; }
		if (m_joint_state[i].joint_angle[0] > HexapodConst::PHANTOMX_COXA_DEFAULT_ANGLE[i] + HexapodConst::PHANTOMX_COXA_ANGLE_MAX) { str += "coxa_max "; }
		if (m_joint_state[i].joint_angle[1] < HexapodConst::PHANTOMX_FEMUR_ANGLE_MIN) { str += "femur_min "; }
		if (m_joint_state[i].joint_angle[1] > HexapodConst::PHANTOMX_FEMUR_ANGLE_MAX) { str += "femur_max "; }
		if (m_joint_state[i].joint_angle[2] < HexapodConst::PHANTOMX_TIBIA_ANGLE_MIN) { str += "tibia_min "; }
		if (m_joint_state[i].joint_angle[2] > HexapodConst::PHANTOMX_TIBIA_ANGLE_MAX) { str += "tibia_max "; }

		if (!str.empty())
		{
			DrawFormatString(kTextXPos, kTextYMinPos + kTextYInterval * (text_line++), kErrorTextColor, "error %s", str.c_str());
		}
	}
}
