#include "node_display_gui.h"

#include "DxLib.h"

#include "leg_state.h"


const int NodeDisplayGUI::BOX_SIZE_X = 450;
const int NodeDisplayGUI::BOX_SIZE_Y = 550;


NodeDisplayGUI::NodeDisplayGUI(const int x_pos, const int y_pos) : kGUILeftPosX(x_pos), kGUITopPosY(y_pos)
{
}


void NodeDisplayGUI::setDisplayNode(const SNode& node)
{
	//ノードをセットする
	m_node = node;

	//! @todo 関節の角度を計算する
}


void NodeDisplayGUI::draw() const
{
	const unsigned int kBoxColor = GetColor(255, 255, 255);
	const unsigned int kBoxAlpha = 200;

	// 枠
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, kBoxAlpha);
	DrawBox(kGUILeftPosX, kGUITopPosY, kGUILeftPosX + BOX_SIZE_X, kGUITopPosY + BOX_SIZE_Y, kBoxColor, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// テキスト
	const unsigned int kTextColor = GetColor(10, 10, 10);
	const unsigned int kBaseTextColor = GetColor(50, 50, 50);
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
		"深さ：%d, 次の動作 : %s", static_cast<int>(m_node.depth), std::to_string(m_node.next_move).c_str());

	DrawFormatString(kTextXPos, kTextYMinPos + kTextYInterval * (text_line++), kTextColor, "単位は長さが[mm]，角度が[rad]");
}
