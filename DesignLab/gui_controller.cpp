#include "gui_controller.h"

#include <string>

#include "DxLib.h"

#include "leg_state.h"
#include "keyboard.h"


GUIController::GUIController()
{
	const int RIGHTX = GraphicConst::WIN_X - (CENTER_X + BOX_X / 2);
	const int RIGHTY = CENTER_Y - BOX_Y / 2;

	const int kCameraButtomX = 100;
	const int kCameraButtomY = 50;
	const int kCameraY = RIGHTY + 150;

	m_buttom.push_back(std::make_unique<ButtomController>(RIGHTX + kCameraButtomX * 6 / 4, kCameraY, kCameraButtomX, kCameraButtomY, "真上カメラ"));
	m_buttom.push_back(std::make_unique<ButtomController>(RIGHTX + kCameraButtomX * 9 / 4, kCameraY + kCameraButtomY * 5 / 4, kCameraButtomX, kCameraButtomY, "俯瞰カメラ"));
	m_buttom.push_back(std::make_unique<ButtomController>(RIGHTX + kCameraButtomX * 9 / 4, kCameraY + kCameraButtomY * 10 / 4, kCameraButtomX, kCameraButtomY, "真横カメラ"));
	m_buttom.push_back(std::make_unique<ButtomController>(RIGHTX + kCameraButtomX * 3 / 4, kCameraY + kCameraButtomY * 5 / 4, kCameraButtomX, kCameraButtomY, "俯瞰(反転)"));
	m_buttom.push_back(std::make_unique<ButtomController>(RIGHTX + kCameraButtomX * 3 / 4, kCameraY + kCameraButtomY * 10 / 4, kCameraButtomX, kCameraButtomY, "真横(反転)"));

	m_buttom.push_back(std::make_unique<ButtomController>(RIGHTX + kCameraButtomX * 6 / 4, kCameraY + kCameraButtomY * 20 / 4, kCameraButtomX * 2, kCameraButtomY, "ノード表示切り替え"));
	m_buttom.push_back(std::make_unique<ButtomController>(RIGHTX + kCameraButtomX * 3 / 4, kCameraY + kCameraButtomY * 25 / 4, kCameraButtomX, kCameraButtomY, "←"));
	m_buttom.push_back(std::make_unique<ButtomController>(RIGHTX + kCameraButtomX * 9 / 4, kCameraY + kCameraButtomY * 25 / 4, kCameraButtomX, kCameraButtomY, "→"));
}


void GUIController::update(CameraController& camera_controller, const int max_node, int& display_node, const int counter)
{
	if (m_mode == ENodeDisplayNode::AUTO_UPDATE)
	{
		if (counter % CHANGE_NEXT_NODE == 0)
		{
			display_node++;
		}
	}
	else if (m_mode == ENodeDisplayNode::ALWAYS_NEW)
	{
		display_node = max_node - 1;
	}


	//表示切替キーが押されたならば，表示切り返えフラグの値をさかさまにする．
	if (Keyboard::getIns()->getPressingCount(KEY_INPUT_Z) == 1) { m_is_displayed = !m_is_displayed; }

	if (m_is_displayed)
	{
		//表示していない時にボタンの処理を行わない

		//ボタンを更新する．
		for (int i = 0; i < (int)m_buttom.size(); i++)
		{
			m_buttom.at(i)->update();

			//ボタンの追加された順番ごとに処理をする．クソみたいな実装でスマソ
			switch (i)
			{
			case 0:
				if (m_buttom.at(i)->isPushedNow()) { camera_controller.setCameraMode(ECameraMode::TOP_VIEW); }
				break;

			case 1:
				if (m_buttom.at(i)->isPushedNow()) { camera_controller.setCameraMode(ECameraMode::OVERHEAD_VIEW); }
				break;

			case 2:
				if (m_buttom.at(i)->isPushedNow()) { camera_controller.setCameraMode(ECameraMode::SIDE_VIEW); }
				break;

			case 3:
				if (m_buttom.at(i)->isPushedNow()) { camera_controller.setCameraMode(ECameraMode::OVERHEAD_VIEW_FLIP); }
				break;

			case 4:
				if (m_buttom.at(i)->isPushedNow()) { camera_controller.setCameraMode(ECameraMode::SIDE_VIEW_FLIP); }
				break;

			case 5:
				if (m_buttom.at(i)->isPushedNow())
				{
					if (m_mode == ENodeDisplayNode::SELECTABLE)m_mode = ENodeDisplayNode::ALWAYS_NEW;
					else if (m_mode == ENodeDisplayNode::ALWAYS_NEW)m_mode = ENodeDisplayNode::AUTO_UPDATE;
					else if (m_mode == ENodeDisplayNode::AUTO_UPDATE)m_mode = ENodeDisplayNode::SELECTABLE;
				}
				break;

			case 6:
				if (m_buttom.at(i)->isPushedNow()) { display_node--; }
				break;

			case 7:
				if (m_buttom.at(i)->isPushedNow()) { display_node++; }
				break;

			default:
				break;
			}
		}
	}


	if (display_node < 0) { display_node = 0; }
	if (display_node >= max_node) { display_node = max_node - 1; }
}


void GUIController::draw(const SNode& node) const
{
	//表示しないなら即終了
	if (m_is_displayed == false) { return; }

	const int kBaseColor = GetColor(255, 255, 255);

	//半透明に描画する．
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);

	//下地を描画する
	DrawBox(CENTER_X - BOX_X / 2, CENTER_Y - BOX_Y / 2, CENTER_X + BOX_X / 2, CENTER_Y + BOX_Y / 2, kBaseColor, TRUE);

	//右側
	DrawBox(GraphicConst::WIN_X - (CENTER_X - BOX_X / 2), CENTER_Y - BOX_Y / 2, GraphicConst::WIN_X - (CENTER_X + BOX_X / 2), CENTER_Y + BOX_Y / 2, kBaseColor, TRUE);

	//半透明から元に戻す．これ忘れると描画が崩壊するので注意
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//ノードの状態を文字で出力する．
	drawNodeByStr(node);

	drawExplanationByStr();

	//ボタンを描画する．
	for (const auto& i : m_buttom)
	{
		i->draw();
	}
}


void GUIController::drawNodeByStr(const SNode node) const
{
	const int kStrColor = GetColor(54, 54, 54);
	const int kLineHeight = 28;

	//現在の行数から表示する地点を割り出してくれる関数オブジェクト．
	int now_line = 0;
	auto line = [kLineHeight, &now_line]() -> int { return kLineHeight * (now_line++); };

	//長くなったのでスコープの中にいれておく，別に入れなくとも処理は変わらないのだけど，visual stdioの機能で折りたためるので見た目を整理できる．

	//脚状態
	{
		std::string leg_state_bit_str = "";

		//int 型は32bitなので 32回ループする．
		const int kIntegerBitSize = 32;

		for (int i = 4; i < kIntegerBitSize; i++)
		{
			//表示する都合上，上位bitから調べる．
			if (node.leg_state & 1 << (kIntegerBitSize - i - 1))
			{
				//bitがたっているなら
				leg_state_bit_str += "1";
			}
			else
			{
				//bitがたっていないなら
				leg_state_bit_str += "0";
			}

			// 4で割った余りが3なら，4bitごとにスペースをいれたい．
			if (i % 4 == 3)
			{
				leg_state_bit_str += " ";
			}
		}

		DrawFormatString(CENTER_X - BOX_X / 2, CENTER_Y - BOX_Y / 2 + line(), kStrColor, "脚状態");
		DrawFormatString(CENTER_X - BOX_X / 2, CENTER_Y - BOX_Y / 2 + line(), kStrColor, leg_state_bit_str.c_str());
		line();		//改行
	}

	//脚の細かい情報．
	{
		std::string temp_str = "";
		auto ground_or_lift = [](int _state, int _num) -> std::string
		{
			if (dl_leg::isGrounded(_state, _num) == true) { return "接地"; }
			else { return "遊脚"; }
		};

		DrawFormatString(CENTER_X - BOX_X / 2, CENTER_Y - BOX_Y / 2 + line(), kStrColor, "脚５　　　　　　脚０");
		temp_str = "　" + ground_or_lift(node.leg_state, 5) + "　　　　　　" + ground_or_lift(node.leg_state, 0);
		DrawFormatString(CENTER_X - BOX_X / 2, CENTER_Y - BOX_Y / 2 + line(), kStrColor, temp_str.c_str());
		temp_str = "　離散化位置：" + std::to_string(dl_leg::getLegState(node.leg_state, 5)) + " 離散化位置：" + std::to_string(dl_leg::getLegState(node.leg_state, 0));
		DrawFormatString(CENTER_X - BOX_X / 2, CENTER_Y - BOX_Y / 2 + line(), kStrColor, temp_str.c_str());

		DrawFormatString(CENTER_X - BOX_X / 2, CENTER_Y - BOX_Y / 2 + line(), kStrColor, "脚４　　　　　　脚１");
		temp_str = "　" + ground_or_lift(node.leg_state, 4) + "　　　　　　" + ground_or_lift(node.leg_state, 1);
		DrawFormatString(CENTER_X - BOX_X / 2, CENTER_Y - BOX_Y / 2 + line(), kStrColor, temp_str.c_str());
		temp_str = "　離散化位置：" + std::to_string(dl_leg::getLegState(node.leg_state, 4)) + " 離散化位置：" + std::to_string(dl_leg::getLegState(node.leg_state, 1));
		DrawFormatString(CENTER_X - BOX_X / 2, CENTER_Y - BOX_Y / 2 + line(), kStrColor, temp_str.c_str());

		DrawFormatString(CENTER_X - BOX_X / 2, CENTER_Y - BOX_Y / 2 + line(), kStrColor, "脚３　　　　　　脚２");
		temp_str = "　" + ground_or_lift(node.leg_state, 3) + "　　　　　　" + ground_or_lift(node.leg_state, 2);
		DrawFormatString(CENTER_X - BOX_X / 2, CENTER_Y - BOX_Y / 2 + line(), kStrColor, temp_str.c_str());
		temp_str = "　離散化位置：" + std::to_string(dl_leg::getLegState(node.leg_state, 3)) + " 離散化位置：" + std::to_string(dl_leg::getLegState(node.leg_state, 2));
		DrawFormatString(CENTER_X - BOX_X / 2, CENTER_Y - BOX_Y / 2 + line(), kStrColor, temp_str.c_str());
	}

	//重心について
	{
		std::string str = "重心パターン" + std::to_string(dl_leg::getComPatternState(node.leg_state));

		line();	//改行
		DrawFormatString(CENTER_X - BOX_X / 2, CENTER_Y - BOX_Y / 2 + line(), kStrColor, str.c_str());

		str = "重心座標";
		DrawFormatString(CENTER_X - BOX_X / 2, CENTER_Y - BOX_Y / 2 + line(), kStrColor, str.c_str());
		str = "　x = " + std::to_string(node.global_center_of_mass.x);
		DrawFormatString(CENTER_X - BOX_X / 2, CENTER_Y - BOX_Y / 2 + line(), kStrColor, str.c_str());
		str = "　y = " + std::to_string(node.global_center_of_mass.y);
		DrawFormatString(CENTER_X - BOX_X / 2, CENTER_Y - BOX_Y / 2 + line(), kStrColor, str.c_str());
		str = "　z = " + std::to_string(node.global_center_of_mass.z);
		DrawFormatString(CENTER_X - BOX_X / 2, CENTER_Y - BOX_Y / 2 + line(), kStrColor, str.c_str());
	}

	//回転について
	{
		std::string str = "回転";

		line();	//改行
		DrawFormatString(CENTER_X - BOX_X / 2, CENTER_Y - BOX_Y / 2 + line(), kStrColor, str.c_str());

		str = "　pitch = " + std::to_string(node.rot.pitch);
		DrawFormatString(CENTER_X - BOX_X / 2, CENTER_Y - BOX_Y / 2 + line(), kStrColor, str.c_str());
		str = "　roll  = " + std::to_string(node.rot.roll);
		DrawFormatString(CENTER_X - BOX_X / 2, CENTER_Y - BOX_Y / 2 + line(), kStrColor, str.c_str());
		str = "　yaw   = " + std::to_string(node.rot.yaw);
		DrawFormatString(CENTER_X - BOX_X / 2, CENTER_Y - BOX_Y / 2 + line(), kStrColor, str.c_str());
	}
}


void GUIController::drawExplanationByStr() const
{
	const int kRightX = GraphicConst::WIN_X - (CENTER_X + BOX_X / 2);
	const int kStrColor = GetColor(54, 54, 54);
	const int kLineHeight = 28;

	//現在の行数から表示する地点を割り出してくれる関数オブジェクト．
	int now_line = 0;
	auto line = [kLineHeight, &now_line]() -> int { return kLineHeight * (now_line++); };

	DrawFormatString(kRightX, CENTER_Y - BOX_Y / 2 + line(), kStrColor, "Zキーを押すことでUIの表示を消けます．");
	DrawFormatString(kRightX, CENTER_Y - BOX_Y / 2 + line(), kStrColor, "もう一度Zキーを押すことで戻ります.");
	DrawFormatString(kRightX, CENTER_Y - BOX_Y / 2 + line(), kStrColor, "基本的にはクリックで操作をします．");
	DrawFormatString(kRightX, CENTER_Y - BOX_Y / 2 + line(), kStrColor, "以下のボタンでカメラの視点を変更．");

	//ボタンが置いてある分だけ改行する
	for (int i = 0; i < 7; i++) { line(); }

	{
		std::string str = "表示方法：";

		if (m_mode == ENodeDisplayNode::ALWAYS_NEW) { str += "常に最新．"; }
		else if (m_mode == ENodeDisplayNode::AUTO_UPDATE) { str += "自動で更新"; }
		else if (m_mode == ENodeDisplayNode::SELECTABLE) { str += "ボタンで選択"; }
		DrawFormatString(kRightX, CENTER_Y - BOX_Y / 2 + line(), kStrColor, str.c_str());
	}

}
