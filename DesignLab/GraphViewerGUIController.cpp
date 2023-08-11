#include "GraphViewerGUIController.h"

#include <string>

#include "DxLib.h"

#include "Dxlib3DFunction.h"
#include "graphic_const.h"
#include "Define.h"
#include "keyboard.h"
#include "LegState.h"


GraphViewerGUIController::GraphViewerGUIController(const std::vector<SNode>* const p_graph, size_t* const p_display_node_index)
	: mp_graph(p_graph)
	, mp_display_node_index(p_display_node_index)
{
}


void GraphViewerGUIController::update()
{
	inputNumber();
	updateChildrenList();
	changeDisplayNodeIndex();
}


void GraphViewerGUIController::draw() const
{
	drawGraphData();
	drawNodeControllPanel();

	if (mp_graph->size() != 0 && *mp_display_node_index < mp_graph->size())
	{
		drawNodeData(mp_graph->at(*mp_display_node_index));
	}
}


void GraphViewerGUIController::drawGraphData() const
{
	const int _box_size_x = 250;
	const int _box_size_y = 200;
	const int _box_min_x = GraphicConst::WIN_X - _box_size_x - 50;
	const int _box_min_y = GraphicConst::WIN_Y - _box_size_y - 50;
	const unsigned int _color = GetColor(255, 255, 255);

	// 枠
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawBox(_box_min_x, _box_min_y, _box_min_x + _box_size_x, _box_min_y + _box_size_y, _color, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	const unsigned int _text_color = GetColor(10, 10, 10);

	// テキスト
	if (mp_graph->size() == 0)
	{
		DrawString(_box_min_x + 10, _box_min_y + 10, "ノード数 : 0", _text_color);
		DrawString(_box_min_x + 10, _box_min_y + 30, "グラフを生成してください", _text_color);
	}
	else
	{
		DrawFormatString(_box_min_x + 10, _box_min_y + 10, _text_color, "総ノード数:%d", mp_graph->size());
		DrawFormatString(_box_min_x + 10, _box_min_y + 30, _text_color, "表示ノード:%d番", *mp_display_node_index);

		//深さごとのノードの数
		for (size_t i = 0; i < m_graph_node_depth_data.size(); i++)
		{
			DrawFormatString(_box_min_x + 10, _box_min_y + 50 + 20 * (int)i, _text_color, "　(深さ%dのノード:%d)", (int)i, (int)m_graph_node_depth_data.at(i));
		}
	}
}


void GraphViewerGUIController::drawNodeControllPanel() const
{
	const int _box_size_x = 350;
	const int _box_size_y = 340;
	const int _box_min_x = 50;
	const int _box_min_y = 50;
	const unsigned int _color = GetColor(255, 255, 255);

	// 枠
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawBox(_box_min_x, _box_min_y, _box_min_x + _box_size_x, _box_min_y + _box_size_y, _color, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	const unsigned int _text_color = GetColor(10, 10, 10);

	// テキスト
	DrawFormatString(_box_min_x + 10, _box_min_y + 10, _text_color, "input ( C でクリア)");
	if (m_input_number < 0) DrawFormatString(_box_min_x + 10, _box_min_y + 30, _text_color, "　数字を入力してください");
	else DrawFormatString(_box_min_x + 10, _box_min_y + 30, _text_color, "　%d", m_input_number);

	if (mp_graph->size() > *mp_display_node_index)
	{
		DrawFormatString(_box_min_x + 10, _box_min_y + 60, _text_color, "%d番ノードの親ノード:%d番", *mp_display_node_index, mp_graph->at(*mp_display_node_index).parent_num);
		DrawFormatString(_box_min_x + 10, _box_min_y + 90, _text_color, "%d番ノードの子ノード数:%d個", m_childen_list.first, m_childen_list.second.size());

		std::string _str = m_childen_list.second.size() == 0 ? "None" : "　";
		for (size_t i = 0; i < m_childen_list.second.size(); i++)
		{
			if (i > (size_t)6 * 5 - 1) { _str += "..."; break; }
			_str += std::to_string(m_childen_list.second.at(i)) + ",";
			if (i % 6 == 0 && i != 0) { _str += "\n　"; }
		}

		DrawFormatString(_box_min_x + 10, _box_min_y + 110, _text_color, "%d番ノードの子ノードリスト", m_childen_list.first);
		DrawFormatString(_box_min_x + 10, _box_min_y + 130, _text_color, _str.c_str());
		DrawFormatString(_box_min_x + 10, _box_min_y + 230, _text_color, "　(子ノードリストの更新は U )");
		DrawFormatString(_box_min_x + 10, _box_min_y + 250, _text_color, "　(上下キーでノード移動)");
		DrawFormatString(_box_min_x + 10, _box_min_y + 270, _text_color, "　(左右キーで表示する子ノード切り替え)");
		DrawFormatString(_box_min_x + 10, _box_min_y + 290, _text_color, "　(Zキーでカメラ表示を切り替え)");
	}
}


void GraphViewerGUIController::drawNodeData(const SNode node) const
{
	const int kBoxSizeX = 400;
	const int KBoxSizeY = 300;
	const int kBoxMinX = GraphicConst::WIN_X - 25 - kBoxSizeX;
	const int kBoxMinY = 25;
	const unsigned int kBoxColor = GetColor(255, 255, 255);
	const unsigned int kBoxAlpha = 128;

	// 枠
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, kBoxAlpha);
	DrawBox(kBoxMinX, kBoxMinY, kBoxMinX + kBoxSizeX, kBoxMinY + KBoxSizeY, kBoxColor, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// テキスト
	const unsigned int kTextColor = GetColor(10, 10, 10);
	const int kTextXPos = kBoxMinX + 10;
	const int kTextYMinPos = kBoxMinY + 10;
	const int kTextYInterval = 30;
	int text_line = 0;

	DrawFormatString(kTextXPos, kTextYMinPos + kTextYInterval * (text_line++), kTextColor, "重心：%d，脚位置：%d,%d,%d,%d,%d,%d", LegStateEdit::getComPatternState(node.leg_state),
		LegStateEdit::getLegState(node.leg_state, 0), LegStateEdit::getLegState(node.leg_state, 1), LegStateEdit::getLegState(node.leg_state, 2),
		LegStateEdit::getLegState(node.leg_state, 3), LegStateEdit::getLegState(node.leg_state, 4), LegStateEdit::getLegState(node.leg_state, 5));

	// 重心を表示する
	DrawFormatString(kTextXPos, kTextYMinPos + kTextYInterval * (text_line++), kTextColor,
		"重心位置(x:%5.3f,y:%5.3f,z:%5.3f)", node.global_center_of_mass.x, node.global_center_of_mass.y, node.global_center_of_mass.z);

	//遊脚か接地脚か
	std::string str = "";
	for (int i = 0; i < HexapodConst::LEG_NUM; i++)
	{
		if (LegStateEdit::isGrounded(node.leg_state, i)) { str += "接地,"; }
		else { str += "遊脚,"; }
	}
	DrawFormatString(kTextXPos, kTextYMinPos + kTextYInterval * (text_line++), kTextColor, "脚の状態：%s", str.c_str());

	// 脚の位置を表示する
	for (int i = 0; i < HexapodConst::LEG_NUM; i++)
	{
		DrawFormatString(kTextXPos, kTextYMinPos + kTextYInterval * (text_line++), kTextColor,
			"%d番脚の位置(x:%5.3f,y:%5.3f,z:%5.3f)", i, node.leg_pos[i].x, node.leg_pos[i].y, node.leg_pos[i].z);
	}

	// 深さと次の動作を表示する
	DrawFormatString(kTextXPos, kTextYMinPos + kTextYInterval * (text_line++), kTextColor,
		"深さ：%d, 次の動作 : %s", static_cast<int>(node.depth), std::to_string(node.next_move).c_str());
}


void GraphViewerGUIController::inputNumber()
{
	// Cキーでリセット
	if (Keyboard::getIns()->getPressingCount(KEY_INPUT_C) == 1)
	{
		m_input_number = -1;
		return;
	}

	// 数字入力
	int _temp = -1;

	if (Keyboard::getIns()->getPressingCount(KEY_INPUT_0) == 1) { _temp = 0; }
	else if (Keyboard::getIns()->getPressingCount(KEY_INPUT_1) == 1) { _temp = 1; }
	else if (Keyboard::getIns()->getPressingCount(KEY_INPUT_2) == 1) { _temp = 2; }
	else if (Keyboard::getIns()->getPressingCount(KEY_INPUT_3) == 1) { _temp = 3; }
	else if (Keyboard::getIns()->getPressingCount(KEY_INPUT_4) == 1) { _temp = 4; }
	else if (Keyboard::getIns()->getPressingCount(KEY_INPUT_5) == 1) { _temp = 5; }
	else if (Keyboard::getIns()->getPressingCount(KEY_INPUT_6) == 1) { _temp = 6; }
	else if (Keyboard::getIns()->getPressingCount(KEY_INPUT_7) == 1) { _temp = 7; }
	else if (Keyboard::getIns()->getPressingCount(KEY_INPUT_8) == 1) { _temp = 8; }
	else if (Keyboard::getIns()->getPressingCount(KEY_INPUT_9) == 1) { _temp = 9; }

	if (_temp >= 0)
	{
		if (m_input_number < 0)
		{
			m_input_number = _temp;
		}
		else
		{
			m_input_number *= 10;
			m_input_number += _temp;
		}
	}
}


void GraphViewerGUIController::changeDisplayNodeIndex()
{
	if (mp_graph->size() == 0) return;

	if (Keyboard::getIns()->getPressingCount(KEY_INPUT_DOWN) == 1)
	{
		(*mp_display_node_index)--;
	}
	else if (Keyboard::getIns()->getPressingCount(KEY_INPUT_UP) == 1)
	{
		(*mp_display_node_index)++;
	}
	else if (Keyboard::getIns()->getPressingCount(KEY_INPUT_RETURN) == 1)
	{
		(*mp_display_node_index) = m_input_number;
		m_input_number = -1;
	}
	else if (Keyboard::getIns()->getPressingCount(KEY_INPUT_P) == 1)
	{
		if (mp_graph->size() > *mp_display_node_index)
		{
			(*mp_display_node_index) = mp_graph->at(*mp_display_node_index).parent_num;
		}
	}
	else if (Keyboard::getIns()->getPressingCount(KEY_INPUT_LEFT) == 1 && m_childen_list.second.empty() == false)
	{
		m_display_children_list_index--;

		if (m_display_children_list_index < 0) m_display_children_list_index = (int)m_childen_list.second.size() - 1;
		m_display_children_list_index = m_display_children_list_index < 0 ? 0 : m_display_children_list_index;

		(*mp_display_node_index) = m_childen_list.second.at(m_display_children_list_index);
	}
	else if (Keyboard::getIns()->getPressingCount(KEY_INPUT_RIGHT) == 1 && m_childen_list.second.empty() == false)
	{
		m_display_children_list_index++;

		if (m_display_children_list_index >= m_childen_list.second.size()) m_display_children_list_index = 0;
		(*mp_display_node_index) = m_childen_list.second.at(m_display_children_list_index);
	}


	if (*mp_display_node_index < 0)*mp_display_node_index = mp_graph->size() - 1;
	else if (*mp_display_node_index >= mp_graph->size()) *mp_display_node_index = 0;
}


void GraphViewerGUIController::updateChildrenList()
{
	if (mp_graph->size() == 0) return;

	if (Keyboard::getIns()->getPressingCount(KEY_INPUT_U) == 1)
	{
		m_childen_list.first = (int)(*mp_display_node_index);
		m_childen_list.second.clear();

		const size_t _size = mp_graph->size();

		for (size_t i = 0; i < _size; i++)
		{
			if (mp_graph->at(i).parent_num == m_childen_list.first)
			{
				m_childen_list.second.push_back((int)i);
			}
		}
	}
}


void GraphViewerGUIController::updateGraphNodeDepthData()
{
	m_graph_node_depth_data.clear();

	if (mp_graph->size() > 0)
	{
		m_graph_node_depth_data.resize((size_t)Define::GRAPH_SEARCH_DEPTH + 1);

		for (size_t i = 0; i < mp_graph->size(); ++i)
		{
			m_graph_node_depth_data.at(static_cast<size_t>(mp_graph->at(i).depth))++;
		}
	}
}
