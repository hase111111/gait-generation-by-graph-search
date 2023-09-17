#include "display_node_switch_gui.h"

#include <algorithm>

#include "DxLib.h"



DisplayNodeSwitchGUI::DisplayNodeSwitchGUI(const int x, const int y) : kGUILeftPosX(x), kGUITopPosY(y),
m_display_node_num(0), m_all_node_num(0), m_simulation_num(0),
m_counter(0), m_do_auto_animation(false), m_animation_speed(kAnimeSpeedMin)
{
	const int kButtonDif = 10;
	const int kButtonWidth = 40;
	const int kButtonLeftX = kButtonWidth / 2 + kGUILeftPosX + 10;
	const int kButtonTopY = kButtonWidth / 2 + kGUITopPosY + 100;


	//各種ボタンを作成する
	m_button[EButtonType::MostPrevNode] = std::make_unique<ButtomController>(kButtonLeftX, kButtonTopY, kButtonWidth, kButtonWidth, "<<");
	m_button[EButtonType::PrevNode] = std::make_unique<ButtomController>(kButtonLeftX + (kButtonDif + kButtonWidth) * 1, kButtonTopY, kButtonWidth, kButtonWidth, "<");
	m_button[EButtonType::PlayStop] = std::make_unique<ButtomController>(kButtonLeftX + (kButtonDif + kButtonWidth) * 2, kButtonTopY, kButtonWidth, kButtonWidth, "再生\n停止");
	m_button[EButtonType::NextNode] = std::make_unique<ButtomController>(kButtonLeftX + (kButtonDif + kButtonWidth) * 3, kButtonTopY, kButtonWidth, kButtonWidth, ">");
	m_button[EButtonType::MostNextNode] = std::make_unique<ButtomController>(kButtonLeftX + (kButtonDif + kButtonWidth) * 4, kButtonTopY, kButtonWidth, kButtonWidth, ">>");
	m_button[EButtonType::SpeedDown] = std::make_unique<ButtomController>(kButtonLeftX + (kButtonDif + kButtonWidth) * 3, kButtonTopY + (kButtonDif + kButtonWidth), kButtonWidth, kButtonWidth, "↓");
	m_button[EButtonType::SpeedUp] = std::make_unique<ButtomController>(kButtonLeftX + (kButtonDif + kButtonWidth) * 4, kButtonTopY + (kButtonDif + kButtonWidth), kButtonWidth, kButtonWidth, "↑");

	m_button[EButtonType::PrevSimu] = std::make_unique<ButtomController>(kButtonLeftX + (kButtonDif + kButtonWidth) * 1, kButtonTopY + (kButtonDif + kButtonWidth) * 2,
		kButtonWidth * 2, kButtonWidth, "Prev Simu");
	m_button[EButtonType::NextSimu] = std::make_unique<ButtomController>(kButtonLeftX + (kButtonDif + kButtonWidth) * 3, kButtonTopY + (kButtonDif + kButtonWidth) * 2,
		kButtonWidth * 2, kButtonWidth, "Next Simu");
}


DisplayNodeSwitchGUI::DisplayNodeSwitchGUI() : DisplayNodeSwitchGUI::DisplayNodeSwitchGUI(0, 0)
{
}


void DisplayNodeSwitchGUI::setGraphData(const size_t node_num, const std::vector<size_t>& simu_end_index)
{
	m_all_node_num = node_num;


	m_simu_end_index.clear();

	for (const auto& i : simu_end_index)
	{
		m_simu_end_index.push_back(i);
	}
}


size_t DisplayNodeSwitchGUI::getDisplayNodeNum() const
{
	// 範囲外の値を返さないようにする．
	if (m_display_node_num > m_all_node_num && m_all_node_num != 0) { return m_all_node_num - 1; }


	// 範囲内の値ならば，そのまま返す
	return m_display_node_num;
}


int DisplayNodeSwitchGUI::getSimulationNum() const
{
	return m_simulation_num;
}


void DisplayNodeSwitchGUI::Update()
{
	++m_counter;


	// 自動再生を行う
	if (m_do_auto_animation && m_counter % (180 / m_animation_speed) == 0)
	{
		moveNextNode();
	}


	// ボタンを更新する
	for (auto& i : m_button)
	{
		i.second->Update();

		if (i.second->isPushedNow())
		{
			switch (i.first)
			{
			case EButtonType::MostPrevNode:
				moveMostPrevNode();
				break;

			case EButtonType::MostNextNode:
				moveMostNextNode();
				break;

			case EButtonType::PrevNode:
				movePrevNode();
				break;

			case EButtonType::NextNode:
				moveNextNode();
				break;

			case EButtonType::PrevSimu:
				movePrevSimulation();
				break;

			case EButtonType::NextSimu:
				moveNextSimulation();
				break;

			case EButtonType::PlayStop:
				m_do_auto_animation = !m_do_auto_animation;
				break;
			}
		}

		if (i.second->isPushedNow())
		{
			if (i.first == EButtonType::SpeedDown)
			{
				if (m_animation_speed > kAnimeSpeedMin)
				{
					--m_animation_speed;
				}
			}
			else if (i.first == EButtonType::SpeedUp)
			{
				if (m_animation_speed < kAnimeSpeedMax)
				{
					++m_animation_speed;
				}
			}
		}

	}
}


void DisplayNodeSwitchGUI::Draw() const
{
	const unsigned int kAlpha = 200;

	const unsigned int kColor = GetColor(255, 255, 255);

	// ボックスを描画する
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, kAlpha);

	DrawBox(kGUILeftPosX, kGUITopPosY, kGUILeftPosX + GUI_WIDTH, kGUITopPosY + GUI_HEIGHT, kColor, TRUE);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);


	// ボタンを描画する
	for (const auto& i : m_button)
	{
		i.second->Draw();
	}


	//文字を描画する

	const int kTextLeftX = kGUILeftPosX + 10;

	const unsigned int kTextColor = GetColor(0, 0, 0);


	DrawFormatString(kTextLeftX, kGUITopPosY + 10, kTextColor, "[シミュレーション%d回目(全%d回)]", m_simulation_num + 1, getAllSimulationNum(), m_display_node_num, m_all_node_num);

	int start_node_num = 0;
	int end_node_num = 0;

	if (m_simulation_num == 0)
	{
		start_node_num = 0;
		if (!m_simu_end_index.empty()) { end_node_num = (int)m_simu_end_index[0]; }
		else { end_node_num = (int)m_all_node_num - 1; }
	}
	else if (m_simulation_num == (int)m_simu_end_index.size())
	{
		start_node_num = (int)m_simu_end_index[m_simulation_num - 1] + 1;
		end_node_num = (int)m_all_node_num - 1;
	}
	else
	{
		start_node_num = (int)m_simu_end_index[m_simulation_num - 1] + 1;
		end_node_num = (int)m_simu_end_index[m_simulation_num];
	}

	DrawFormatString(kTextLeftX, kGUITopPosY + 30, kTextColor, "表示ノード : %d (%d～%d)", m_display_node_num, start_node_num, end_node_num, m_all_node_num - 1);

	DrawFormatString(kTextLeftX, kGUITopPosY + 50, kTextColor, "全ノード : %d ", m_all_node_num - 1);

	DrawFormatString(kTextLeftX, kGUITopPosY + 70, kTextColor, m_do_auto_animation == true ? "自動再生 : 再生/速度%d" : "自動再生 : 停止", m_animation_speed);

	DrawFormatString(kTextLeftX, kGUITopPosY + 150, kTextColor, "アニメーションの\n     速度変更");
}


void DisplayNodeSwitchGUI::moveMostPrevNode()
{
	//候補
	size_t candidate = 0;

	for (size_t i = 0; i < m_simu_end_index.size(); i++)
	{
		if (m_simu_end_index[i] < m_display_node_num)
		{
			candidate = m_simu_end_index[i] + 1;
		}
		else
		{
			break;
		}
	}

	m_display_node_num = candidate;
}


void DisplayNodeSwitchGUI::movePrevNode()
{
	for (size_t i = 0; i < m_simu_end_index.size(); i++)
	{
		if (m_simu_end_index[i] + 1 == m_display_node_num)
		{
			return;
		}
	}

	--m_display_node_num;

	m_display_node_num = static_cast<size_t>((std::max)(static_cast<int>(m_display_node_num), 0));
}


void DisplayNodeSwitchGUI::moveMostNextNode()
{
	//候補
	size_t candidate = m_all_node_num - 1;

	for (size_t i = 0; i < m_simu_end_index.size(); i++)
	{
		if (m_simu_end_index[i] >= m_display_node_num)
		{
			candidate = m_simu_end_index[i];
			break;
		}
	}

	m_display_node_num = candidate;
}


void DisplayNodeSwitchGUI::moveNextNode()
{
	for (size_t i = 0; i < m_simu_end_index.size(); i++)
	{
		if (m_simu_end_index[i] == m_display_node_num)
		{
			return;
		}
	}

	++m_display_node_num;

	m_display_node_num = static_cast<size_t>((std::min)(static_cast<int>(m_display_node_num), static_cast<int>(m_all_node_num - 1)));
}


void DisplayNodeSwitchGUI::movePrevSimulation()
{
	//前のシミュレーションへ移動する
	--m_simulation_num;

	m_simulation_num = static_cast<size_t>((std::max)(static_cast<int>(m_simulation_num), 0));

	//ノードをそのシミュレーションの最初のノードに移動する
	if (m_simulation_num == 0)
	{
		m_display_node_num = 0;
	}
	else
	{
		m_display_node_num = m_simu_end_index[m_simulation_num - 1] + 1;
	}
}


void DisplayNodeSwitchGUI::moveNextSimulation()
{
	//次のシミュレーションへ移動する
	++m_simulation_num;


	m_simulation_num = static_cast<size_t>((std::min)(static_cast<int>(m_simulation_num), getAllSimulationNum() - 1));


	//ノードをそのシミュレーションの最初のノードに移動する
	if (m_simulation_num == 0)
	{
		m_display_node_num = 0;
	}
	else
	{
		m_display_node_num = m_simu_end_index[m_simulation_num - 1] + 1;
	}
}


int DisplayNodeSwitchGUI::getAllSimulationNum() const
{
	int all_simu_num = 1;

	if (!m_simu_end_index.empty())
	{
		if (m_simu_end_index.back() == m_all_node_num - 1) { all_simu_num = (int)m_simu_end_index.size(); }
		else { all_simu_num = (int)m_simu_end_index.size() + 1; }
	}

	return all_simu_num;
}
