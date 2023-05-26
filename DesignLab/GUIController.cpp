#include "GUIController.h"
#include "DxLib.h"
#include "LegState.h"
#include <string>
#include "Keyboard.h"

GUIController::GUIController()
{
	const int RIGHTX = GraphicConst::WIN_X - (CENTER_X + BOX_X / 2);
	const int RIGHTY = CENTER_Y - BOX_Y / 2;

	const int _camera_buttomx = 100;
	const int _camera_buttomy = 50;
	const int _camera_y = RIGHTY + 150;
	m_buttom.push_back(std::make_unique<ButtomController>(RIGHTX + _camera_buttomx * 6 / 4, _camera_y, _camera_buttomx, _camera_buttomy, "真上カメラ"));
	m_buttom.push_back(std::make_unique<ButtomController>(RIGHTX + _camera_buttomx * 9 / 4, _camera_y + _camera_buttomy * 5 / 4, _camera_buttomx, _camera_buttomy, "俯瞰カメラ"));
	m_buttom.push_back(std::make_unique<ButtomController>(RIGHTX + _camera_buttomx * 9 / 4, _camera_y + _camera_buttomy * 10 / 4, _camera_buttomx, _camera_buttomy, "真横カメラ"));
	m_buttom.push_back(std::make_unique<ButtomController>(RIGHTX + _camera_buttomx * 3 / 4, _camera_y + _camera_buttomy * 5 / 4, _camera_buttomx, _camera_buttomy, "俯瞰(反転)"));
	m_buttom.push_back(std::make_unique<ButtomController>(RIGHTX + _camera_buttomx * 3 / 4, _camera_y + _camera_buttomy * 10 / 4, _camera_buttomx, _camera_buttomy, "真横(反転)"));
}

void GUIController::update(CameraController& _camera)
{
	//表示切替キーが押されたならば，表示切り返えフラグの値をさかさまにする．
	if (Keyboard::getIns()->getPressingCount(KEY_INPUT_Z) == 1) { m_is_displayed = !m_is_displayed; }

	//表示していない時にボタンの処理を行わない
	if (m_is_displayed == false) { return; }

	//ボタンを更新する．
	for (int i = 0; i < (int)m_buttom.size(); i++)
	{
		m_buttom.at(i)->update();

		//ボタンの追加された順番ごとに処理をする．クソみたいな実装でスマソ
		switch (i)
		{
		case 0:
			if (m_buttom.at(i)->isPushedNow() == true) { _camera.setCameraMode(ECameraMode::TopView); }
			break;

		case 1:
			if (m_buttom.at(i)->isPushedNow() == true) { _camera.setCameraMode(ECameraMode::OverheadView); }
			break;

		case 2:
			if (m_buttom.at(i)->isPushedNow() == true) { _camera.setCameraMode(ECameraMode::SideView); }
			break;

		case 3:
			if (m_buttom.at(i)->isPushedNow() == true) { _camera.setCameraMode(ECameraMode::OverheadViewFlip); }
			break;

		case 4:
			if (m_buttom.at(i)->isPushedNow() == true) { _camera.setCameraMode(ECameraMode::SideViewFlip); }
			break;

		default:
			break;
		}
	}
}

void GUIController::draw(const SNode _node) const
{
	//表示しないなら即終了
	if (m_is_displayed == false) { return; }

	const int _base_color = GetColor(255, 255, 255);

	//半透明に描画する．
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);

	//下地を描画する
	DrawBox(CENTER_X - BOX_X / 2, CENTER_Y - BOX_Y / 2, CENTER_X + BOX_X / 2, CENTER_Y + BOX_Y / 2, _base_color, TRUE);

	//右側
	DrawBox(GraphicConst::WIN_X - (CENTER_X - BOX_X / 2), CENTER_Y - BOX_Y / 2, GraphicConst::WIN_X - (CENTER_X + BOX_X / 2), CENTER_Y + BOX_Y / 2, _base_color, TRUE);

	//半透明から元に戻す．これ忘れると描画が崩壊するので注意
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//ノードの状態を文字で出力する．
	drawNodeByStr(_node);

	drawExplanationByStr();

	//ボタンを描画する．
	for (const auto& i : m_buttom)
	{
		i->draw();
	}
}

void GUIController::drawNodeByStr(const SNode _node) const
{
	const int _str_color = GetColor(54, 54, 54);
	const int _line_height = 28;

	//現在の行数から表示する地点を割り出してくれる関数オブジェクト．
	int _now_line = 0;
	auto line = [_line_height, &_now_line]() -> int { return _line_height * (_now_line++); };

	//長くなったのでスコープの中にいれておく，別に入れなくとも処理は変わらないのだけど，visual stdioの機能で折りたためるので見た目を整理できる．

	//脚状態
	{
		std::string _leg_state_bit_str = "";

		//int 型は32bitなので 32回ループする．
		const int _INTEGER_BIT_SIZE = 32;

		for (int i = 4; i < _INTEGER_BIT_SIZE; i++)
		{
			//表示する都合上，上位bitから調べる．
			if (_node.leg_state & 1 << (_INTEGER_BIT_SIZE - i - 1))
			{
				//bitがたっているなら
				_leg_state_bit_str += "1";
			}
			else
			{
				//bitがたっていないなら
				_leg_state_bit_str += "0";
			}

			// 4で割った余りが3なら，4bitごとにスペースをいれたい．
			if (i % 4 == 3)
			{
				_leg_state_bit_str += " ";
			}
		}

		DrawFormatString(CENTER_X - BOX_X / 2, CENTER_Y - BOX_Y / 2 + line(), _str_color, "脚状態");
		DrawFormatString(CENTER_X - BOX_X / 2, CENTER_Y - BOX_Y / 2 + line(), _str_color, _leg_state_bit_str.c_str());
		line();		//改行
	}

	//脚の細かい情報．
	{
		std::string temp_str = "";
		auto _ground_or_lift = [](int _state, int _num) -> std::string
		{
			if (LegState::isGrounded(_state, _num) == true) { return "接地"; }
			else { return "遊脚"; }
		};

		DrawFormatString(CENTER_X - BOX_X / 2, CENTER_Y - BOX_Y / 2 + line(), _str_color, "脚５　　　　　　脚０");
		temp_str = "　" + _ground_or_lift(_node.leg_state, 5) + "　　　　　　" + _ground_or_lift(_node.leg_state, 0);
		DrawFormatString(CENTER_X - BOX_X / 2, CENTER_Y - BOX_Y / 2 + line(), _str_color, temp_str.c_str());
		temp_str = "　離散化位置：" + std::to_string(LegState::getLegState(_node.leg_state, 5)) + " 離散化位置：" + std::to_string(LegState::getLegState(_node.leg_state, 0));
		DrawFormatString(CENTER_X - BOX_X / 2, CENTER_Y - BOX_Y / 2 + line(), _str_color, temp_str.c_str());

		DrawFormatString(CENTER_X - BOX_X / 2, CENTER_Y - BOX_Y / 2 + line(), _str_color, "脚４　　　　　　脚１");
		temp_str = "　" + _ground_or_lift(_node.leg_state, 4) + "　　　　　　" + _ground_or_lift(_node.leg_state, 1);
		DrawFormatString(CENTER_X - BOX_X / 2, CENTER_Y - BOX_Y / 2 + line(), _str_color, temp_str.c_str());
		temp_str = "　離散化位置：" + std::to_string(LegState::getLegState(_node.leg_state, 4)) + " 離散化位置：" + std::to_string(LegState::getLegState(_node.leg_state, 1));
		DrawFormatString(CENTER_X - BOX_X / 2, CENTER_Y - BOX_Y / 2 + line(), _str_color, temp_str.c_str());

		DrawFormatString(CENTER_X - BOX_X / 2, CENTER_Y - BOX_Y / 2 + line(), _str_color, "脚３　　　　　　脚２");
		temp_str = "　" + _ground_or_lift(_node.leg_state, 3) + "　　　　　　" + _ground_or_lift(_node.leg_state, 2);
		DrawFormatString(CENTER_X - BOX_X / 2, CENTER_Y - BOX_Y / 2 + line(), _str_color, temp_str.c_str());
		temp_str = "　離散化位置：" + std::to_string(LegState::getLegState(_node.leg_state, 3)) + " 離散化位置：" + std::to_string(LegState::getLegState(_node.leg_state, 2));
		DrawFormatString(CENTER_X - BOX_X / 2, CENTER_Y - BOX_Y / 2 + line(), _str_color, temp_str.c_str());
	}

	//重心について
	{
		std::string str = "重心パターン" + std::to_string(LegState::getComPatternState(_node.leg_state));

		line();	//改行
		DrawFormatString(CENTER_X - BOX_X / 2, CENTER_Y - BOX_Y / 2 + line(), _str_color, str.c_str());

		str = "重心座標";
		DrawFormatString(CENTER_X - BOX_X / 2, CENTER_Y - BOX_Y / 2 + line(), _str_color, str.c_str());
		str = "　x = " + std::to_string(_node.global_center_of_mass.x);
		DrawFormatString(CENTER_X - BOX_X / 2, CENTER_Y - BOX_Y / 2 + line(), _str_color, str.c_str());
		str = "　y = " + std::to_string(_node.global_center_of_mass.y);
		DrawFormatString(CENTER_X - BOX_X / 2, CENTER_Y - BOX_Y / 2 + line(), _str_color, str.c_str());
		str = "　z = " + std::to_string(_node.global_center_of_mass.z);
		DrawFormatString(CENTER_X - BOX_X / 2, CENTER_Y - BOX_Y / 2 + line(), _str_color, str.c_str());
	}

	//回転について
	{
		std::string str = "回転";

		line();	//改行
		DrawFormatString(CENTER_X - BOX_X / 2, CENTER_Y - BOX_Y / 2 + line(), _str_color, str.c_str());

		str = "　pitch = " + std::to_string(_node.pitch);
		DrawFormatString(CENTER_X - BOX_X / 2, CENTER_Y - BOX_Y / 2 + line(), _str_color, str.c_str());
		str = "　roll  = " + std::to_string(_node.roll);
		DrawFormatString(CENTER_X - BOX_X / 2, CENTER_Y - BOX_Y / 2 + line(), _str_color, str.c_str());
		str = "　yaw   = " + std::to_string(_node.yaw);
		DrawFormatString(CENTER_X - BOX_X / 2, CENTER_Y - BOX_Y / 2 + line(), _str_color, str.c_str());
	}
}

void GUIController::drawExplanationByStr() const
{
	const int RIGHTX = GraphicConst::WIN_X - (CENTER_X + BOX_X / 2);
	const int _str_color = GetColor(54, 54, 54);
	const int _line_height = 28;

	//現在の行数から表示する地点を割り出してくれる関数オブジェクト．
	int _now_line = 0;
	auto line = [_line_height, &_now_line]() -> int { return _line_height * (_now_line++); };

	DrawFormatString(RIGHTX, CENTER_Y - BOX_Y / 2 + line(), _str_color, "Zキーを押すことでUIの表示を消けます．");
	DrawFormatString(RIGHTX, CENTER_Y - BOX_Y / 2 + line(), _str_color, "もう一度Zキーを押すことで戻ります.");
	DrawFormatString(RIGHTX, CENTER_Y - BOX_Y / 2 + line(), _str_color, "基本的にはクリックで操作をします．");
	DrawFormatString(RIGHTX, CENTER_Y - BOX_Y / 2 + line(), _str_color, "以下のボタンでカメラの視点を変更．");
}
